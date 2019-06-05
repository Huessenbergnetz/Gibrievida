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

#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QSqlDatabase>

namespace Gibrievida {

/*!
 * \brief Base controller class.
 *
 * Other conroller classes should be subclasses of this class. The BaseController provides
 * methods to connect to the database.
 */
class BaseController : public QObject
{
    Q_OBJECT
public:
    explicit BaseController(QObject *parent = nullptr);
    ~BaseController();

protected:
    bool connectDb();
    QSqlDatabase m_db;  /*!< Contains the default database. */

private:
    Q_DISABLE_COPY(BaseController)
};

}

#endif // BASECONTROLLER_H
