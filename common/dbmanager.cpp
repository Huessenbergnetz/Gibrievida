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
#include "globals.h"

#include <QVariant>
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

    QString dbpath = dirs.first();
    dbpath.append(QLatin1String("/")).append(QLatin1String(APP_NAME));

    QDir dbdir(dbpath);
    if (!dbdir.exists()) {
        dbdir.mkpath(dbpath);
    }

    dbpath.append(QStringLiteral("/database.sqlite"));

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_db.setDatabaseName(dbpath);

    if (!m_db.open()) {
        fatalError("Failed to open database", m_db.lastError());
    }

    if (!createDatabase()) {
        return;
    }

    if (!updateDatabase()) {
        return;
    }
}


/*!
 * \brief Creates the database tables.
 */
bool DBManager::createDatabase()
{
    qDebug("Checking basic database schema");

    QSqlQuery q(m_db);

    if (!m_db.transaction()) {
        fatalError("Failed to create database transaction", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS categories "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "name TEXT NOT NULL, "
                               "color TEXT NOT NULL)"
                               ))) {
        fatalError("Failed to create table categories", q.lastError());
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
        fatalError("Failed to create table activities", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_activities_category ON activities (category)"))) {
        fatalError("Failed to create index", q.lastError());
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
        fatalError("Failed to create table records", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_records_activity ON records (activity)"))) {
        fatalError("Failed to create index", q.lastError());
        return false;
    }

    if (!m_db.commit()) {
        fatalError("Failed to execute database transaction", q.lastError());
        return false;
    }

    return true;
}


bool DBManager::updateDatabase()
{
    int db_schema_version = 0;

    QSqlQuery q(m_db);

    if (q.exec(QStringLiteral("SELECT value FROM system WHERE key = 'schema_version'"))) {
        if (q.next()) {
            db_schema_version = q.value(0).toInt();
        }
    }

    if (db_schema_version < DB_SCHEMA_VERSION) {
        return updateToSchemaV2();
    }

    if (db_schema_version < DB_SCHEMA_VERSION) {

        qDebug("Updating database schema version");

        if (!q.prepare(QStringLiteral("UPDATE system SET value = ? WHERE key = 'schema_version'"))) {
            fatalError("Failed to prepare database query", q.lastError());
        }

        q.addBindValue(DB_SCHEMA_VERSION);

        if (!q.exec()) {
            fatalError("Failed to execute database query", q.lastError());
        }

    }

    return true;
}




bool DBManager::updateToSchemaV2()
{
    qDebug("Update database to schema version 2");

    QSqlQuery q(m_db);

    if (!m_db.transaction()) {
        fatalError("Failed to create database transaction", m_db.lastError());
        return false;
    }

    if(!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS system "
                              "(id INTEGER PRIMARY KEY NOT NULL, "
                              "key TEXT NOT NULL, "
                              "value TEXT NOT NULL)"
                              ))) {
        fatalError("Failed to create system table", q.lastError());
        return false;
    }

    if (!q.prepare(QStringLiteral("INSERT INTO system (key, value) VALUES (?, ?)"))) {
        fatalError("Failed to prepare database query", q.lastError());
        return false;
    }

    q.addBindValue(QStringLiteral("schema_version"));
    q.addBindValue(QStringLiteral("2"));

    if (!q.exec()) {
        fatalError("Failed to execute database query", q.lastError());
        return false;
    }

    if (!q.exec(QStringLiteral("ALTER TABLE activities ADD COLUMN sensor INTEGER DEFAULT 0"))) {
        fatalError("Failed to add column sensor to table activities", q.lastError());
        return false;
    }

    if (!m_db.commit()) {
        fatalError("Failed to commit database transaction", m_db.lastError());
        return false;
    }

    return true;
}



void DBManager::fatalError(const char *message, const QSqlError &error)
{
    qFatal("%s: %s", message, error.text().toLocal8Bit().constData());
}
