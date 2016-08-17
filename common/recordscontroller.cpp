#include "recordscontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QTimer>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new records controller object.
 */
RecordsController::RecordsController(QObject *parent) : BaseController(parent)
{
    m_currentId = -1;
    m_currentActivityId = -1;
    m_currentCategoryId = -1;
    m_currentRepetitions = 0;
    m_currentDistance = 0.0;
    m_currentMinRepetitions = 0;
    m_currentMaxRepetitions = 0;
    m_currentDistanceActive = false;
    m_visible = false;
    m_currentDuration = 0;

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
    if (m_currentId > 0) {

        if (connectDb()) {

            QSqlQuery q(m_db);

            if (q.prepare(QStringLiteral("UPDATE records SET repetitions = ?, distance = ? WHERE id = ?"))) {

                q.addBindValue(getCurrentRepetitions());
                q.addBindValue(getCurrentDistance());
                q.addBindValue(getCurrentId());

                q.exec();
            }
        }
    }
}



/*!
 * \brief Adds a new recording identified by \c activitiy id.
 *
 * Setting a not is optional.
 */
int RecordsController::add(int activity, const QString &note)
{
    if (m_currentId > -1) {
        return -1;
    }

    if (!connectDb()) {
        return -1;
    }

    QDateTime startTime = QDateTime::currentDateTimeUtc();

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO records (activity, start, note) VALUES (?, ?, ?)"))) {
        return -1;
    }

    q.addBindValue(activity);
    q.addBindValue(startTime.toTime_t());
    q.addBindValue(note);

    if (!q.exec()) {
        return -1;
    }

    int id = q.lastInsertId().toInt();

    if (!q.prepare(QStringLiteral("SELECT a.name, a.minrepeats, a.maxrepeats, a.distance, a.category, c.name as categoryname, c.color FROM activities a JOIN categories c ON c.id = a.category WHERE a.id = ?"))) {
        return -1;
    }

    q.addBindValue(activity);

    if (!q.exec()) {
        return -1;
    }

    if (q.next()) {
        setCurrentId(id);
        setCurrentActivityId(activity);
        setCurrentActivityName(q.value(0).toString());
        setCurrentMinRepetitions(q.value(1).toInt());
        setCurrentMaxRepetitions(q.value(2).toInt());
        setCurrentDistanceActive(q.value(3).toBool());
        setCurrentCategoryId(q.value(4).toInt());
        setCurrentCategoryName(q.value(5).toString());
        setCurrentCategoryColor(q.value(6).toString());
        setCurrentStartTime(startTime);
        setCurrentRepetitions(0);
        setCurrentDistance(0.0);
    } else {
        return -1;
    }

    return id;
}




/*!
 * \brief Finishes the current activity.
 */
void RecordsController::finish()
{
    if (m_currentId < 0) {
        return;
    }

    if (!connectDb()) {
        return;
    }

    QDateTime endTime = QDateTime::currentDateTimeUtc();
    qint64 duration = m_currentStartTime.secsTo(endTime);

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE records SET end = ?, duration = ?, repetitions = ?, distance = ? WHERE id = ?"))) {
        qDebug() << q.lastError().text();
        return;
    }

    q.addBindValue(endTime.toTime_t());
    q.addBindValue(duration);
    q.addBindValue(getCurrentRepetitions());
    q.addBindValue(getCurrentDistance());
    q.addBindValue(getCurrentId());

    if (!q.exec()) {
        qDebug() << q.lastError().text();
        return;
    }

    emit finished(getCurrentId(), getCurrentActivityId(), getCurrentCategoryId());

    setCurrentId(-1);
}




/*!
 * \brief Removes one single recored from the database identified by \c databaseId.
 *
 * \c activity and \c category are given to the removed() signal that is emitted on success.
 */
void RecordsController::remove(int databaseId, int activity, int category)
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM records WHERE id = ?"))) {
        return;
    }

    q.addBindValue(databaseId);

    if (!q.exec()) {
        return;
    }

    emit removed(databaseId, activity, category);
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
 * \property RecordsController::currentId
 * \brief The database id of the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentId() const</TD></TR><TR><TD>void</TD><TD>setCurrentId(int currentId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentIdChanged(int currentId)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentIdChanged(int currentId)
 * \brief Part of the \link RecordsController::currentId currentId \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentId currentId \endlink property.
 */
int RecordsController::getCurrentId() const { return m_currentId; }

/*!
 * \brief Part of the \link RecordsController::currentId currentId \endlink property.
 */
void RecordsController::setCurrentId(int currentId)
{
    if (currentId != m_currentId) {
        m_currentId = currentId;
#ifdef QT_DEBUG
        qDebug() << "Changed currentId to" << m_currentId;
#endif

        startStopTimer();

        emit currentIdChanged(getCurrentId());
    }
}




/*!
 * \property RecordsController::currentActivityId
 * \brief Database ID of the currently recorded activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentActivityId() const</TD></TR><TR><TD>void</TD><TD>setCurrentActivityId(int currentActivityId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentActivityIdChanged(int currentActivityId)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentActivityIdChanged(int currentActivityId)
 * \brief Part of the \link RecordsController::currentActivityId currentActivityId \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentActivityId currentActivityId \endlink property.
 */
int RecordsController::getCurrentActivityId() const { return m_currentActivityId; }

/*!
 * \brief Part of the \link RecordsController::currentActivityId currentActivityId \endlink property.
 */
void RecordsController::setCurrentActivityId(int currentActivityId)
{
    if (currentActivityId != m_currentActivityId) {
        m_currentActivityId = currentActivityId;
#ifdef QT_DEBUG
        qDebug() << "Changed currentActivityId to" << m_currentActivityId;
#endif
        emit currentActivityIdChanged(getCurrentActivityId());
    }
}




/*!
 * \property RecordsController::currentActivityName
 * \brief Name of the currently recorded activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getCurrentActivityName() const</TD></TR><TR><TD>void</TD><TD>setCurrentActivityName(const QString &currentActivityName)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentActivityNameChanged(const QString &currentActivityName)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentActivityNameChanged(const QString &currentActivityName)
 * \brief Part of the \link RecordsController::currentActivityName currentActivityName \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentActivityName currentActivityName \endlink property.
 */
QString RecordsController::getCurrentActivityName() const { return m_currentActivityName; }

/*!
 * \brief Part of the \link RecordsController::currentActivityName currentActivityName \endlink property.
 */
void RecordsController::setCurrentActivityName(const QString &currentActivityName)
{
    if (currentActivityName != m_currentActivityName) {
        m_currentActivityName = currentActivityName;
#ifdef QT_DEBUG
        qDebug() << "Changed currentActivityName to" << m_currentActivityName;
#endif
        emit currentActivityNameChanged(getCurrentActivityName());
    }
}




/*!
 * \property RecordsController::currentCategoryId
 * \brief Database ID of the currents activity category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentCategoryId() const</TD></TR><TR><TD>void</TD><TD>setCurrentCategoryId(int currentCategoryId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentCategoryIdChanged(int currentCategoryId)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentCategoryIdChanged(int currentCategoryId)
 * \brief Part of the \link RecordsController::currentCategoryId currentCategoryId \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentCategoryId currentCategoryId \endlink property.
 */
int RecordsController::getCurrentCategoryId() const { return m_currentCategoryId; }

/*!
 * \brief Part of the \link RecordsController::currentCategoryId currentCategoryId \endlink property.
 */
void RecordsController::setCurrentCategoryId(int currentCategoryId)
{
    if (currentCategoryId != m_currentCategoryId) {
        m_currentCategoryId = currentCategoryId;
#ifdef QT_DEBUG
        qDebug() << "Changed currentCategoryId to" << m_currentCategoryId;
#endif
        emit currentCategoryIdChanged(getCurrentCategoryId());
    }
}




/*!
 * \property RecordsController::currentCategoryName
 * \brief Name of the current activities category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getCurrentCategoryName() const</TD></TR><TR><TD>void</TD><TD>setCurrentCategoryName(const QString &currentCategoryName)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentCategoryNameChanged(const QString &currentCategoryName)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentCategoryNameChanged(const QString &currentCategoryName)
 * \brief Part of the \link RecordsController::currentCategoryName currentCategoryName \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentCategoryName currentCategoryName \endlink property.
 */
QString RecordsController::getCurrentCategoryName() const { return m_currentCategoryName; }

/*!
 * \brief Part of the \link RecordsController::currentCategoryName currentCategoryName \endlink property.
 */
void RecordsController::setCurrentCategoryName(const QString &currentCategoryName)
{
    if (currentCategoryName != m_currentCategoryName) {
        m_currentCategoryName = currentCategoryName;
#ifdef QT_DEBUG
        qDebug() << "Changed currentCategoryName to" << m_currentCategoryName;
#endif
        emit currentCategoryNameChanged(getCurrentCategoryName());
    }
}




/*!
 * \property RecordsController::currentCategoryColor
 * \brief Color value of the current activities category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getCurrentCategoryColor() const</TD></TR><TR><TD>void</TD><TD>setCurrentCategoryColor(const QString &currentCategoryColor)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentCategoryColorChanged(const QString &currentCategoryColor)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentCategoryColorChanged(const QString &currentCategoryColor)
 * \brief Part of the \link RecordsController::currentCategoryColor currentCategoryColor \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentCategoryColor currentCategoryColor \endlink property.
 */
QString RecordsController::getCurrentCategoryColor() const { return m_currentCategoryColor; }

/*!
 * \brief Part of the \link RecordsController::currentCategoryColor currentCategoryColor \endlink property.
 */
void RecordsController::setCurrentCategoryColor(const QString &currentCategoryColor)
{
    if (currentCategoryColor != m_currentCategoryColor) {
        m_currentCategoryColor = currentCategoryColor;
#ifdef QT_DEBUG
        qDebug() << "Changed currentCategoryColor to" << m_currentCategoryColor;
#endif
        emit currentCategoryColorChanged(getCurrentCategoryColor());
    }
}




/*!
 * \property RecordsController::currentStartTime
 * \brief Start time of the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QDateTime</TD><TD>getCurrentStartTime() const</TD></TR><TR><TD>void</TD><TD>setCurrentStartTime(const QDateTime &currentStartTime)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentStartTimeChanged(const QDateTime &currentStartTime)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentStartTimeChanged(const QDateTime &currentStartTime)
 * \brief Part of the \link RecordsController::currentStartTime currentStartTime \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentStartTime currentStartTime \endlink property.
 */
QDateTime RecordsController::getCurrentStartTime() const { return m_currentStartTime; }

/*!
 * \brief Part of the \link RecordsController::currentStartTime currentStartTime \endlink property.
 */
void RecordsController::setCurrentStartTime(const QDateTime &currentStartTime)
{
    if (currentStartTime != m_currentStartTime) {
        m_currentStartTime = currentStartTime;
#ifdef QT_DEBUG
        qDebug() << "Changed currentStartTime to" << m_currentStartTime;
#endif
        emit currentStartTimeChanged(getCurrentStartTime());
    }
}




/*!
 * \property RecordsController::currentRepetitions
 * \brief Actual repetition value of the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentRepetitions() const</TD></TR><TR><TD>void</TD><TD>setCurrentRepetitions(int currentRepetitions)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentRepetitionsChanged(int currentRepetitions)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentRepetitionsChanged(int currentRepetitions)
 * \brief Part of the \link RecordsController::currentRepetitions currentRepetitions \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentRepetitions currentRepetitions \endlink property.
 */
int RecordsController::getCurrentRepetitions() const { return m_currentRepetitions; }

/*!
 * \brief Part of the \link RecordsController::currentRepetitions currentRepetitions \endlink property.
 */
void RecordsController::setCurrentRepetitions(int currentRepetitions)
{
    if (currentRepetitions != m_currentRepetitions) {
        m_currentRepetitions = currentRepetitions;
#ifdef QT_DEBUG
        qDebug() << "Changed currentRepetitions to" << m_currentRepetitions;
#endif
        emit currentRepetitionsChanged(getCurrentRepetitions());
    }
}




/*!
 * \property RecordsController::currentDistance
 * \brief Actual distance of the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>float</TD><TD>getCurrentDistance() const</TD></TR><TR><TD>void</TD><TD>setCurrentDistance(float currentDistance)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentDistanceChanged(float currentDistance)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentDistanceChanged(float currentDistance)
 * \brief Part of the \link RecordsController::currentDistance currentDistance \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentDistance currentDistance \endlink property.
 */
float RecordsController::getCurrentDistance() const { return m_currentDistance; }

/*!
 * \brief Part of the \link RecordsController::currentDistance currentDistance \endlink property.
 */
void RecordsController::setCurrentDistance(float currentDistance)
{
    if (currentDistance != m_currentDistance) {
        m_currentDistance = currentDistance;
#ifdef QT_DEBUG
        qDebug() << "Changed currentDistance to" << m_currentDistance;
#endif
        emit currentDistanceChanged(getCurrentDistance());
    }
}



/*!
 * \property RecordsController::currentMinRepetitions
 * \brief The minimum repetitions for the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentMinRepetitions() const</TD></TR><TR><TD>void</TD><TD>setCurrentMinRepetitions(int currentMinRepetitions)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentMinRepetitionsChanged(int currentMinRepetitions)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentMinRepetitionsChanged(int currentMinRepetitions)
 * \brief Part of the \link RecordsController::currentMinRepetitions currentMinRepetitions \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentMinRepetitions currentMinRepetitions \endlink property.
 */
int RecordsController::getCurrentMinRepetitions() const { return m_currentMinRepetitions; }

/*!
 * \brief Part of the \link RecordsController::currentMinRepetitions currentMinRepetitions \endlink property.
 */
void RecordsController::setCurrentMinRepetitions(int currentMinRepetitions)
{
    if (currentMinRepetitions != m_currentMinRepetitions) {
        m_currentMinRepetitions = currentMinRepetitions;
#ifdef QT_DEBUG
        qDebug() << "Changed currentMinRepetitions to" << m_currentMinRepetitions;
#endif
        emit currentMinRepetitionsChanged(getCurrentMinRepetitions());
    }
}




/*!
 * \property RecordsController::currentMaxRepetitions
 * \brief The maximum repetitions for the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentMaxRepetitions() const</TD></TR><TR><TD>void</TD><TD>setCurrentMaxRepetitions(int currentMaxRepetitions)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentMaxRepetitionsChanged(int currentMaxRepetitions)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentMaxRepetitionsChanged(int currentMaxRepetitions)
 * \brief Part of the \link RecordsController::currentMaxRepetitions currentMaxRepetitions \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentMaxRepetitions currentMaxRepetitions \endlink property.
 */
int RecordsController::getCurrentMaxRepetitions() const { return m_currentMaxRepetitions; }

/*!
 * \brief Part of the \link RecordsController::currentMaxRepetitions currentMaxRepetitions \endlink property.
 */
void RecordsController::setCurrentMaxRepetitions(int currentMaxRepetitions)
{
    if (currentMaxRepetitions != m_currentMaxRepetitions) {
        m_currentMaxRepetitions = currentMaxRepetitions;
#ifdef QT_DEBUG
        qDebug() << "Changed currentMaxRepetitions to" << m_currentMaxRepetitions;
#endif
        emit currentMaxRepetitionsChanged(getCurrentMaxRepetitions());
    }
}




/*!
 * \property RecordsController::currentDistanceActive
 * \brief True if the current recording should record the distance.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>hasCurrentDistanceActive() const</TD></TR><TR><TD>void</TD><TD>setCurrentDistanceActive(bool currentDistanceActive)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentDistanceActiveChanged(bool currentDistanceActive)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentDistanceActiveChanged(bool currentDistanceActive)
 * \brief Part of the \link RecordsController::currentDistanceActive currentDistanceActive \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentDistanceActive currentDistanceActive \endlink property.
 */
bool RecordsController::hasCurrentDistanceActive() const { return m_currentDistanceActive; }

/*!
 * \brief Part of the \link RecordsController::currentDistanceActive currentDistanceActive \endlink property.
 */
void RecordsController::setCurrentDistanceActive(bool currentDistanceActive)
{
    if (currentDistanceActive != m_currentDistanceActive) {
        m_currentDistanceActive = currentDistanceActive;
#ifdef QT_DEBUG
        qDebug() << "Changed currentDistanceActive to" << m_currentDistanceActive;
#endif
        emit currentDistanceActiveChanged(hasCurrentDistanceActive());
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
 * \property RecordsController::currentDuration
 * \brief The duration of the current recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentDuration() const</TD></TR><TR><TD>void</TD><TD>setCurrentDuration(int currentDuration)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentDurationChanged(int currentDuration)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentDurationChanged(int currentDuration)
 * \brief Part of the \link RecordsController::currentDuration currentDuration \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentDuration currentDuration \endlink property.
 */
int RecordsController::getCurrentDuration() const { return m_currentDuration; }

/*!
 * \brief Part of the \link RecordsController::currentDuration currentDuration \endlink property.
 */
void RecordsController::setCurrentDuration(int currentDuration)
{
    if (currentDuration != m_currentDuration) {
        m_currentDuration = currentDuration;
#ifdef QT_DEBUG
        qDebug() << "Changed currentDuration to" << m_currentDuration;
#endif
        setCurrentDurationString(helpers.createDurationString(m_currentDuration));
        emit currentDurationChanged(getCurrentDuration());
    }
}




/*!
 * \property RecordsController::currentDurationString
 * \brief Human readable version of the current duration.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getCurrentDurationString() const</TD></TR><TR><TD>void</TD><TD>setCurrentDurationString(const QString &currentDurationString)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentDurationStringChanged(const QString &currentDurationString)</TD></TR></TABLE>
 */

/*!
 * \fn void RecordsController::currentDurationStringChanged(const QString &currentDurationString)
 * \brief Part of the \link RecordsController::currentDurationString currentDurationString \endlink property.
 */

/*!
 * \brief Part of the \link RecordsController::currentDurationString currentDurationString \endlink property.
 */
QString RecordsController::getCurrentDurationString() const { return m_currentDurationString; }

/*!
 * \brief Part of the \link RecordsController::currentDurationString currentDurationString \endlink property.
 */
void RecordsController::setCurrentDurationString(const QString &currentDurationString)
{
    if (currentDurationString != m_currentDurationString) {
        m_currentDurationString = currentDurationString;
#ifdef QT_DEBUG
        qDebug() << "Changed currentDurationString to" << m_currentDurationString;
#endif
        emit currentDurationStringChanged(getCurrentDurationString());
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
        setCurrentId(-1);
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("SELECT r.id, r.activity, a.name, a.category, c.name, c.color, r.start, r.repetitions, r.distance, a.minrepeats, a.maxrepeats, a.distance FROM records r JOIN activities a ON a.id = r.activity JOIN categories c ON c.id = a.category WHERE r.end = 0 LIMIT 1"))) {
        setCurrentId(-1);
        return;
    }

    if (q.next()) {
        setCurrentId(q.value(0).toInt());
        setCurrentActivityId(q.value(1).toInt());
        setCurrentActivityName(q.value(2).toString());
        setCurrentCategoryId(q.value(3).toInt());
        setCurrentCategoryName(q.value(4).toString());
        setCurrentCategoryColor(q.value(5).toString());
        setCurrentStartTime(QDateTime::fromTime_t(q.value(6).toUInt()));
        setCurrentRepetitions(q.value(7).toInt());
        setCurrentDistance(q.value(8).toFloat());
        setCurrentMinRepetitions(q.value(9).toInt());
        setCurrentMaxRepetitions(q.value(10).toInt());
        setCurrentDistanceActive(q.value(11).toBool());
        setCurrentDuration(m_currentStartTime.secsTo(QDateTime::currentDateTimeUtc()));
    } else {
        setCurrentId(-1);
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
    if (m_currentId < 0) {
        return;
    }

    int reps = m_currentRepetitions + 1;

    if (reps <= m_currentMaxRepetitions) {
        setCurrentRepetitions(reps);
    } else {
        setCurrentRepetitions(m_currentMaxRepetitions);
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
    if (m_currentId < 0) {
        return;
    }

    int reps = m_currentRepetitions - 1;

    if (reps >= m_currentMinRepetitions) {
        setCurrentRepetitions(reps);
    } else {
        setCurrentRepetitions(m_currentMinRepetitions);
    }
}




/*!
 * \brief Updates the current recording's duration value and string.
 */
void RecordsController::updateDuration()
{
    if (m_currentId < 0) {
        return;
    }

    setCurrentDuration(m_currentStartTime.secsTo(QDateTime::currentDateTimeUtc()));
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
    if (m_visible && (m_currentId > 0)) {
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
