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

#include "helpers.h"
#include <QtCore/qmath.h>
#include <QDateTime>

using namespace Gibrievida;

/*!
 * \brief Constructs a new helpers class object.
 */
Helpers::Helpers(QObject *parent) : QObject(parent)
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
    if (duration <= 0) {
        return tr("invalid");
    }

    if (duration > 0 && duration < 60) {
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
        return local.toString(tr("hh:mmap"));
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
