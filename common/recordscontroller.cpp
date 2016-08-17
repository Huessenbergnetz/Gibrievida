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
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new records controller object.
 */
RecordsController::RecordsController(QObject *parent) : BaseController(parent)
{
    m_current = nullptr;
    m_visible = false;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_timer, &QTimer::timeout, this, &RecordsController::updateDuration);

    init();
}


/*!
 * \brief Destroys the records controler object.
 *
 * If there is a current record, it will save the last state to the database.
 */
RecordsController::~RecordsController()
{
    if (m_current) {

        if (connectDb()) {

            QSqlQuery q(m_db);

            if (q.prepare(QStringLiteral("UPDATE records SET repetitions = ?, distance = ? WHERE id = ?"))) {

                q.addBindValue(m_current->repetitions());
                q.addBindValue(m_current->distance());
                q.addBindValue(m_current->databaseId());

                q.exec();
            }
        }

        delete m_current;
    }
}



/*!
 * \brief Adds a new recording identified by \c activitiy id.
 *
 * Setting a not is optional.
 */
int RecordsController::add(Activity *activity, const QString &note)
{
    if (!activity) {
        return -1;
    }

    if (m_current) {
        return -1;
    }

    if (!connectDb()) {
        return -1;
    }

    QDateTime startTime = QDateTime::currentDateTimeUtc();

    Record *r = new Record(-1, startTime, QDateTime::fromTime_t(0), 0, 0, 0.0, note);

    Activity *a = new Activity(activity, r);

    r->setActivity(a);

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO records (activity, start, note) VALUES (?, ?, ?)"))) {
        delete r;
        return -1;
    }

    q.addBindValue(a->databaseId());
    q.addBindValue(startTime.toTime_t());
    q.addBindValue(note);

    if (!q.exec()) {
        delete r;
        return -1;
    }

    r->setDatabaseId(q.lastInsertId().toInt());

    if (r->isValid()) {
        setCurrent(r);
        return r->databaseId();
    } else {
        setCurrent(nullptr);
        delete r;
        return -1;
    }
}




/*!
 * \brief Finishes the current activity.
 */
void RecordsController::finish()
{
    if (!m_current) {
        return;
    }

    if (!connectDb()) {
        return;
    }

    QDateTime endTime = QDateTime::currentDateTimeUtc();
    qint64 duration = m_current->start().secsTo(endTime);

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE records SET end = ?, duration = ?, repetitions = ?, distance = ? WHERE id = ?"))) {
        Record *current = m_current;
        setCurrent(nullptr);
        delete current;
        return;
    }

    q.addBindValue(endTime.toTime_t());
    q.addBindValue(duration);
    q.addBindValue(m_current->repetitions());
    q.addBindValue(m_current->distance());
    q.addBindValue(m_current->databaseId());

    if (!q.exec()) {
        Record *current = m_current;
        setCurrent(nullptr);
        delete current;
        return;
    }

    emit finished(current());

    setCurrent(nullptr);
}




/*!
 * \brief Removes one single recored from the database identified by \c databaseId.
 *
 * \c activity and \c category are given to the removed() signal that is emitted on success.
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
 * \brief Removes all records that are part of \c activity.
 *
 * \c category is given to the removedByActivity() signal that is emitted on success.
 */
void RecordsController::removeByActivity(int activity, int category)
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM records WHERE activity = ?"))) {
        return;
    }

    q.addBindValue(activity);

    if (!q.exec()) {
        return;
    }

    emit removedByActivity(activity, category);
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

    if (!q.exec(QStringLiteral("DELETE FROM records"))) {
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
 * \fn void RecordsController::visibleChanged(bool visible)
 * \brief Part of the \link RecordsController::visible visible \endlink property.
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
    if (!connectDb()) {
        setCurrent(nullptr);
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("SELECT r.id, r.activity, a.name, a.category, c.name, c.color, r.start, r.repetitions, r.distance, a.minrepeats, a.maxrepeats, a.distance, r.note FROM records r JOIN activities a ON a.id = r.activity JOIN categories c ON c.id = a.category WHERE r.end = 0 LIMIT 1"))) {
        setCurrent(nullptr);
        return;
    }

    if (q.next()) {
        QDateTime startTime = QDateTime::fromTime_t(q.value(6).toUInt());
        Record *r = new Record(q.value(0).toInt(), startTime, QDateTime::fromTime_t(0), startTime.secsTo(QDateTime::currentDateTimeUtc()), q.value(7).toInt(), q.value(8).toDouble(), q.value(12).toString());

        Activity *a = new Activity(q.value(1).toInt(), q.value(2).toString(), q.value(9).toInt(), q.value(10).toInt(), q.value(11).toBool(), 0, r);

        Category *c = new Category(q.value(3).toInt(), q.value(4).toString(), q.value(5).toString(), 0, a);

        a->setCategory(c);
        r->setActivity(a);

        if (r->isValid()) {
            setCurrent(r);
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
        return;
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
 * \brief Updates the current recording's duration value and string.
 */
void RecordsController::updateDuration()
{
    if (!m_current) {
        return;
    }

    m_current->setDuration(m_current->start().secsTo(QDateTime::currentDateTimeUtc()));

}



/*!
 * \brief Starts or stops the duration update timer.
 *
 * If the application is somehow visible to the user and ther is
 * a current recording, the timer will be started. Otherwise the timer will
 * be stopped.
 */
void RecordsController::startStopTimer()
{
    if (m_visible && m_current) {
#ifdef QT_DEBUG
        qDebug() << "Starting timer for duration updates.";
#endif
        m_timer->start();
    } else {
#ifdef QT_DEBUG
        qDebug() << "Stopping timer for duration updates.";
#endif
        m_timer->stop();
    }
}
