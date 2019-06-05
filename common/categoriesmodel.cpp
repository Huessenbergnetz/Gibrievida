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

#include "categoriesmodel.h"
#include <QSqlQuery>
#include "categoriescontroller.h"
#include "activitiescontroller.h"
#include "category.h"
#include "activity.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new categories database model.
 */
CategoriesModel::CategoriesModel(QObject *parent) : DBModel(parent)
{
    m_controller = nullptr;
    m_actsController = nullptr;
    init();
}


/*!
 * \brief Destroys the categories database model and all model data.
 */
CategoriesModel::~CategoriesModel()
{

}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> CategoriesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Item, QByteArrayLiteral("item"));
    return roles;
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
int CategoriesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_categories.count();
}



/*!
 * \brief Reimplemented from QAbstractListModel.
 */
QModelIndex CategoriesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QVariant CategoriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (m_categories.size()-1)) {
        return QVariant();
    }

    if (role == Item) {
        return QVariant::fromValue<Category*>(m_categories.at(index.row()));
    } else {
        return QVariant();
    }
}



/*!
 * \brief Initializes the model data from the SQL database.
 */
void CategoriesModel::init()
{
    setInOperation(true);

    clear();

    if (!connectDb()) {
        setInOperation(false);
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("SELECT c.id, c.name, c.color, (SELECT COUNT(id) FROM activities WHERE category = c.id) AS activities FROM categories c ORDER BY name ASC"))) {
        setInOperation(false);
        return;
    }

    if (!q.exec()) {
        setInOperation(false);
        return;
    }

    QList<Category*> t_categories;

    while (q.next()) {
        Category *c = new Category(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(3).toInt(), this);
        t_categories.append(c);
    }

    if (!t_categories.isEmpty()) {

        beginInsertRows(QModelIndex(), 0, t_categories.size()-1);

        m_categories = t_categories;

        endInsertRows();
    }

    setInOperation(false);
}


/*!
 * \brief Clears the model and removes all model data.
 */
void CategoriesModel::clear()
{
    if (!m_categories.isEmpty()) {

        beginRemoveRows(QModelIndex(), 0, rowCount()-1);

        qDeleteAll(m_categories);
        m_categories.clear();

        endRemoveRows();
    }
}




/*!
 * \brief Removes the category with \a databaseId.
 */
void CategoriesModel::remove(int databaseId)
{
    int idx = find(databaseId);

    if (idx < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);

    delete m_categories.takeAt(idx);

    endRemoveRows();
}



/*!
 * \brief Removes all categories from the model.
 */
void CategoriesModel::removeAll()
{
    clear();
}



/*!
 * \brief Adds a new category to the model.
 */
void CategoriesModel::add(int databaseId, const QString &name, const QString &color)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    Category *c = new Category(databaseId, name, color, 0, this);
    m_categories.append(c);

    endInsertRows();
}





/*!
 * \brief Returns the index of the model item identified by databaseId.
 *
 * If the item is not in the model, \a -1 will be returned.
 */
int CategoriesModel::find(int databaseId)
{
    int idx = -1;

    if (m_categories.isEmpty()) {
        return idx;
    }

    for (int i = 0; i < m_categories.size(); ++i) {
        if (m_categories.at(i)->databaseId() == databaseId) {
            idx = i;
            break;
        }
    }

    return idx;
}




/*!
 * \brief This slot updates the model after a new Activity has been added.
 */
void CategoriesModel::addActivity(int databaseId, const QString &name, Category *c, int minRepeats, int maxRepeats, bool useDistance)
{
    Q_UNUSED(databaseId)
    Q_UNUSED(name)
    Q_UNUSED(minRepeats)
    Q_UNUSED(maxRepeats)
    Q_UNUSED(useDistance)

    int idx = find(c->databaseId());

    if (idx < 0) {
        return;
    }

    m_categories.at(idx)->increaseActivities();
}



/*!
 * \brief This slot updates the model after an Activity has been changed.
 */
void CategoriesModel::updateActivity(Activity *a, int oldCategoryId)
{
    if (a->category()->databaseId() != oldCategoryId) {
        int idx = find(oldCategoryId);

        if (idx > -1) {
            m_categories.at(idx)->decreaseActivities();
        }

        idx = find(a->category()->databaseId());

        if (idx > -1) {
            m_categories.at(idx)->increaseActivities();
        }
    }
}



/*!
 * \brief This slot updates the model after an Activity has been removed.
 */
void CategoriesModel::removeActivity(int databaseId, int category)
{
    Q_UNUSED(databaseId);

    int idx = find(category);

    if (idx < 0) {
        return;
    }

    m_categories.at(idx)->decreaseActivities();
}





/*!
 * \brief This slot updates the model after all activities have been removed.
 */
void CategoriesModel::removeAllActivities()
{
    if (m_categories.isEmpty()) {
        return;
    }

    for (int i = 0; i < m_categories.size(); ++i) {
        m_categories.at(i)->setActivities(0);
    }

}




/*!
 * \brief Sets the categories controller and connnects signals and slots.
 */
void CategoriesModel::setCategoriesController(CategoriesController *controller)
{
    if (controller != m_controller) {
        if (m_controller) {
            disconnect(m_controller, &CategoriesController::added, this, &CategoriesModel::add);
            disconnect(m_controller, &CategoriesController::removed, this, &CategoriesModel::remove);
            disconnect(m_controller, &CategoriesController::removedAll, this, &CategoriesModel::removeAll);
        }
        m_controller = controller;
        if (m_controller) {
            connect(m_controller, &CategoriesController::added, this, &CategoriesModel::add);
            connect(m_controller, &CategoriesController::removed, this, &CategoriesModel::remove);
            connect(m_controller, &CategoriesController::removedAll, this, &CategoriesModel::removeAll);
        }
    }
}


/*!
 * \brief Returns the currently set categories controller.
 */
CategoriesController *CategoriesModel::getCategoriesController() const
{
    return m_controller;
}


/*!
 * \brief Sets the activities controller.
 */
void CategoriesModel::setActivitiesController(ActivitiesController *controller)
{
    if (controller != m_actsController) {
        if (m_actsController) {
            disconnect(m_actsController, &ActivitiesController::added, this, &CategoriesModel::addActivity);
            disconnect(m_actsController, &ActivitiesController::updated, this, &CategoriesModel::updateActivity);
            disconnect(m_actsController, &ActivitiesController::removed, this, &CategoriesModel::removeActivity);
            disconnect(m_actsController, &ActivitiesController::removedAll, this, &CategoriesModel::removeAllActivities);
        }
        m_actsController = controller;
        if (m_actsController) {
            connect(m_actsController, &ActivitiesController::added, this, &CategoriesModel::addActivity);
            connect(m_actsController, &ActivitiesController::updated, this, &CategoriesModel::updateActivity);
            connect(m_actsController, &ActivitiesController::removed, this, &CategoriesModel::removeActivity);
            connect(m_actsController, &ActivitiesController::removedAll, this, &CategoriesModel::removeAllActivities);
        }
    }
}


/*!
 * \brief Returns the currently set activities controller.
 */
ActivitiesController *CategoriesModel::getActivitiesController() const
{
    return m_actsController;
}
