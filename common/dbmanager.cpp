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

#include "dbmanager.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QDir>

#include "globals.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new database manager.
 */
DBManager::DBManager(QObject *parent) : QThread(parent)
{

}

/*!
 * \brief Destroys the database manager.
 */
DBManager::~DBManager()
{

}


/*!
 * \brief The starting point for the thread.
 *
 * This will call createDatabase().
 */
void DBManager::run()
{
    QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

    if (dirs.isEmpty()) {
        return;
    }

    m_dbpath = dirs.first();
    m_dbpath.append(QLatin1String("/")).append(QLatin1String(APP_NAME));

    QDir dbdir(m_dbpath);
    if (!dbdir.exists()) {
        dbdir.mkpath(m_dbpath);
    }

    m_dbpath.append(QStringLiteral("/database.sqlite"));

    if (!createDatabase()) {
        return;
    }
}


/*!
 * \brief Creates the database tables.
 */
bool DBManager::createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    db.setDatabaseName(m_dbpath);

    if (!db.open()) {
        return false;
    }

    QSqlQuery q(db);

    if (!db.transaction()) {
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS categories "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "name TEXT NOT NULL, "
                               "color TEXT NOT NULL)"
                               ))) {
#ifdef QT_DEBUG
        qDebug() << q.lastError().text();
#endif
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS activities "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "category INTEGER NOT NULL, "
                               "name TEXT NOT NULL, "
                               "minrepeats INTEGER NOT NULL, "
                               "maxrepeats INTEGER NOT NULL, "
                               "distance INTEGER NOT NULL, "
                               "note TEXT, "
                               "FOREIGN KEY(category) REFERENCES categories(id) ON DELETE CASCADE)"
                               ))) {
#ifdef QT_DEBUG
        qDebug() << q.lastError().text();
#endif
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_activities_category ON activities (category)"))) {
#ifdef QT_DEBUG
        qDebug() << q.lastError().text();
#endif
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS records "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "activity INTEGER NOT NULL, "
                               "start INTEGER NOT NULL, "
                               "end INTEGER NOT NULL DEFAULT 0, "
                               "duration INTEGER NOT NULL DEFAULT 0, "
                               "repetitions INTEGER DEFAULT 0, "
                               "distance REAL DEFAULT 0.0, "
                               "note TEXT, "
                               "tpr REAL DEFAULT 0.0, "
                               "maxSpeed REAL DEFAULT 0.0, "
                               "avgSpeed REAL DEFAULT 0.0, "
                               "FOREIGN KEY(activity) REFERENCES activities(id) ON DELETE CASCADE)"
                               ))) {
#ifdef QT_DEBUG
        qDebug() << q.lastError().text();
#endif
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_records_activity ON records (activity)"))) {
#ifdef QT_DEBUG
        qDebug() << q.lastError().text();
#endif
        return false;
    }

    if (!db.commit()) {
        return false;
    }

    return true;
}
