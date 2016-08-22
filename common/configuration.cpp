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
    m_repetitionClickSound = value(QStringLiteral("repetitionClickSound"), 0).toInt();
    m_language = value(QStringLiteral("language"), QStringLiteral("")).toString();
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



/*!
 * \property Configuration::repetitionClickSound
 * \brief Index of the sound effect file to play if the repetition increase button is clicked.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>repetitionClickSound() const</TD></TR><TR><TD>void</TD><TD>setRepetitionClickSound(int nRepetitionClickSound)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>repetitionClickSoundChanged(int repetitionClickSound)</TD></TR></TABLE>
 */

/*!
 * \fn void Configuration::repetitionClickSoundChanged(int repetitionClickSound)
 * \brief Part of the \link Configuration::repetitionClickSound repetitionClickSound \endlink property.
 */

/*!
 * \brief Part of the \link Configuration::repetitionClickSound repetitionClickSound \endlink property.
 */
int Configuration::repetitionClickSound() const { return m_repetitionClickSound; }

/*!
 * \brief Part of the \link Configuration::repetitionClickSound repetitionClickSound \endlink property.
 */
void Configuration::setRepetitionClickSound(int nRepetitionClickSound)
{
    if (nRepetitionClickSound != m_repetitionClickSound) {
        m_repetitionClickSound = nRepetitionClickSound;
#ifdef QT_DEBUG
        qDebug() << "Changed repetitionClickSound to" << m_repetitionClickSound;
#endif
        setValue(QStringLiteral("repetitionClickSound"), m_repetitionClickSound);

        emit repetitionClickSoundChanged(repetitionClickSound());
    }
}


/*!
 * \property Configuration::language
 * \brief The currently selected language.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>language() const</TD></TR><TR><TD>void</TD><TD>setLanguage(const QString &nLanguage)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>languageChanged(const QString &language)</TD></TR></TABLE>
 */

/*!
 * \fn void Configuration::languageChanged(const QString &language)
 * \brief Part of the \link Configuration::language language \endlink property.
 */

/*!
 * \brief Part of the \link Configuration::language language \endlink property.
 */
QString Configuration::language() const { return m_language; }

/*!
 * \brief Part of the \link Configuration::language language \endlink property.
 */
void Configuration::setLanguage(const QString &nLanguage)
{
    if (nLanguage != m_language) {
        m_language = nLanguage;
#ifdef QT_DEBUG
        qDebug() << "Changed language to" << m_language;
#endif
        setValue(QStringLiteral("language"), m_language);

        emit languageChanged(language());
    }
}

