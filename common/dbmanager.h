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

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>

class QSqlError;

namespace Gibrievida {

/*!
 * \brief Manages the initialization of the local SQLite database.
 */
class DBManager : public QThread
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool createDatabase();
    bool updateDatabase();
    bool updateToSchemaV2();

    void fatalError(const char *message, const QSqlError &error);
    QSqlDatabase m_db;

    Q_DISABLE_COPY(DBManager)
};

}

#endif // DBMANAGER_H
