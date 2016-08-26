/*
    Gibrievida - An activity tracker
    Copyright (C) 2016 Buschtrommel/Matthias Fehring http://gibrievida.buschmann23.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "recordscontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QTimer>
#include "record.h"
#include "activity.h"
#include "category.h"
#include "configuration.h"
#include "globals.h"
#include "distancemeasurement.h"
#include <QProximitySensor>
#include <QProximityReading>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QRotationSensor>
#include <QRotationReading>
#include <QOrientationSensor>
#include <QOrientationReading>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new records controller object.
 *
 * This will also search the database for an active Record and load it as \link RecordsController::current current \endlink Record.
 */
RecordsController::RecordsController(Configuration *config, QObject *parent) : BaseController(parent), m_config(config)
{
    m_current = nullptr;
    m_visible = false;
    m_proximitySensor = nullptr;
    m_accelSensor = nullptr;
    m_sensorTimer = nullptr;
    m_rotationSensor = nullptr;
    m_orientationSensor = nullptr;
    m_finishOnCoveringTimer = nullptr;
    m_finishOnCovering = 0;
    m_distanceMeasurement = nullptr;
    m_soundPlayer = nullptr;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_timer, &QTimer::timeout, this, &RecordsController::updateDuration);

    if (m_config->repetitionClickSound() > 0) {
        updateRepetitionClickSound(m_config->repetitionClickSound());
    }

    connect(m_config, &Configuration::repetitionClickSoundChanged, this, &RecordsController::updateRepetitionClickSound);

    init();
}


/*!
 * \brief Destroys the records controler object.
 *
 * If there is a current record it will be saved to the database if it is active.
 */
RecordsController::~RecordsController()
{
    if (m_current) {

        if (m_current->isValid()) {

#ifdef QT_DEBUG
            qDebug("Synchronizing the current record data to the database.");
#endif

            if (connectDb()) {

                QSqlQuery q(m_db);

                if (q.prepare(QStringLiteral("UPDATE records SET repetitions = ?, distance = ? WHERE id = ?"))) {

                    q.addBindValue(m_current->repetitions());
                    q.addBindValue(m_current->distance());
                    q.addBindValue(m_current->databaseId());

                    q.exec();
                }
            }
        }

        delete m_current;
    }
}


/*!
 * \brief Prepares a new Record.
 *
 * This should be called before add(). It creates a new empty set of Record, Activity and Category
 * to populate the \link RecordsController::current current \endlink Record property in order to have
 * minimal valid data for further operations, especially for UI display.
 */
void RecordsController::prepare()
{
#ifdef QT_DEBUG
    qDebug() << "Preaparing a new record";
#endif

    if (m_current && m_current->isValid()) {
        qWarning("Current record is already set an active. Returning.");
        return;
    }

    Record *r = new Record();
    Activity *a = new Activity(r);
    Category *c = new Category(a);

    a->setCategory(c);
    r->setActivity(a);

    setCurrent(r);
}


/*!
 * \brief Cancels the current recording if any.
 *
 * Will also delete the Record from the database if it is active.
 */
void RecordsController::cancel()
{
#ifdef QT_DEBUG
    qDebug("Canceling the current recording.");
#endif

    removeSensor();

    m_finishOnCovering = 0;

    if (m_current && m_current->isValid()) {
        Record *r = current();
        int id = r->databaseId();
        setCurrent(nullptr);
        delete r;

        if (id > -1) {

            if (connectDb()) {

                QSqlQuery q(m_db);

                if (q.prepare(QStringLiteral("DELETE FROM records WHERE id = ?"))) {

                    q.addBindValue(id);

                    q.exec();

                }
            }
        }

    } else if (m_current && !m_current->isValid()) {
        Record *r = current();
        setCurrent(nullptr);
        delete r;
    }

    setDistanceMeasurement(nullptr);

    startStopTimer();
}


/*!
 * \brief Adds a new recording identified by \c activitiy.
 *
 * You should call prepare() before you call this. Setting a note is optional.
 */
int RecordsController::add(Activity *activity, const QString &note, int finishOnCovering)
{
#ifdef QT_DEBUG
    qDebug("Adding a new record.");
#endif

    if (!activity) {
        qCritical("No pointer to an activitiy is given. Returning.");
        return -1;
    }

    if (!activity->isValid()) {
        qCritical("Given activitiy is not valid. Returning.");
        return -1;
    }

    if (m_current && m_current->isValid()) {
        qWarning("Current record is already set and valid. Returning.");
        return -1;
    }

    if (!connectDb()) {
        return -1;
    }

    QDateTime startTime = QDateTime::currentDateTimeUtc();

    Record *r = nullptr;

    if (current()) {
        r = current();
        r->setStart(startTime);
        r->setNote(note);
    } else {
        r = new Record(-1, startTime, QDateTime::fromTime_t(0), 0, 0, 0.0, note, 0.0, 0.0, 0.0);
    }

    Activity *a = new Activity(activity, r);

    r->setActivity(a);

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO records (activity, start, note) VALUES (?, ?, ?)"))) {
        setCurrent(nullptr);
        delete r;
        return -1;
    }

    q.addBindValue(a->databaseId());
    q.addBindValue(startTime.toTime_t());
    q.addBindValue(note);

    if (!q.exec()) {
        setCurrent(nullptr);
        delete r;
        return -1;
    }

    r->setDatabaseId(q.lastInsertId().toInt());

    if (r->isValid()) {
        setCurrent(r);
        m_finishOnCovering = finishOnCovering;
        setSensor();
        return r->databaseId();
    } else {
        setCurrent(nullptr);
        delete r;
        return -1;
    }
}




/*!
 * \brief Finishes the current recording.
 *
 * Will set the end time and the calculated duration to the \link RecordsController::current current \endlink Record
 * and synchronizes it's data to the database. On success the finished() signal will be emitted.
 */
void RecordsController::finish()
{
#ifdef QT_DEBUG
    qDebug() << "Finishing the current recording.";
#endif

    removeSensor();

    m_timer->stop();

    if (m_finishOnCovering > 0 && current()->activity()->sensorType() == 1) {
        decreaseRepetitions();
        m_finishOnCovering = 0;
    }

    if (!m_current) {
        qWarning("No current recording set. Returning.");
        return;
    }

    if (!m_current->isValid()) {
        qWarning("Current recording is not valid. Returning");
        return;
    }

    if (!connectDb()) {
        return;
    }

    QDateTime endTime = QDateTime::currentDateTimeUtc();
    qint64 duration = m_current->start().secsTo(endTime);
    float tpr = 0.0;
    if (m_current->repetitions() > 0) {
        tpr = (float)m_current->duration()/(float)m_current->repetitions();
    }

    float avgSpeed = 0.0;
    if (m_current->distance() > 0.0) {
        avgSpeed = m_current->distance()/(double)duration;
    }

    m_timer->stop();

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE records SET end = ?, duration = ?, repetitions = ?, distance = ?, tpr = ?, maxSpeed = ?, avgSpeed = ? WHERE id = ?"))) {
        Record *current = m_current;
        setCurrent(nullptr);
        delete current;
        return;
    }

    q.addBindValue(endTime.toTime_t());
    q.addBindValue(duration);
    q.addBindValue(m_current->repetitions());
    q.addBindValue(m_current->distance());
    q.addBindValue(tpr);
    q.addBindValue(m_current->maxSpeed());
    q.addBindValue(avgSpeed);
    q.addBindValue(m_current->databaseId());

    if (!q.exec()) {
        Record *current = m_current;
        setCurrent(nullptr);
        delete current;
        return;
    }

    m_current->setEnd(endTime);
    m_current->setDuration(duration);
    m_current->setTpr(tpr);
    m_current->setAvgSpeed(avgSpeed);

    emit finished(current());

    setCurrent(nullptr);

    startStopTimer();

    if (!this->isVisible() && m_config->finishingSound() > 0) {
        playSound(QStringLiteral(FINISHING_SOUND_BASE_URL).append(QString::number(m_config->finishingSound())).append(QStringLiteral(".oga")));
//        if (!m_soundPlayer) {
//            m_soundPlayer = new QMediaPlayer(this);
//            m_soundPlayer->setVolume(100);
//            connect(m_soundPlayer, &QMediaPlayer::mediaStatusChanged, this, &RecordsController::soundPlayerStatusChanged);
//        }
//        m_soundPlayer->setMedia(QUrl::fromLocalFile(QStringLiteral(FINISHING_SOUND_BASE_URL).append(QString::number(m_config->finishingSound())).append(QStringLiteral(".oga"))));
//        m_soundPlayer->play();
    }
}




/*!
 * \brief Updates Record data in the database.
 *
 * On success the updated() signal will be emitted. \c oldActivityId is given to that signal
 * for further processing.
 */
void RecordsController::update(Record *r, int oldActivityId)
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE records SET activity = ?, start = ?, end = ?, duration = ?, repetitions = ?, distance = ?, note = ?, tpr = ?, avgSpeed = ?, maxSpeed = ? WHERE id = ?"))) {
        return;
    }

    q.addBindValue(r->activity()->databaseId());
    q.addBindValue(r->start().toTime_t());
    q.addBindValue(r->end().toTime_t());
    q.addBindValue(r->duration());
    q.addBindValue(r->repetitions());
    q.addBindValue(r->distance());
    q.addBindValue(r->note());
    q.addBindValue(r->tpr());
    q.addBindValue(r->avgSpeed());
    q.addBindValue(r->maxSpeed());
    q.addBindValue(r->databaseId());

    if (!q.exec()) {
        return;
    }

    emit updated(r, oldActivityId);
}




/*!
 * \brief Removes one single Record from the database.
 *
 * On success the removed() signal will be emitted.
 */
void RecordsController::remove(Record *r)
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM records WHERE id = ?"))) {
        return;
    }

    q.addBindValue(r->databaseId());

    if (!q.exec()) {
        return;
    }

    emit removed(r->databaseId(), r->activity()->databaseId(), r->activity()->category()->databaseId());
}



/*!
 * \brief Removes all records that are part of Activity \c a.
 *
 * On success the removedByActivity() signal will be emitted.
 */
void RecordsController::removeByActivity(Activity *a)
{
    if (!a) {
        qCritical("No actvitiy given. Returning.");
        return;
    }

    if (!a->isValid()) {
        qCritical("Invalid activity. Returning.");
        return;
    }

    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM records WHERE activity = ? AND end > 0"))) {
        return;
    }

    q.addBindValue(a->databaseId());

    if (!q.exec()) {
        return;
    }

    emit removedByActivity(a->databaseId(), a->category()->databaseId());
}



/*!
 * \brief Removes all records that are part of Category \c c.
 *
 * On success the removedByCategory() signal will be emitted.
 */
void RecordsController::removeByCategory(Category *c)
{
    if (!c) {
        qCritical("No category given. Returning.");
        return;
    }

    if (!c->isValid()) {
        qCritical("Invalid category. Returning.");
        return;
    }

    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM records WHERE end > 0 AND activity IN (SELECT id FROM activities WHERE category = ?)"))) {
        return;
    }

    q.addBindValue(c->databaseId());

    if (!q.exec()) {
        return;
    }

    emit removedByCategory(c->databaseId());
}


/*!
 * \brief Removes all records from the database.
 *
 * On success, the removedAll() signal will be emitted.
 */
void RecordsController::removeAll()
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("DELETE FROM records WHERE end > 0"))) {
        return;
    }

    emit removedAll();
}



/*!
 * \property RecordsController::current
 * \brief The currently active record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>Record*</TD><TD>current() const</TD></TR><TR><TD>void</TD><TD>setCurrent(Record *nCurrent)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentChanged(Record *current)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentChanged(Record *current)
 * \brief Part of the \link RecordsController::current current \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::current current \endlink property.
 */
Record *RecordsController::current() const { return m_current; }

/*!
 * \brief Part of the \link RecordsController::current current \endlink property.
 */
void RecordsController::setCurrent(Record *nCurrent)
{
    if (nCurrent != m_current) {
        m_current = nCurrent;
#ifdef QT_DEBUG
        qDebug() << "Changed currentRecord to" << m_current;
#endif
        emit currentChanged(current());
    }
}



/*!
 * \property RecordsController::visible
 * \brief Set this to true while the application is visible.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>isVisible() const</TD></TR><TR><TD>void</TD><TD>setVisible(bool visible)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>visibleChanged(bool visible)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsController::visible visible \endlink property.
 */
bool RecordsController::isVisible() const { return m_visible; }

/*!
 * \brief Part of the \link RecordsController::visible visible \endlink property.
 */
void RecordsController::setVisible(bool visible)
{
    if (visible != m_visible) {
        m_visible = visible;
#ifdef QT_DEBUG
        qDebug() << "Changed visible to" << m_visible;
#endif
        startStopTimer();
    }
}



/*!
 * \brief Initializes the records controller.
 *
 * This will search the database for not finished records.
 */
void RecordsController::init()
{
#ifdef QT_DEBUG
    qDebug() << "Initializing" << this;
#endif

    if (!connectDb()) {
        setCurrent(nullptr);
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("SELECT r.id, r.activity, a.name, a.category, c.name, c.color, r.start, r.repetitions, r.distance, a.minrepeats, a.maxrepeats, a.distance, r.note, r.tpr, r.maxSpeed, r.avgSpeed, a.sensor, a.sensorDelay FROM records r JOIN activities a ON a.id = r.activity JOIN categories c ON c.id = a.category WHERE r.end = 0 LIMIT 1"))) {
        setCurrent(nullptr);
        return;
    }

    if (q.next()) {
        QDateTime startTime = QDateTime::fromTime_t(q.value(6).toUInt());
        Record *r = new Record(q.value(0).toInt(), startTime, QDateTime::fromTime_t(0), startTime.secsTo(QDateTime::currentDateTimeUtc()), q.value(7).toInt(), q.value(8).toDouble(), q.value(12).toString(), q.value(13).toFloat(), q.value(14).toFloat(), q.value(15).toFloat());

        Activity *a = new Activity(q.value(1).toInt(), q.value(2).toString(), q.value(9).toInt(), q.value(10).toInt(), q.value(11).toBool(), 0, q.value(16).toInt(), q.value(17).toInt(), r);

        Category *c = new Category(q.value(3).toInt(), q.value(4).toString(), q.value(5).toString(), 0, a);

        a->setCategory(c);
        r->setActivity(a);

        if (r->isValid()) {
            setCurrent(r);
            setSensor();
        } else {
            setCurrent(nullptr);
            delete r;
        }

    } else {
        setCurrent(nullptr);
    }
}




/*!
 * \brief Increases the current repetitions by one.
 *
 * If the current repetitions will exceed the value of the current
 * maximum repetitions value, this value will be set.
 */
void RecordsController::increaseRepetitions()
{
    if (!m_current) {
        qWarning("No current record set. Returning.");
        return;
    }

    if (m_config->repetitionClickSound() > 0) {
        m_repetitionClickSound.play();
    }

    int reps = m_current->repetitions() + 1;

    if (reps <= m_current->activity()->maxRepeats()) {
        m_current->setRepetitions(reps);
    } else {
        m_current->setRepetitions(m_current->activity()->maxRepeats());
    }
}


/*!
 * \brief Decreases the current repetitions by one.
 *
 * If the current repetitions will be lower than the current value of
 * the minimum repetitions value, this value will be set.
 */
void RecordsController::decreaseRepetitions()
{
    if (!m_current) {
        qWarning("No current record set. Returning.");
        return;
    }

    int reps = m_current->repetitions() - 1;

    if (reps >= m_current->activity()->minRepeats()) {
        m_current->setRepetitions(reps);
    } else {
        m_current->setRepetitions(m_current->activity()->minRepeats());
    }
}




/*!
 * \brief Updates the current recording's duration value.
 */
void RecordsController::updateDuration()
{
    if (!m_current) {
        qWarning("No current record set. Returning.");
        return;
    }

    if (m_current && m_distanceMeasurement && !m_distanceMeasurement->isInitialPositionAvailable()) {
        return;
    }

    m_current->setDuration(m_current->start().secsTo(QDateTime::currentDateTimeUtc()));

}



/*!
 * \brief Starts or stops the duration update timer.
 *
 * If the application is somehow visible to the user and there is
 * a current recording, the timer will be started. Otherwise the timer will
 * be stopped.
 */
void RecordsController::startStopTimer()
{
    if (m_visible && m_current) {
        if (!m_timer->isActive()) {
#ifdef QT_DEBUG
        qDebug() << "Starting timer for duration updates.";
#endif
        m_timer->start();
        }
    } else {
        if (m_timer->isActive()) {
#ifdef QT_DEBUG
        qDebug() << "Stopping timer for duration updates.";
#endif
        m_timer->stop();
        }
    }
}


/*!
 * \brief Updates the URL of the local sound file, after is been changed.
 */
void RecordsController::updateRepetitionClickSound(int clickSound)
{
    if (clickSound > 0) {
        m_repetitionClickSound.setSource(QUrl::fromLocalFile(QStringLiteral(REPETITION_CLICKSOUND_BASE_URL).append(QString::number(clickSound)).append(QStringLiteral(".wav"))));
    }
}


/*!
 * \brief Sets the used sensor according to the activity sensor type.
 */
void RecordsController::setSensor()
{
    if (!current()) {
        return;
    }

    if (m_finishOnCovering > 0 && !m_finishOnCoveringTimer) {
        m_finishOnCoveringTimer = new QTimer(this);
        m_finishOnCoveringTimer->setTimerType(Qt::VeryCoarseTimer);
        m_finishOnCoveringTimer->setInterval(m_finishOnCovering * 1000);
        connect(m_finishOnCoveringTimer, &QTimer::timeout, this, &RecordsController::finish);
    }

    if (current()->activity()->useDistance()) {
        DistanceMeasurement *dm = new DistanceMeasurement(5000, this);
        connect(dm, &DistanceMeasurement::gotDistance, current(), &Record::addDistance);
        connect(dm, &DistanceMeasurement::gotSpeed, this, &RecordsController::updateMaxSpeed);
        connect(dm, &DistanceMeasurement::initialPositionAvailableChanged, this, &RecordsController::initialPositionAvailable);
        setDistanceMeasurement(dm);
    }

    if (current()->activity()->sensorType() == 1 || m_finishOnCovering > 0) {
        if (!m_proximitySensor) {
            m_proximitySensor = new QProximitySensor(this);
            m_proximitySensor->setAlwaysOn(true);
            if (current()->activity()->sensorType() == 1) {
                connect(m_proximitySensor, &QSensor::readingChanged, this, &RecordsController::proximityUpdate);
            }
            if (m_finishOnCovering > 0) {
                connect(m_proximitySensor, &QSensor::readingChanged, this, &RecordsController::detectFinishOnCovering);
            }
            m_proximitySensor->start();
        }
    }

    if (current()->activity()->sensorType() == 2 || current()->activity()->sensorType() == 3) {

        if (!m_sensorTimer) {
            m_sensorTimer = new QTimer(this);
            m_sensorTimer->setTimerType(Qt::CoarseTimer);
            if (current()->activity()->sensorDelay() > 0) {
                m_sensorTimer->setInterval(current()->activity()->sensorDelay());
            } else {
                m_sensorTimer->setInterval(2500);
            }
            m_sensorTimer->setSingleShot(true);
        }

        if (!m_accelSensor) {
            m_accelSensor = new QAccelerometer(this);
            m_accelSensor->setAlwaysOn(true);
            if (current()->activity()->sensorType() == 2) {
                connect(m_accelSensor, &QSensor::readingChanged, this, &RecordsController::detectUpDownTop);
            } else if (current()->activity()->sensorType() == 3) {
                connect(m_accelSensor, &QSensor::readingChanged, this, &RecordsController::detectUpDownFace);
            }
            m_accelSensor->setDataRate(10);
        }

        m_sensorTimer->start();
        m_accelSensor->start();
    }
}


/*!
 * \brief Reads data from the proximity sensor and increases the repetition count if user is nera to phone.
 */
void RecordsController::proximityUpdate()
{
    if (m_proximitySensor->reading()->close()) {
        increaseRepetitions();
    }
}


/*!
 * \brief Reads the data from the acceleration sensor and tries to detect an up and down movement.
 */
void RecordsController::detectUpDownTop()
{
    if (!m_sensorTimer->isActive() && m_accelSensor->reading()->y() > 9.9f) {
        qDebug() << m_accelSensor->reading()->y();
        increaseRepetitions();
        m_sensorTimer->start();
    }
}

/*!
 * \brief Reads the data from the acceleration sensor and tries to detect an up and down movement.
 */
void RecordsController::detectUpDownFace()
{
    if (!m_sensorTimer->isActive() && m_accelSensor->reading()->z() > 9.9f) {
        qDebug() << m_accelSensor->reading()->z();
        increaseRepetitions();
        m_sensorTimer->start();
    }
}


/*!
 * \brief Removes the current sensor and its connections.
 */
void RecordsController::removeSensor()
{
    if (m_proximitySensor) {
        m_proximitySensor->stop();
        disconnect(m_proximitySensor, &QSensor::readingChanged, this, &RecordsController::proximityUpdate);
        delete m_proximitySensor;
        m_proximitySensor = nullptr;
    }

    if (m_accelSensor) {
        m_accelSensor->stop();
        disconnect(m_accelSensor, &QSensor::readingChanged, this, &RecordsController::detectUpDownTop);
        disconnect(m_accelSensor, &QSensor::readingChanged, this, &RecordsController::detectUpDownFace);
        delete m_accelSensor;
        m_accelSensor = nullptr;
    }

    if (m_rotationSensor) {
        m_rotationSensor->stop();
        delete m_rotationSensor;
        m_rotationSensor = nullptr;
    }

    if (m_sensorTimer) {
        delete m_sensorTimer;
        m_sensorTimer = nullptr;
    }

    if (m_finishOnCoveringTimer) {
        delete m_finishOnCoveringTimer;
        m_finishOnCoveringTimer = nullptr;
    }

    setDistanceMeasurement(nullptr);
}

/*!
 * \brief Reads the data from the proximity sensor and tries to detect, if the recording should be finished.
 *
 * This will start a timer that runs a specified time, unless the user moves away from the device in time.
 */
void RecordsController::detectFinishOnCovering()
{
    if (m_proximitySensor->reading()->close() && !m_finishOnCoveringTimer->isActive()) {
        m_finishOnCoveringTimer->start();
    } else if (!m_proximitySensor->reading()->close() && m_finishOnCoveringTimer->isActive()) {
        m_finishOnCoveringTimer->stop();
    }
}


/*!
 * \brief Removes the mediaplayer object after the sound has been played.
 */
void RecordsController::soundPlayerStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        m_soundPlayer->stop();
        m_soundPlayer->deleteLater();
        m_soundPlayer = nullptr;
    }
}





/*!
 * \property RecordsController::distanceMeasurement
 * \brief Pointer to a DistanceMeasurement object.
 *
 * \par Access functions:
 * <TABLE><TR><TD>DistanceMeasurement*</TD><TD>distanceMeasurement() const</TD></TR><TR><TD>void</TD><TD>setDistanceMeasurement(DistanceMeasurement *nDistanceMeasurement)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>distanceMeasurementChanged(DistanceMeasurement *distanceMeasurement)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::distanceMeasurementChanged(DistanceMeasurement *distanceMeasurement)
 * \brief Part of the \link RecordsController::distanceMeasurement distanceMeasurement \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::distanceMeasurement distanceMeasurement \endlink property.
 */
DistanceMeasurement *RecordsController::distanceMeasurement() const { return m_distanceMeasurement; }

/*!
 * \brief Part of the \link RecordsController::distanceMeasurement distanceMeasurement \endlink property.
 */
void RecordsController::setDistanceMeasurement(DistanceMeasurement *nDistanceMeasurement)
{
    if (nDistanceMeasurement != m_distanceMeasurement) {

        DistanceMeasurement *old = m_distanceMeasurement;

        m_distanceMeasurement = nDistanceMeasurement;
#ifdef QT_DEBUG
        qDebug() << "Changed distanceMeasurement to" << m_distanceMeasurement;
#endif
        emit distanceMeasurementChanged(distanceMeasurement());

        if (old) {
            delete old;
        }
    }
}


/*!
 * \brief Updates the maximum speed value of the current record.
 */
void RecordsController::updateMaxSpeed(qreal speed)
{
    if (current() && current()->isValid() && current()->maxSpeed() < speed) {
        current()->setMaxSpeed(speed);
    }
}



void RecordsController::initialPositionAvailable(bool available)
{
    if (available && current()) {
        current()->setStart(QDateTime::currentDateTime());

        if (m_config->startSound() > 0) {
            playSound(QStringLiteral(START_SOUND_BASE_URL).append(QString::number(m_config->startSound())).append(QLatin1String(".oga")));
        }

        if (connectDb()) {
            QSqlQuery q(m_db);

            if (q.prepare(QStringLiteral("UPDATE records SET start = ? WHERE id = ?"))) {

                q.addBindValue(current()->start().toTime_t());
                q.addBindValue(current()->databaseId());

                q.exec();
            }

        }
    }
}



void RecordsController::playSound(const QString &soundFile)
{
    if (!m_soundPlayer) {
        m_soundPlayer = new QMediaPlayer(this);
        m_soundPlayer->setVolume(100);
        connect(m_soundPlayer, &QMediaPlayer::mediaStatusChanged, this, &RecordsController::soundPlayerStatusChanged);
    }
    m_soundPlayer->setMedia(QUrl::fromLocalFile(soundFile));
    m_soundPlayer->play();
}



void RecordsController::positionSignalLost()
{
    if (m_config->signalLostSound() > 0) {
        playSound(QStringLiteral(SIGNALLOST_SOUND_BASE_URL).append(QString::number(m_config->signalLostSound())).append(QLatin1String(".oga")));
    }
}
