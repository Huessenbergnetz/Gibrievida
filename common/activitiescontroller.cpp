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

#include "activitiescontroller.h"
#include "category.h"
#include "activity.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new activities controller.
 */
ActivitiesController::ActivitiesController(QObject *parent) : BaseController(parent)
{

}

/*!
 * \brief Destroys the activity controller.
 */
ActivitiesController::~ActivitiesController()
{

}


/*!
 * \brief Adds a new activity to the database.
 *
 * Emits the added() signal and returns the database id > -1 if successful.
 */
int ActivitiesController::add(const QString &name, Category *c, int minRepeats, int maxRepeats, bool useDistance, int sensorType, int sensorDelay)
{
    if (!connectDb()) {
        return -1;
    }

    Category *cat = new Category(c);

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO activities (name, category, minrepeats, maxrepeats, distance, sensor, sensorDelay) VALUES (?, ?, ?, ?, ?, ?, ?)"))) {
        delete cat;
        return -1;
    }

    q.addBindValue(name);
    q.addBindValue(cat->databaseId());
    q.addBindValue(minRepeats);
    q.addBindValue(maxRepeats);
    q.addBindValue(useDistance);
    q.addBindValue(sensorType);
    q.addBindValue(sensorDelay);

    if (!q.exec()) {
        delete cat;
        return -1;
    }

    int id = q.lastInsertId().toInt();

    emit added(id, name, cat, minRepeats, maxRepeats, useDistance, sensorType, sensorDelay);

    return id;

}



/*!
 * \brief Changes the activity with \a databaseId and updates the table data.
 *
 * Emits the edited() signal and returns true on success.
 */
bool ActivitiesController::update(Activity *a, int oldCategoryId)
{
    if (!a->isValid()) {
        qCritical("Activity is not valid. Returning.");
        return false;
    }

    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE activities SET name = ?, category = ?, minRepeats = ?, maxRepeats = ?, distance = ?, sensor = ?, sensorDelay = ? WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(a->name());
    q.addBindValue(a->category()->databaseId());
    q.addBindValue(a->minRepeats());
    q.addBindValue(a->maxRepeats());
    q.addBindValue(a->useDistance());
    q.addBindValue(a->sensorType());
    q.addBindValue(a->sensorDelay());
    q.addBindValue(a->databaseId());

    if (!q.exec()) {
        return false;
    }

    emit updated(a, oldCategoryId);

    return true;
}


/*!
 * \brief Removes the Activity \c a and all associated records.
 *
 * Emits the removed() signal and return true on success.
 */
bool ActivitiesController::remove(Activity *a)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM activities WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(a->databaseId());

    if (!q.exec()) {
        return false;
    }

    emit removed(a->databaseId(), a->category()->databaseId());

    return true;
}



/*!
 * \brief Removes all activities from the database.
 *
 * Emits the removedAll() signal and returns true on success.
 */
bool ActivitiesController::removeAll()
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("DELETE FROM activities"))) {
        return false;
    }

    emit removedAll();

    return true;
}
