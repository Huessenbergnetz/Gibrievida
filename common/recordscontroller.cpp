#include "recordscontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
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
}


/*!
 * \brief Destroys the records controler object.
 */
RecordsController::~RecordsController()
{

}



int RecordsController::add(int activity)
{
    if (!connectDb()) {
        return -1;
    }

    QDateTime startTime = QDateTime::currentDateTimeUtc();

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO records (activity, start) VALUES (?, ?)"))) {
        return -1;
    }

    q.addBindValue(activity);
    q.addBindValue(startTime.toTime_t());

    if (!q.exec()) {
        return -1;
    }

    int id = q.lastInsertId().toInt();

    emit added(id, activity, startTime);

    return id;
}



void RecordsController::finish(int databaseId, int repetitions, float distance, QString note)
{

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
