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

#include "activity.h"
#include "category.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new Activity object.
 */
Activity::Activity(QObject *parent) :
    QObject(parent)
{
    m_databaseId = -1;
    m_minRepeats = 0;
    m_maxRepeats = 0;
    m_useRepeats = false;
    m_useDistance = false;
    m_records = 0;
    m_category = nullptr;

#ifdef QT_DEBUG
    qDebug() << "Constructed a new empty" << this;
#endif
}

/*!
 * \overload
 */
Activity::Activity(int databaseId, const QString &name, int minRepeats, int maxRepeats, bool useDistance, int records, QObject *parent) :
    QObject(parent), m_databaseId(databaseId), m_name(name), m_minRepeats(minRepeats), m_maxRepeats(maxRepeats), m_useDistance(useDistance), m_records(records)
{
    m_useRepeats = (minRepeats > 0 && maxRepeats > 0);

#ifdef QT_DEBUG
    qDebug() << "Constructed a new" << this << "ID:" << databaseId << "Name:" << name << "MinRepeats:" << minRepeats << "MaxRepeats:" << maxRepeats << "Use Repeats:" << m_useRepeats << "Use Distance:" << useDistance << "Records:" << records;
#endif
}


/*!
 * \brief Constructs a new Activity from pointer to \c other Activity.
 *
 * This will create a deep copy. Copying every member data from the \c other Activity to the new Activity.
 */
Activity::Activity(Activity *other, QObject *parent) :
    QObject(parent), m_databaseId(other->databaseId()), m_name(other->name()), m_minRepeats(other->minRepeats()), m_maxRepeats(other->maxRepeats()), m_useDistance(other->useDistance()), m_records(other->records())
{
    m_useRepeats = (other->minRepeats() > 0 && other->maxRepeats() > 0);

#ifdef QT_DEBUG
    qDebug() << "Constructed a new" << this << "from" << other;
#endif

    Category *c = new Category(other->category(), this);
    setCategory(c);
}



/*!
 * \brief Deconstructs the Activity object.
 */
Activity::~Activity()
{
#ifdef QT_DEBUG
    qDebug() << "Destroying" << this;
#endif
}


/*!
 * \property Activity::databaseId
 * \brief The database ID of this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>databaseId() const</TD></TR><TR><TD>void</TD><TD>setDatabaseId(int nDatabaseId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>databaseIdChanged(int databaseId)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::databaseIdChanged(int databaseId)
 * \brief Part of the \link Activity::databaseId databaseId \endlink property.
 */

/*!
 * \brief Part of the \link Activity::databaseId databaseId \endlink property.
 */
int Activity::databaseId() const { return m_databaseId; }

/*!
 * \brief Part of the \link Activity::databaseId databaseId \endlink property.
 */
void Activity::setDatabaseId(int nDatabaseId)
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
 * \property Activity::name
 * \brief Name of this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>name() const</TD></TR><TR><TD>void</TD><TD>setName(const QString &nName)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>nameChanged(const QString &name)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::nameChanged(const QString &name)
 * \brief Part of the \link Activity::name name \endlink property.
 */

/*!
 * \brief Part of the \link Activity::name name \endlink property.
 */
QString Activity::name() const { return m_name; }

/*!
 * \brief Part of the \link Activity::name name \endlink property.
 */
void Activity::setName(const QString &nName)
{
    if (nName != m_name) {
        m_name = nName;
#ifdef QT_DEBUG
        qDebug() << "Changed name to" << m_name;
#endif
        emit nameChanged(name());
    }
}




/*!
 * \property Activity::minRepeats
 * \brief Minimum repeats for this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>minRepeats() const</TD></TR><TR><TD>void</TD><TD>setMinRepeats(int nMinRepeats)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>minRepeatsChanged(int minRepeats)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::minRepeatsChanged(int minRepeats)
 * \brief Part of the \link Activity::minRepeats minRepeats \endlink property.
 */

/*!
 * \brief Part of the \link Activity::minRepeats minRepeats \endlink property.
 */
int Activity::minRepeats() const { return m_minRepeats; }

/*!
 * \brief Part of the \link Activity::minRepeats minRepeats \endlink property.
 */
void Activity::setMinRepeats(int nMinRepeats)
{
    if (nMinRepeats != m_minRepeats) {
        m_minRepeats = nMinRepeats;
#ifdef QT_DEBUG
        qDebug() << "Changed minRepeats to" << m_minRepeats;
#endif
        emit minRepeatsChanged(minRepeats());
    }
}




/*!
 * \property Activity::maxRepeats
 * \brief Maximum repeats for this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>maxRepeats() const</TD></TR><TR><TD>void</TD><TD>setMaxRepeats(int nMaxRepeats)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>maxRepeatsChanged(int maxRepeats)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::maxRepeatsChanged(int maxRepeats)
 * \brief Part of the \link Activity::maxRepeats maxRepeats \endlink property.
 */

/*!
 * \brief Part of the \link Activity::maxRepeats maxRepeats \endlink property.
 */
int Activity::maxRepeats() const { return m_maxRepeats; }

/*!
 * \brief Part of the \link Activity::maxRepeats maxRepeats \endlink property.
 */
void Activity::setMaxRepeats(int nMaxRepeats)
{
    if (nMaxRepeats != m_maxRepeats) {
        m_maxRepeats = nMaxRepeats;
#ifdef QT_DEBUG
        qDebug() << "Changed maxRepeats to" << m_maxRepeats;
#endif
        emit maxRepeatsChanged(maxRepeats());
    }
}




/*!
 * \property Activity::useRepeats
 * \brief Returns true if repeats are used.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>useRepeats() const</TD></TR><TR><TD>void</TD><TD>setUseRepeats(bool nUseRepeats)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>useRepeatsChanged(bool useRepeats)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::useRepeatsChanged(bool useRepeats)
 * \brief Part of the \link Activity::useRepeats useRepeats \endlink property.
 */

/*!
 * \brief Part of the \link Activity::useRepeats useRepeats \endlink property.
 */
bool Activity::useRepeats() const { return m_useRepeats; }

/*!
 * \brief Part of the \link Activity::useRepeats useRepeats \endlink property.
 */
void Activity::setUseRepeats(bool nUseRepeats)
{
    if (nUseRepeats != m_useRepeats) {
        m_useRepeats = nUseRepeats;
#ifdef QT_DEBUG
        qDebug() << "Changed useRepeats to" << m_useRepeats;
#endif
        emit useRepeatsChanged(useRepeats());
    }
}




/*!
 * \property Activity::useDistance
 * \brief Returns true if distance should be recorded for this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>useDistance() const</TD></TR><TR><TD>void</TD><TD>setUseDistance(bool nUseDistance)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>useDistanceChanged(bool useDistance)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::useDistanceChanged(bool useDistance)
 * \brief Part of the \link Activity::useDistance useDistance \endlink property.
 */

/*!
 * \brief Part of the \link Activity::useDistance useDistance \endlink property.
 */
bool Activity::useDistance() const { return m_useDistance; }

/*!
 * \brief Part of the \link Activity::useDistance useDistance \endlink property.
 */
void Activity::setUseDistance(bool nUseDistance)
{
    if (nUseDistance != m_useDistance) {
        m_useDistance = nUseDistance;
#ifdef QT_DEBUG
        qDebug() << "Changed useDistance to" << m_useDistance;
#endif
        emit useDistanceChanged(useDistance());
    }
}




/*!
 * \property Activity::records
 * \brief Number of records associated with this activity.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>records() const</TD></TR><TR><TD>void</TD><TD>setRecords(int nRecords)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>recordsChanged(int records)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::recordsChanged(int records)
 * \brief Part of the \link Activity::records records \endlink property.
 */

/*!
 * \brief Part of the \link Activity::records records \endlink property.
 */
int Activity::records() const { return m_records; }

/*!
 * \brief Part of the \link Activity::records records \endlink property.
 */
void Activity::setRecords(int nRecords)
{
    if (nRecords != m_records) {
        m_records = nRecords;
#ifdef QT_DEBUG
        qDebug() << "Changed records to" << m_records;
#endif
        emit recordsChanged(records());
    }
}




/*!
 * \property Activity::category
 * \brief Category this activity belongs to.
 *
 * \par Access functions:
 * <TABLE><TR><TD>Category*</TD><TD>category() const</TD></TR><TR><TD>void</TD><TD>setCategory(Category *nCategory)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>categoryChanged(Category *category)</TD></TR></TABLE>
 */

/*!
 * \fn void Activity::categoryChanged(Category *category)
 * \brief Part of the \link Activity::category category \endlink property.
 */

/*!
 * \brief Part of the \link Activity::category category \endlink property.
 */
Category *Activity::category() const { return m_category; }

/*!
 * \brief Part of the \link Activity::category category \endlink property.
 */
void Activity::setCategory(Category *nCategory)
{
    if (nCategory != m_category) {
        m_category = nCategory;
#ifdef QT_DEBUG
        qDebug() << "Changed category to" << m_category;
#endif
        emit categoryChanged(category());
    }
}



/*!
 * \brief Returns true if this is a valid Activity object.
 *
 * This will test if there is a valid database ID and a valid category set.
 */
bool Activity::isValid() const
{
    return (m_databaseId > 0 && m_category && m_category->isValid());
}


