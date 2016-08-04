#include "activitiesmodel.h"
#include "activitiescontroller.h"
#include "categoriescontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new activities model.
 */
ActivitiesModel::ActivitiesModel(QObject *parent) : DBModel(parent)
{
    m_actsController = nullptr;
    m_catsController = nullptr;
    init();
}



/*!
 * \brief Destroys the model and all model data.
 */
ActivitiesModel::~ActivitiesModel()
{
    qDeleteAll(m_activities);
    m_activities.clear();
}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> ActivitiesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(DatabaseId, QByteArrayLiteral("databaseId"));
    roles.insert(Name, QByteArrayLiteral("name"));
    roles.insert(MinRepeats, QByteArrayLiteral("minRepeats"));
    roles.insert(MaxRepeats, QByteArrayLiteral("maxRepeats"));
    roles.insert(Distance, QByteArrayLiteral("distance"));
    roles.insert(CategoryId, QByteArrayLiteral("categoryId"));
    roles.insert(CategoryName, QByteArrayLiteral("categoryName"));
    roles.insert(CategoryColor, QByteArrayLiteral("categoryColor"));
    roles.insert(Records, QByteArrayLiteral("records"));
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

    Activity *a = m_activities.at(index.row());

    switch(role) {
    case DatabaseId:
        return QVariant::fromValue(a->databaseId);
    case Name:
        return QVariant::fromValue(a->name);
    case MinRepeats:
        return QVariant::fromValue(a->minRepeats);
    case MaxRepeats:
        return QVariant::fromValue(a->maxRepeats);
    case Distance:
        return QVariant::fromValue(a->distance);
    case CategoryId:
        return QVariant::fromValue(a->categoryId);
    case CategoryName:
        return QVariant::fromValue(a->categoryName);
    case CategoryColor:
        return QVariant::fromValue(a->categoryColor);
    case Records:
        return QVariant::fromValue(a->records);
    default:
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

    if (!q.exec(QStringLiteral("SELECT a.id, a.name, a.minrepeats, a.maxrepeats, a.distance, a.category, c.name as categoryname, c.color, (SELECT COUNT(id) FROM records WHERE activity = a.id) AS records FROM activities a JOIN categories c ON c.id = a.category"))) {
        setInOperation(false);
        return;
    }

    QList<Activity*> t_activities;

    while (q.next()) {
        Activity *a = new Activity;
        a->databaseId = q.value(0).toInt();
        a->name = q.value(1).toString();
        a->minRepeats = q.value(2).toInt();
        a->maxRepeats = q.value(3).toInt();
        a->distance = q.value(4).toBool();
        a->categoryId = q.value(5).toInt();
        a->categoryName = q.value(6).toString();
        a->categoryColor = q.value(7).toString();
        a->records = q.value(8).toInt();
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
 * \brief Adds a new activity to the model.
 */
void ActivitiesModel::add(int databaseId, const QString &name, int category, int minRepeats, int maxRepeats, bool distance)
{
    if (!connectDb()) {
        return;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("SELECT name, color FROM categories WHERE id = ?"))) {
        return;
    }

    q.addBindValue(category);

    if (!q.exec()) {
        return;
    }

    Activity *a = new Activity;

    if (q.next()) {
        a->databaseId = databaseId;
        a->name = name;
        a->minRepeats = minRepeats;
        a->maxRepeats = maxRepeats;
        a->distance = distance;
        a->categoryId = category;
        a->categoryName = q.value(0).toString();
        a->categoryColor = q.value(1).toString();
    } else {
        delete a;
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_activities.append(a);

    endInsertRows();
}


/*!
 * \brief Updates the model after an activity has been changed.
 */
void ActivitiesModel::edit(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance)
{
    int idx = find(databaseId);

    if (idx < 0) {
        return;
    }

    QString categoryName;
    QString categoryColor;

    if (oldCategory != newCategory) {

        if (!connectDb()) {
            return;
        }

        QSqlQuery q(m_db);

        if (!q.prepare(QStringLiteral("SELECT name, color FROM categories WHERE id = ?"))) {
            return;
        }

        q.addBindValue(newCategory);

        if (!q.exec()) {
            return;
        }

        if (q.next()) {
            categoryName = q.value(0).toString();
            categoryColor = q.value(1).toString();
        }
    }

    Activity *a = m_activities.at(idx);
    a->name = name;
    a->minRepeats = minRepeats;
    a->maxRepeats = maxRepeats;
    a->distance = distance;

    if (oldCategory != newCategory) {
        a->categoryId = newCategory;
        a->categoryName = categoryName;
        a->categoryColor = categoryColor;
        emit dataChanged(index(idx), index(idx));
    } else {
        emit dataChanged(index(idx), index(idx), QVector<int>({Name, MinRepeats, MaxRepeats, Distance}));
    }

}



/*!
 * \brief Removes the activity with \a databaseId.
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
 * \brief Reloads the model after a category has been removed.
 */
void ActivitiesModel::removeCategory(int category)
{
    Q_UNUSED(category)
    init();
}




/*!
 * \brief Updates the model items after a category has been updated.
 */
void ActivitiesModel::editCategory(int category, const QString &name, const QString &color)
{
    if (m_activities.isEmpty()) {
        return;
    }

    if (category < 0) {
        return;
    }

    for (int i = 0; i < m_activities.size(); ++i) {
        if (m_activities.at(i)->categoryId == category) {
            m_activities.at(i)->categoryName = name;
            m_activities.at(i)->categoryColor = color;
            emit dataChanged(index(i), index(i), QVector<int>({CategoryName, CategoryColor}));
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
        if (m_activities.at(i)->databaseId == databaseId) {
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
            disconnect(m_actsController, &ActivitiesController::edited, this, &ActivitiesModel::edit);
            disconnect(m_actsController, &ActivitiesController::removed, this, &ActivitiesModel::remove);
            disconnect(m_actsController, &ActivitiesController::removedAll, this, &ActivitiesModel::removeAll);
        }
        m_actsController = controller;
        if (m_actsController) {
            connect(m_actsController, &ActivitiesController::added, this, &ActivitiesModel::add);
            connect(m_actsController, &ActivitiesController::edited, this, &ActivitiesModel::edit);
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
            disconnect(m_catsController, &CategoriesController::edited, this, &ActivitiesModel::editCategory);
        }
        m_catsController = controller;
        if (m_catsController) {
            connect(m_catsController, &CategoriesController::removed, this, &ActivitiesModel::removeCategory);
            connect(m_catsController, &CategoriesController::removedAll, this, &ActivitiesModel::removeAll);
            connect(m_catsController, &CategoriesController::edited, this, &ActivitiesModel::editCategory);
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
