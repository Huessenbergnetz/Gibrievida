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

#include "activitiesmodel.h"
#include "activitiescontroller.h"
#include "categoriescontroller.h"
#include "recordscontroller.h"
#include "category.h"
#include "activity.h"
#include "record.h"
#include <QSqlQuery>
#include <QSqlError>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new activities model.
 *
 * On construction the model will be populated with data from the SQL database.
 */
ActivitiesModel::ActivitiesModel(QObject *parent) : DBModel(parent)
{
    m_actsController = nullptr;
    m_catsController = nullptr;
    m_recsController = nullptr;
    init();
}



/*!
 * \brief Destroys the model and all model data.
 */
ActivitiesModel::~ActivitiesModel()
{

}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> ActivitiesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Item, QByteArrayLiteral("item"));
    return roles;
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
int ActivitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_activities.count();
}


/*!
 * \brief Reimplemented from QAbstractListModel.
 */
QModelIndex ActivitiesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QVariant ActivitiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (m_activities.size()-1)) {
        return QVariant();
    }

    if (role == Item) {
        return QVariant::fromValue<Activity*>(m_activities.at(index.row()));
    } else {
        return QVariant();
    }
}



/*!
 * \brief Initializes the model data from the SQL database.
 */
void ActivitiesModel::init()
{
    setInOperation(true);

    clear();

    if (!connectDb()) {
        setInOperation(false);
        return;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("SELECT a.id, a.name, a.minrepeats, a.maxrepeats, a.distance, a.category, c.name as categoryname, c.color, (SELECT COUNT(id) FROM records WHERE activity = a.id) AS records, (SELECT COUNT(id) FROM activities WHERE category = a.category) AS catActs, a.sensor, a.sensorDelay FROM activities a JOIN categories c ON c.id = a.category"))) {
        setInOperation(false);
        return;
    }

    QList<Activity*> t_activities;

    while (q.next()) {
        Activity *a = new Activity(q.value(0).toInt(), q.value(1).toString(), q.value(2).toInt(), q.value(3).toInt(), q.value(4).toBool(), q.value(8).toInt(), q.value(10).toInt(), q.value(11).toInt(), this);
        Category *c = new Category(q.value(5).toInt(), q.value(6).toString(), q.value(7).toString(), q.value(9).toInt(), a);
        a->setCategory(c);
        t_activities.append(a);
    }

    if (!t_activities.isEmpty()) {

        beginInsertRows(QModelIndex(), 0, t_activities.count()-1);

        m_activities = t_activities;

        endInsertRows();
    }

    setInOperation(false);
}



/*!
 * \brief Clears the model and removes all model data.
 */
void ActivitiesModel::clear()
{
    if (!m_activities.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);

        qDeleteAll(m_activities);
        m_activities.clear();

        endRemoveRows();
    }
}



/*!
 * \brief Adds a new Activity to the model.
 */
void ActivitiesModel::add(int databaseId, const QString &name, Category *c, int minRepeats, int maxRepeats, bool distance, int sensorType, int sensorDelay)
{
    Activity *a = new Activity(databaseId, name, minRepeats, maxRepeats, distance, 0, sensorType, sensorDelay, this);
    c->setParent(a);
    a->setCategory(c);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_activities.append(a);

    endInsertRows();
}




/*!
 * \brief Removes the activity identified by \a databaseId.
 */
void ActivitiesModel::remove(int databaseId, int category)
{
    Q_UNUSED(category)

    int idx = find(databaseId);

    if (idx < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);

    delete m_activities.takeAt(idx);

    endRemoveRows();
}



/*!
 * \brief Removes all activites from the model.
 */
void ActivitiesModel::removeAll()
{
    clear();
}


/*!
 * \brief Reloads the model after a Category has been removed.
 */
void ActivitiesModel::removeCategory(int category)
{
    Q_UNUSED(category)
    init();
}




/*!
 * \brief Updates the model items after a Category has been updated.
 */
void ActivitiesModel::updateCategory(Category *c)
{
    if (m_activities.isEmpty()) {
        return;
    }

    if (!c) {
        return;
    }

    if (!c->isValid()) {
        return;
    }

    for (int i = 0; i < m_activities.size(); ++i) {
        Category *cat = m_activities.at(i)->category();
        if (cat->databaseId() == c->databaseId()) {
            cat->setName(c->name());
            cat->setColor(c->color());
        }
    }
}



/*!
 * \brief Returns the index of the model item identified by databaseId.
 *
 * If the item is not in the model, \a -1 will be returned.
 */
int ActivitiesModel::find(int databaseId)
{
    int idx = -1;

    if (m_activities.isEmpty()) {
        return idx;
    }

    for (int i = 0; i < m_activities.size(); ++i) {
        if (m_activities.at(i)->databaseId() == databaseId) {
            idx = i;
            break;
        }
    }

    return idx;
}



/*!
 * \brief Sets the activites controller.
 */
void ActivitiesModel::setActivitiesController(ActivitiesController *controller)
{
    if (controller != m_actsController) {
        if (m_actsController) {
            disconnect(m_actsController, &ActivitiesController::added, this, &ActivitiesModel::add);
            disconnect(m_actsController, &ActivitiesController::removed, this, &ActivitiesModel::remove);
            disconnect(m_actsController, &ActivitiesController::removedAll, this, &ActivitiesModel::removeAll);
        }
        m_actsController = controller;
        if (m_actsController) {
            connect(m_actsController, &ActivitiesController::added, this, &ActivitiesModel::add);
            connect(m_actsController, &ActivitiesController::removed, this, &ActivitiesModel::remove);
            connect(m_actsController, &ActivitiesController::removedAll, this, &ActivitiesModel::removeAll);
        }
    }
}


/*!
 * \brief Returns the currently set activities contoller.
 */
ActivitiesController *ActivitiesModel::getActivitiesController() const
{
    return m_actsController;
}



/*!
 * \brief Sets the categories controller.
 */
void ActivitiesModel::setCategoriesController(CategoriesController *controller)
{
    if (controller != m_catsController) {
        if (m_catsController) {
            disconnect(m_catsController, &CategoriesController::removed, this, &ActivitiesModel::removeCategory);
            disconnect(m_catsController, &CategoriesController::removedAll, this, &ActivitiesModel::removeAll);
            disconnect(m_catsController, &CategoriesController::updated, this, &ActivitiesModel::updateCategory);
        }
        m_catsController = controller;
        if (m_catsController) {
            connect(m_catsController, &CategoriesController::removed, this, &ActivitiesModel::removeCategory);
            connect(m_catsController, &CategoriesController::removedAll, this, &ActivitiesModel::removeAll);
            connect(m_catsController, &CategoriesController::updated, this, &ActivitiesModel::updateCategory);
        }
    }
}


/*!
 * \brief Returns the currently set categories controller.
 */
CategoriesController *ActivitiesModel::getCategoriesController() const
{
    return m_catsController;
}


/*!
 * \brief Sets the records controller.
 */
void ActivitiesModel::setRecordsController(RecordsController *controller)
{
    if (controller != m_recsController) {

        if (m_recsController) {
            disconnect(m_recsController, &RecordsController::removed, this, &ActivitiesModel::recordRemoved);
            disconnect(m_recsController, &RecordsController::removedByActivity, this, &ActivitiesModel::recordsRemovedByActivity);
            disconnect(m_recsController, &RecordsController::removedByCategory, this, &ActivitiesModel::recordsRemovedByCategory);
            disconnect(m_recsController, &RecordsController::finished, this, &ActivitiesModel::recordFinished);
        }
        m_recsController = controller;
        if (m_recsController) {
            connect(m_recsController, &RecordsController::removed, this, &ActivitiesModel::recordRemoved);
            connect(m_recsController, &RecordsController::removedByActivity, this, &ActivitiesModel::recordsRemovedByActivity);
            connect(m_recsController, &RecordsController::removedByCategory, this, &ActivitiesModel::recordsRemovedByCategory);
            connect(m_recsController, &RecordsController::finished, this, &ActivitiesModel::recordFinished);
        }
    }
}


/*!
 * \brief Returns the currently set records controller.
 */
RecordsController *ActivitiesModel::getRecordsController() const
{
    return m_recsController;
}




/*!
 * \brief Updates the records count of the \c activity after a records has been removed.
 */
void ActivitiesModel::recordRemoved(int record, int activity, int category)
{
    Q_UNUSED(record)
    Q_UNUSED(category)

    int idx = find(activity);

    if (idx < 0) {
        return;
    }

    Activity *a = m_activities.at(idx);

    a->setRecords(a->records()-1);
}



/*!
 * \brief Updates the records count of \c activity after all records of that activity have been removed.
 */
void ActivitiesModel::recordsRemovedByActivity(int activity, int category)
{
    Q_UNUSED(category)

    int idx = find(activity);

    if (idx < 0) {
        return;
    }

    m_activities.at(idx)->setRecords(0);
}


/*!
 * \brief Updates the records count of activities belonging to \c category after the records of that category have been removed.
 */
void ActivitiesModel::recordsRemovedByCategory(int category)
{
    if (m_activities.isEmpty()) {
        return;
    }

    for (int i = 0; i < m_activities.size(); ++i) {
        if (m_activities.at(i)->category()->databaseId() == category) {
            m_activities.at(i)->setRecords(0);
        }
    }
}


/*!
 * \brief Updates the record count after a record has been finished.
 */
void ActivitiesModel::recordFinished(Record *r)
{
    if (m_activities.isEmpty()) {
        return;
    }

    int idx = find(r->activity()->databaseId());

    if (idx < 0) {
        return;
    }

    Activity *a = m_activities.at(idx);

    a->setRecords(a->records() + 1);
}
