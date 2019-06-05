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

#include "helpers.h"
#include <QtCore/qmath.h>
#include <QDateTime>
#include "configuration.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new helpers class object.
 *
 * A pointer to a Configuration object has to be set in order to calculate values based
 * on the currently set measurement system.
 */
Helpers::Helpers(Configuration *config, const QLocale &locale, QObject *parent) : QObject(parent), m_c(config), m_l(locale)
{

}


/*!
 * \brief Destroys the helpers class object.
 */
Helpers::~Helpers()
{

}



/*!
 * \brief Creates a human readable string from a duration in seconds.
 */
QString Helpers::createDurationString(uint duration)
{

    if (duration < 60) {
        //: the s is the abbreviation for second(s)
        return tr("%1s").arg(duration);
    }

    uint secs = duration;

    float fDays = secs/86400;
    uint days = qFloor(fDays);

    secs = secs - (86400*days);

    float fHours = secs/3600;
    uint hours = qFloor(fHours);

    secs = secs - (3600*hours);

    float fMins = secs/60;
    int mins = qFloor(fMins);

    secs = secs - (60*mins);

    QString durString;

    if (days) {
        //: d is the abbreviation for day, used in duration time display.
        durString.append(tr("%1d").arg(days)).append(QLatin1String(" "));
    }

    if (hours) {
        //: h is the abbreviation for hour, used in duration time display
        durString.append(tr("%1h").arg(hours)).append(QLatin1String(" "));
    }

    if (mins) {
        //: m is the abbreviation for minute. Used in duration time display.
        durString.append(tr("%1m").arg(mins)).append(QLatin1String(" "));
    }

    if (secs) {
        //: s is the abbreviation for second. Used in duration time display.
        durString.append(tr("%1s").arg(secs));
    }

    return durString;
}


/*!
 * \brief Creates a human readable relative time string.
 */
QString Helpers::relativeTimeString(const QDateTime &time)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime local = time.toLocalTime();
    qint64 days = local.daysTo(now);

    switch (days) {
    case 0:
        //: date and time foramt, see http://doc.qt.io/qt-5/qdatetime.html#toString
        return tr("Today, %1").arg(local.toString(tr("hh:mmap")));
    case 1:
        //: date and time foramt, see http://doc.qt.io/qt-5/qdatetime.html#toString
        return tr("Yesterday, %1").arg(local.toString(tr("hh:mmap")));
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        //: date and time foramt, see http://doc.qt.io/qt-5/qdatetime.html#toString
        return local.toString(tr("dddd, hh:mmap"));
    default:
        //: date and time foramt, see http://doc.qt.io/qt-5/qdatetime.html#toString
        return local.toString(tr("dd.MM.yy hh:mmap"));
    }

}



/*!
 * \brief Converts a numeric value into a distance string.
 *
 * Based on the configured measurement system it will be returned as yards/miles or metres/kilo metres.
 */
QString Helpers::toDistanceString(double distance)
{
    if (m_c->distanceMeasurement() == QLocale::MetricSystem) {

        if (distance > 999.99999999) {
            //: km is the abbreviation for kilo metres
            return tr("%1 km").arg(m_l.toString(distance/1000.0, 'f', 2));
        } else {
            //: m is the abbreviation for metres
            return tr("%1 m").arg(m_l.toString(distance, 'f', 2));
        }

    } else {

        double yards = toYards(distance);

        if (yards > 1759.999999999) {
            //: mi is the abbreviation for mile
            return tr("%1 mi").arg(m_l.toString(yards/1760.0, 'f', 2));
        } else {
            //: yd is the abbreviation for yard
            return tr("%1 yd").arg(m_l.toString(yards, 'f', 2));
        }

    }
}


/*!
 * \brief Converts a numeric value into a speed string.
 *
 * Based on the configured measurement system, it will be returned as mph or km/h.
 */
QString Helpers::toSpeedString(float speed)
{
    if (m_c->distanceMeasurement() == QLocale::MetricSystem ) {
        return tr("%1 km/h").arg(m_l.toString(speed*3.6, 'f', 2));
    } else {
        return tr("%1 mph").arg(m_l.toString(speed/0.44704, 'f', 2));
    }
}


/*!
 * \brief Converts yards to metres.
 *
 * This is based on the currently configured measurement system.
 */
double Helpers::toMetres(double yards)
{
    if (m_c->distanceMeasurement() == QLocale::ImperialUSSystem) {
        return yards * 0.914401829;
    } else {
        return yards * 0.9143993;
    }
}


/*!
 * \brief Converts metres to yards.
 *
 * This is based on the currently configured measurement system.
 */
double Helpers::toYards(double metres)
{
    if (m_c->distanceMeasurement() == QLocale::ImperialUSSystem) {
        return metres / 0.914401829;
    } else {
        return metres / 0.9143993;
    }
}
