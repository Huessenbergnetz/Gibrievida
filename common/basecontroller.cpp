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

#include "basecontroller.h"
#include <QStandardPaths>
#include <QSqlQuery>
#include "globals.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new base controller.
 */
BaseController::BaseController(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::database();
}


/*!
 * \brief Destroys the base controller.
 */
BaseController::~BaseController()
{

}


/*!
 * \brief Establish database connection.
 *
 * Returns true if the database connection has been established successfull, otherwise returns false.
 */
bool BaseController::connectDb()
{
    if (m_db.isValid() && m_db.isOpen()) {
        return true;
    }

    if (!m_db.isValid()) {

        QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

        if (dirs.isEmpty()) {
            return false;
        }

        QString dbPath = dirs.first();
        dbPath.append(QLatin1String("/")).append(QLatin1String(APP_NAME)).append(QStringLiteral("/database.sqlite"));

        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        m_db.setDatabaseName(dbPath);

        if (m_db.open()) {
            QSqlQuery q(m_db);
            q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
            return true;
        } else {
            return false;
        }

    } else if (!m_db.isOpen()) {

        if (m_db.open()) {
            QSqlQuery q(m_db);
            q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
            return true;
        } else {
            return false;
        }

    }

    return false;
}
