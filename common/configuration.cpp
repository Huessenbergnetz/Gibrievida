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

#include "configuration.h"
#include <QDebug>

using namespace Gibrievida;

/*!
 * \brief Constructs a new configuration object.
 */
Configuration::Configuration(QObject *parent) : QSettings(parent)
{
    m_distanceMeasurement = (QLocale::MeasurementSystem)value(QStringLiteral("distanceMeasurement"), 0).toInt();
}


/*!
 * \brief Destroys the configuration object.
 */
Configuration::~Configuration()
{

}


/*!
 * \property Configuration::distanceMeasurement
 * \brief The measurement system for distances.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QLocale::MeasurementSystem</TD><TD>distanceMeasurement() const</TD></TR><TR><TD>void</TD><TD>setDistanceMeasurement(QLocale::MeasurementSystem nDistanceMeasurement)</TD></TR></TABLE>
 */

/*!
 * \fn void Configuration::distanceMeasurementChanged(QLocale::MeasurementSystem distanceMeasurement)
 * \brief Part of the \link Configuration::distanceMeasurement distanceMeasurement \endlink property.
 */

/*!
 * \brief Part of the \link Configuration::distanceMeasurement distanceMeasurement \endlink property.
 */
QLocale::MeasurementSystem Configuration::distanceMeasurement() const { return m_distanceMeasurement; }

/*!
 * \brief Part of the \link Configuration::distanceMeasurement distanceMeasurement \endlink property.
 */
void Configuration::setDistanceMeasurement(QLocale::MeasurementSystem nDistanceMeasurement)
{
    if (m_distanceMeasurement != nDistanceMeasurement) {
        m_distanceMeasurement = nDistanceMeasurement;
#ifdef QT_DEBUG
        qDebug() << " Set distanceMeasurement to" << m_distanceMeasurement;
#endif
        setValue(QStringLiteral("distanceMeasurement"), (int)m_distanceMeasurement);

        emit distanceMeasurementChanged(distanceMeasurement());
    }
}
