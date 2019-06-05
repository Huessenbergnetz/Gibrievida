/*
    Gibrievida - An activity tracker
    Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
    https://github.com/Huessenbergnetz/Gibrievida

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

#include "record.h"
#include "activity.h"
#include "category.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new Record object.
 */
Record::Record(QObject *parent) :
    QObject(parent)
{
    m_databaseId = -1;
    m_activity = nullptr;
    m_duration = 0;
    m_repetitions = 0;
    m_distance = 0.0;
    m_active = true;
    m_start = QDateTime::fromTime_t(0);
    m_end = QDateTime::fromTime_t(0);
    m_tpr = 0.0;
    m_maxSpeed = 0.0;
    m_avgSpeed = 0.0;

#ifdef QT_DEBUG
    qDebug() << "Constructed a new empty" << this;
#endif
}


/*!
 * \overload
 */
Record::Record(int databaseId, const QDateTime &start, const QDateTime &end, uint duration, uint repetitions, double distance, const QString &note, float tpr, float maxSpeed, float avgSpeed, QObject *parent) :
    QObject(parent), m_databaseId(databaseId), m_start(start), m_end(end), m_duration(duration), m_repetitions(repetitions), m_distance(distance), m_note(note), m_tpr(tpr), m_maxSpeed(maxSpeed), m_avgSpeed(avgSpeed)
{
    m_active = (end == QDateTime::fromTime_t(0));

#ifdef QT_DEBUG
    qDebug() << "Constructed a new" << this << "ID:" << databaseId << "Start:" << start << "End:" << end << "Duration:" << duration << "Repetitions:" << repetitions << "Distance:" << distance << "Note:" << note << "TPR:" << tpr << "Max Speed:" << maxSpeed << "Avg Speed:" << avgSpeed;
#endif
}


/*!
 * \brief Deconstructs the Record object.
 */
Record::~Record()
{
#ifdef QT_DEBUG
    qDebug() << "Destroying" << this;
#endif
}


/*!
 * \property Record::databaseId
 * \brief Database ID of this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>databaseId() const</TD></TR><TR><TD>void</TD><TD>setDatabaseId(int nDatabaseId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>databaseIdChanged(int databaseId)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::databaseIdChanged(int databaseId)
 * \brief Part of the \link Record::databaseId databaseId \endlink property.
 */

/*!
 * \brief Part of the \link Record::databaseId databaseId \endlink property.
 */
int Record::databaseId() const { return m_databaseId; }

/*!
 * \brief Part of the \link Record::databaseId databaseId \endlink property.
 */
void Record::setDatabaseId(int nDatabaseId)
{
    if (nDatabaseId != m_databaseId) {
        m_databaseId = nDatabaseId;
#ifdef QT_DEBUG
        qDebug() << "Changed databaseId to" << m_databaseId;
#endif
        emit databaseIdChanged(databaseId());
    }
}




/*!
 * \property Record::activity
 * \brief The activity connected to this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>Activity*</TD><TD>activity() const</TD></TR><TR><TD>void</TD><TD>setActivity(Activity *nActivity)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>activityChanged(Activity *activity)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::activityChanged(Activity *activity)
 * \brief Part of the \link Record::activity activity \endlink property.
 */

/*!
 * \brief Part of the \link Record::activity activity \endlink property.
 */
Activity *Record::activity() const { return m_activity; }

/*!
 * \brief Part of the \link Record::activity activity \endlink property.
 */
void Record::setActivity(Activity *nActivity)
{
    if (nActivity != m_activity) {
        m_activity = nActivity;
#ifdef QT_DEBUG
        qDebug() << "Changed activity to" << m_activity;
#endif
        emit activityChanged(activity());
    }
}




/*!
 * \property Record::start
 * \brief The start time of this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QDateTime</TD><TD>start() const</TD></TR><TR><TD>void</TD><TD>setStart(const QDateTime &nStart)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>startChanged(const QDateTime &start)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::startChanged(const QDateTime &start)
 * \brief Part of the \link Record::start start \endlink property.
 */

/*!
 * \brief Part of the \link Record::start start \endlink property.
 */
QDateTime Record::start() const { return m_start; }

/*!
 * \brief Part of the \link Record::start start \endlink property.
 */
void Record::setStart(const QDateTime &nStart)
{
    if (nStart != m_start) {
        m_start = nStart;
#ifdef QT_DEBUG
        qDebug() << "Changed start to" << m_start;
#endif
        emit startChanged(start());
    }
}




/*!
 * \property Record::end
 * \brief The end time of this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QDateTime</TD><TD>end() const</TD></TR><TR><TD>void</TD><TD>setEnd(const QDateTime &nEnd)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>endChanged(const QDateTime &end)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::endChanged(const QDateTime &end)
 * \brief Part of the \link Record::end end \endlink property.
 */

/*!
 * \brief Part of the \link Record::end end \endlink property.
 */
QDateTime Record::end() const { return m_end; }

/*!
 * \brief Part of the \link Record::end end \endlink property.
 */
void Record::setEnd(const QDateTime &nEnd)
{
    if (nEnd != m_end) {
        m_end = nEnd;
#ifdef QT_DEBUG
        qDebug() << "Changed end to" << m_end;
#endif

        setActive(m_end == QDateTime::fromTime_t(0));

        emit endChanged(end());
    }
}




/*!
 * \property Record::duration
 * \brief The duration this record was active.
 *
 * \par Access functions:
 * <TABLE><TR><TD>uint</TD><TD>duration() const</TD></TR><TR><TD>void</TD><TD>setDuration(uint nDuration)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>durationChanged(uint duration)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::durationChanged(uint duration)
 * \brief Part of the \link Record::duration duration \endlink property.
 */

/*!
 * \brief Part of the \link Record::duration duration \endlink property.
 */
uint Record::duration() const { return m_duration; }

/*!
 * \brief Part of the \link Record::duration duration \endlink property.
 */
void Record::setDuration(uint nDuration)
{
    if (nDuration != m_duration) {
        m_duration = nDuration;
#ifdef QT_DEBUG
        qDebug() << "Changed duration to" << m_duration;
#endif
        emit durationChanged(duration());
    }
}





/*!
 * \property Record::repetitions
 * \brief Repetitions saved for this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>uint</TD><TD>repetitions() const</TD></TR><TR><TD>void</TD><TD>setRepetitions(uint nRepetitions)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>repetitionsChanged(uint repetitions)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::repetitionsChanged(uint repetitions)
 * \brief Part of the \link Record::repetitions repetitions \endlink property.
 */

/*!
 * \brief Part of the \link Record::repetitions repetitions \endlink property.
 */
uint Record::repetitions() const { return m_repetitions; }

/*!
 * \brief Part of the \link Record::repetitions repetitions \endlink property.
 */
void Record::setRepetitions(uint nRepetitions)
{
    if (nRepetitions != m_repetitions) {
        m_repetitions = nRepetitions;
#ifdef QT_DEBUG
        qDebug() << "Changed repetitions to" << m_repetitions;
#endif
        emit repetitionsChanged(repetitions());
    }
}




/*!
 * \property Record::distance
 * \brief The distance saved for this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>double</TD><TD>distance() const</TD></TR><TR><TD>void</TD><TD>setDistance(double nDistance)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>distanceChanged(double distance)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::distanceChanged(double distance)
 * \brief Part of the \link Record::distance distance \endlink property.
 */

/*!
 * \brief Part of the \link Record::distance distance \endlink property.
 */
double Record::distance() const { return m_distance; }

/*!
 * \brief Part of the \link Record::distance distance \endlink property.
 */
void Record::setDistance(double nDistance)
{
    if (nDistance != m_distance) {
        m_distance = nDistance;
#ifdef QT_DEBUG
        qDebug() << "Changed distance to" << m_distance;
#endif
        emit distanceChanged(distance());
    }
}



/*!
 * \property Record::note
 * \brief User note for this record.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>note() const</TD></TR><TR><TD>void</TD><TD>setNote(const QString &nNote)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>noteChanged(const QString &note)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::noteChanged(const QString &note)
 * \brief Part of the \link Record::note note \endlink property.
 */

/*!
 * \brief Part of the \link Record::note note \endlink property.
 */
QString Record::note() const { return m_note; }

/*!
 * \brief Part of the \link Record::note note \endlink property.
 */
void Record::setNote(const QString &nNote)
{
    if (nNote != m_note) {
        m_note = nNote.trimmed();
#ifdef QT_DEBUG
        qDebug() << "Changed note to" << m_note;
#endif
        emit noteChanged(note());
    }
}




/*!
 * \property Record::active
 * \brief Returns true while this record is in active state.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>isActive() const</TD></TR><TR><TD>void</TD><TD>setActive(bool active)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>activeChanged(bool active)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::activeChanged(bool active)
 * \brief Part of the \link Record::active active \endlink property.
 */

/*!
 * \brief Part of the \link Record::active active \endlink property.
 */
bool Record::isActive() const { return m_active; }

/*!
 * \brief Part of the \link Record::active active \endlink property.
 */
void Record::setActive(bool active)
{
    if (active != m_active) {
        m_active = active;
#ifdef QT_DEBUG
        qDebug() << "Changed active to" << m_active;
#endif
        emit activeChanged(isActive());
    }
}




/*!
 * \property Record::tpr
 * \brief Time per repetition
 *
 * \par Access functions:
 * <TABLE><TR><TD>float</TD><TD>tpr() const</TD></TR><TR><TD>void</TD><TD>setTpr(float nTpr)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>tprChanged(float tpr)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::tprChanged(float tpr)
 * \brief Part of the \link Record::tpr tpr \endlink property.
 */

/*!
 * \brief Part of the \link Record::tpr tpr \endlink property.
 */
float Record::tpr() const { return m_tpr; }

/*!
 * \brief Part of the \link Record::tpr tpr \endlink property.
 */
void Record::setTpr(float nTpr)
{
    if (nTpr != m_tpr) {
        m_tpr = nTpr;
#ifdef QT_DEBUG
        qDebug() << "Changed tpr to" << m_tpr;
#endif
        emit tprChanged(tpr());
    }
}




/*!
 * \property Record::maxSpeed
 * \brief Maximum speed in meter per second.
 *
 * \par Access functions:
 * <TABLE><TR><TD>float</TD><TD>maxSpeed() const</TD></TR><TR><TD>void</TD><TD>setMaxSpeed(float nMaxSpeed)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>maxSpeedChanged(float maxSpeed)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::maxSpeedChanged(float maxSpeed)
 * \brief Part of the \link Record::maxSpeed maxSpeed \endlink property.
 */

/*!
 * \brief Part of the \link Record::maxSpeed maxSpeed \endlink property.
 */
float Record::maxSpeed() const { return m_maxSpeed; }

/*!
 * \brief Part of the \link Record::maxSpeed maxSpeed \endlink property.
 */
void Record::setMaxSpeed(float nMaxSpeed)
{
    if (nMaxSpeed != m_maxSpeed) {
        m_maxSpeed = nMaxSpeed;
#ifdef QT_DEBUG
        qDebug() << "Changed maxSpeed to" << m_maxSpeed;
#endif
        emit maxSpeedChanged(maxSpeed());
    }
}




/*!
 * \property Record::avgSpeed
 * \brief Average speed in meter per second.
 *
 * \par Access functions:
 * <TABLE><TR><TD>float</TD><TD>avgSpeed() const</TD></TR><TR><TD>void</TD><TD>setAvgSpeed(float nAvgSpeed)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>avgSpeedChanged(float avgSpeed)</TD></TR></TABLE>
 */

/*!
 * \fn void Record::avgSpeedChanged(float avgSpeed)
 * \brief Part of the \link Record::avgSpeed avgSpeed \endlink property.
 */

/*!
 * \brief Part of the \link Record::avgSpeed avgSpeed \endlink property.
 */
float Record::avgSpeed() const { return m_avgSpeed; }

/*!
 * \brief Part of the \link Record::avgSpeed avgSpeed \endlink property.
 */
void Record::setAvgSpeed(float nAvgSpeed)
{
    if (nAvgSpeed != m_avgSpeed) {
        m_avgSpeed = nAvgSpeed;
#ifdef QT_DEBUG
        qDebug() << "Changed avgSpeed to" << m_avgSpeed;
#endif
        emit avgSpeedChanged(avgSpeed());
    }
}



/*!
 * \brief Returns true if this is a valid record.
 *
 * Tests if the databaseId has a valid value and if a valid Activity is set.
 */
bool Record::isValid() const
{
    return (m_databaseId > 0 && activity() && activity()->isValid());
}


/*!
 * \brief Updates the duration and according the end time, the average speed and time per repetition (tpr).
 *
 * This should be used for finished recordings, it is not meant for active recordings.
 */
void Record::updateDuration(uint nDuration)
{
    if (m_duration != nDuration) {
        setDuration(nDuration);
        setEnd(m_start.addSecs(nDuration));
        if (m_repetitions > 0) {
            setTpr((float)m_duration/(float)m_repetitions);
        } else {
            setTpr(0.0);
        }

        if (m_distance > 0.0) {
            setAvgSpeed(m_distance/(double)m_duration);
        } else {
            setAvgSpeed(0.0);
        }
    }
}


/*!
 * \brief Updates the repetitions and according the time per repetition (tpr).
 */
void Record::updateRepetitions(uint nRepetitions)
{
    if (m_repetitions != nRepetitions) {
        setRepetitions(nRepetitions);
        if (m_repetitions > 0) {
            setTpr((float)m_duration/(float)m_repetitions);
        } else {
            setTpr(0.0);
        }
    }
}


/*!
 * \brief Updates the distance and according the average speed.
 */
void Record::updateDistance(double nDistance)
{
    if (m_distance != nDistance) {
        setDistance(nDistance);
        if (m_distance > 0.0) {
            setAvgSpeed(m_distance/(double)m_duration);
        } else {
            setAvgSpeed(0.0);
        }
    }
}


/*!
 * \brief Updates the activity the record belongs to.
 */
void Record::updateActivity(Activity *activity)
{
    if (m_activity && (m_activity->databaseId() != activity->databaseId())) {
        m_activity->setDatabaseId(activity->databaseId());
        m_activity->setName(activity->name());
        m_activity->setMinRepeats(activity->minRepeats());
        m_activity->setMaxRepeats(activity->maxRepeats());
        m_activity->setUseRepeats(activity->useRepeats());
        m_activity->setUseDistance(activity->useDistance());
        m_activity->setRecords(activity->records());

        Category *c = m_activity->category();
        Category *o = activity->category();

        if (c) {
            c->setDatabaseId(o->databaseId());
            c->setName(o->name());
            c->setColor(o->color());
            c->setActivities(o->activities());
        }

        if (!m_activity->useRepeats()) {
            setRepetitions(0);
        }

        if (!m_activity->useDistance()) {
            setDistance(0.0);
        }
    }
}


/*!
 * \brief Emits the startRemoving() signal.
 *
 * Does nothing more than emitting the startRemoving() signal.
 */
void Record::remove()
{
    emit startRemoving();
}



/*!
 * \brief Adds \c dist to current distance.
 */
void Record::addDistance(double dist)
{
    setDistance(distance() + dist);
}
