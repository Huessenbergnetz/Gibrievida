#include "activitiescontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

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
 * Emits the added() signal and return the database id > -1 if successful.
 */
int ActivitiesController::add(const QString &name, int category, int minRepeats, int maxRepeats, bool distance)
{
    if (!connectDb()) {
        return -1;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO activities (name, category, minrepeats, maxrepeats, distance) VALUES (?, ?, ?, ?, ?)"))) {
        return -1;
    }

    q.addBindValue(name);
    q.addBindValue(category);
    q.addBindValue(minRepeats);
    q.addBindValue(maxRepeats);
    q.addBindValue(distance);

    if (!q.exec()) {
        return -1;
    }

    int id = q.lastInsertId().toInt();

    emit added(id, name, category, minRepeats, maxRepeats, distance);

    return id;
}



/*!
 * \brief Changes the activity with \a databaseId and updates the table data.
 *
 * Emits the edited() signal and returns true on success.
 */
bool ActivitiesController::edit(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE activities SET name = ?, category = ?, minRepeats = ?, maxRepeats = ?, distance = ? WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(name);
    q.addBindValue(newCategory);
    q.addBindValue(minRepeats);
    q.addBindValue(maxRepeats);
    q.addBindValue(distance);
    q.addBindValue(databaseId);

    if (!q.exec()) {
        return false;
    }

    emit edited(databaseId, name, oldCategory, newCategory, minRepeats, maxRepeats, distance);

    return true;
}


/*!
 * \brief Removes the activity with \q databaseId.
 *
 * Emits the removed() signal and return true on success.
 */
bool ActivitiesController::remove(int databaseId, int category)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("REMOVE FROM activities WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(databaseId);

    if (!q.exec()) {
        return false;
    }

    emit removed(databaseId, category);

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
