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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>
#include <QLocale>

namespace Gibrievida {

/*!
 * \brief Configuration interface class.
 *
 * Makes the configuration available to the QML part.
 */
class Configuration : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QLocale::MeasurementSystem distanceMeasurement READ distanceMeasurement WRITE setDistanceMeasurement NOTIFY distanceMeasurementChanged)
    Q_PROPERTY(int repetitionClickSound READ repetitionClickSound WRITE setRepetitionClickSound NOTIFY repetitionClickSoundChanged)
    Q_ENUMS(QLocale::MeasurementSystem)
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration();

    QLocale::MeasurementSystem distanceMeasurement() const;
    void setDistanceMeasurement(QLocale::MeasurementSystem nDistanceMeasurement);

    int repetitionClickSound() const;
    void setRepetitionClickSound(int nRepetitionClickSound);

signals:
    void distanceMeasurementChanged(QLocale::MeasurementSystem distanceMeasurement);
    void repetitionClickSoundChanged(int repetitionClickSound);

private:
    Q_DISABLE_COPY(Configuration)
    QLocale::MeasurementSystem m_distanceMeasurement;
    int m_repetitionClickSound;
};

}

#endif // CONFIGURATION_H
