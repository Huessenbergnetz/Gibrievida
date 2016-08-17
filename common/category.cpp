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

#include "category.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new Category object.
 */
Category::Category(QObject *parent) :
    QObject(parent)
{
    m_databaseId = -1;
    m_activities = 0;

#ifdef QT_DEBUG
    qDebug() << "Constructed a new empty" << this;
#endif
}


/*!
 * \overload
 */
Category::Category(int databaseId, const QString &name, const QString &color, int activities, QObject *parent) :
    QObject(parent), m_databaseId(databaseId), m_name(name), m_color(color), m_activities(activities)
{
#ifdef QT_DEBUG
    qDebug() << "Constructed a new" << this << "ID:" << databaseId << "Name:" << name << "Color:" << color << "Activities:" << activities;
#endif
}


/*!
 * \overload
 */
Category::Category(Category *other, QObject *parent) :
    QObject(parent), m_databaseId(other->databaseId()), m_name(other->name()), m_color(other->color()), m_activities(other->activities())
{
#ifdef QT_DEBUG
    qDebug() << "Constructed new" << this << "from" << other;
#endif
}


/*!
 * \brief Deconstructs the Category object.
 */
Category::~Category()
{
#ifdef QT_DEBUG
    qDebug() << "Destroying" << this;
#endif
}


/*!
 * \property Category::databaseId
 * \brief Database ID of this category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>databaseId() const</TD></TR><TR><TD>void</TD><TD>setDatabaseId(int nDatabaseId)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>databaseIdChanged(int databaseId)</TD></TR></TABLE>
 */

/*!
 * \fn void Category::databaseIdChanged(int databaseId)
 * \brief Part of the \link Category::databaseId databaseId \endlink property.
 */

/*!
 * \brief Part of the \link Category::databaseId databaseId \endlink property.
 */
int Category::databaseId() const { return m_databaseId; }

/*!
 * \brief Part of the \link Category::databaseId databaseId \endlink property.
 */
void Category::setDatabaseId(int nDatabaseId)
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
 * \property Category::name
 * \brief Name of this category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>name() const</TD></TR><TR><TD>void</TD><TD>setName(const QString &nName)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>nameChanged(const QString &name)</TD></TR></TABLE>
 */

/*!
 * \fn void Category::nameChanged(const QString &name)
 * \brief Part of the \link Category::name name \endlink property.
 */

/*!
 * \brief Part of the \link Category::name name \endlink property.
 */
QString Category::name() const { return m_name; }

/*!
 * \brief Part of the \link Category::name name \endlink property.
 */
void Category::setName(const QString &nName)
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
 * \property Category::color
 * \brief The web hex color value for this category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>color() const</TD></TR><TR><TD>void</TD><TD>setColor(const QString &nColor)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>colorChanged(const QString &color)</TD></TR></TABLE>
 */

/*!
 * \fn void Category::colorChanged(const QString &color)
 * \brief Part of the \link Category::color color \endlink property.
 */

/*!
 * \brief Part of the \link Category::color color \endlink property.
 */
QString Category::color() const { return m_color; }

/*!
 * \brief Part of the \link Category::color color \endlink property.
 */
void Category::setColor(const QString &nColor)
{
    if (nColor != m_color) {
        m_color = nColor;
#ifdef QT_DEBUG
        qDebug() << "Changed color to" << m_color;
#endif
        emit colorChanged(color());
    }
}




/*!
 * \property Category::activities
 * \brief Number of activities associated with this category.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>activities() const</TD></TR><TR><TD>void</TD><TD>setActivities(int nActivities)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>activitiesChanged(int activities)</TD></TR></TABLE>
 */

/*!
 * \fn void Category::activitiesChanged(int activities)
 * \brief Part of the \link Category::activities activities \endlink property.
 */

/*!
 * \brief Part of the \link Category::activities activities \endlink property.
 */
int Category::activities() const { return m_activities; }

/*!
 * \brief Part of the \link Category::activities activities \endlink property.
 */
void Category::setActivities(int nActivities)
{
    if (nActivities != m_activities) {
        m_activities = nActivities;
#ifdef QT_DEBUG
        qDebug() << "Changed activities to" << m_activities;
#endif
        emit activitiesChanged(activities());
    }
}



/*!
 * \brief Returns true if this is a valid category.
 *
 * This only checks for a valid database ID.
 */
bool Category::isValid() const
{
    return (m_databaseId > 0);
}


/*!
 * \brief Increases the count of activities in this category by one.
 */
void Category::increaseActivities()
{
    setActivities(m_activities + 1);
}


/*!
 * \brief Decreases the count of activities in this category by one.
 */
void Category::decreaseActivities()
{
    setActivities(m_activities - 1);
}
