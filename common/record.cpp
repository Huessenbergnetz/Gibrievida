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

#include "record.h"
#include "activity.h"
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
    m_databaseId = 0;
    m_activity = nullptr;
    m_duration = 0;
    m_repetitions = 0;
    m_distance = 0.0;
}


/*!
 * \override
 */
Record::Record(int databaseId, const QDateTime &start, const QDateTime &end, uint duration, uint repetitions, double distance, const QString &note, QObject *parent) :
    QObject(parent), m_databaseId(databaseId), m_start(start), m_end(end), m_duration(duration), m_repetitions(repetitions), m_distance(distance), m_note(note)
{

}


/*!
 * \brief Deconstructs the Record object.
 */
Record::~Record()
{
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
        m_note = nNote;
#ifdef QT_DEBUG
        qDebug() << "Changed note to" << m_note;
#endif
        emit noteChanged(note());
    }
}



/*!
 * \brief Returns true if this is a valid record.
 */
bool Record::isValid() const
{
    return (m_databaseId > 0 && activity() && activity()->isValid());
}
