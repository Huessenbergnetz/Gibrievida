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

#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>

namespace Gibrievida {

class Helpers : public QObject
{
    Q_OBJECT
public:
    explicit Helpers(QObject *parent = nullptr);
    ~Helpers();

    Q_INVOKABLE QString createDurationString(uint duration);
    Q_INVOKABLE QString relativeTimeString(const QDateTime &time);

private:
    Q_DISABLE_COPY(Helpers)
};

}

#endif // HELPERS_H
