#include "categoriesmodel.h"
#include <QSqlQuery>
#include "categoriescontroller.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new categories database model.
 */
CategoriesModel::CategoriesModel(QObject *parent) : DBModel(parent)
{
    init();
    m_controller = nullptr;
}


/*!
 * \brief Destroys the categories database model and all model data.
 */
CategoriesModel::~CategoriesModel()
{
    qDeleteAll(m_categories);
    m_categories.clear();
}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> CategoriesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(DatabaseId, QByteArrayLiteral("databaseId"));
    roles.insert(Name, QByteArrayLiteral("name"));
    roles.insert(Color, QByteArrayLiteral("color"));
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

    Category *c = m_categories.at(index.row());

    switch(role) {
    case DatabaseId:
        return QVariant::fromValue(c->databaseId);
    case Name:
        return QVariant::fromValue(c->name);
    case Color:
        return QVariant::fromValue(c->color);
    default:
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

    if (!q.prepare(QStringLiteral("SELECT id, name, color FROM categories ORDER BY name ASC"))) {
        setInOperation(false);
        return;
    }

    if (!q.exec()) {
        setInOperation(false);
        return;
    }

    QList<Category*> t_categories;

    while (q.next()) {
        Category *c = new Category;
        c->databaseId = q.value(0).toInt();
        c->name = q.value(1).toString();
        c->color = q.value(2).toString();
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

    Category *c = new Category;
    c->databaseId = databaseId;
    c->name = name;
    c->color = color;
    m_categories.append(c);

    endInsertRows();
}



/*!
 * \brief Edits the category with \a databaseId.
 */
void CategoriesModel::edit(int databaseId, const QString &name, const QString &color)
{
    int idx = find(databaseId);

    if (idx < 0) {
        return;
    }

    Category *c = m_categories.at(idx);
    c->name = name;
    c->color = color;

    emit dataChanged(index(idx), index(idx), QVector<int>({Name, Color}));
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
        if (m_categories.at(i)->databaseId == databaseId) {
            idx = i;
            break;
        }
    }

    return idx;
}




void CategoriesModel::setCategoriesController(CategoriesController *controller)
{
    if (controller != m_controller) {
        if (m_controller) {
            disconnect(m_controller, &CategoriesController::added, this, &CategoriesModel::add);
            disconnect(m_controller, &CategoriesController::edited, this, &CategoriesModel::edit);
            disconnect(m_controller, &CategoriesController::removed, this, &CategoriesModel::remove);
            disconnect(m_controller, &CategoriesController::removedAll, this, &CategoriesModel::removeAll);
        }
        m_controller = controller;
        if (m_controller) {
            connect(m_controller, &CategoriesController::added, this, &CategoriesModel::add);
            connect(m_controller, &CategoriesController::edited, this, &CategoriesModel::edit);
            connect(m_controller, &CategoriesController::removed, this, &CategoriesModel::remove);
            connect(m_controller, &CategoriesController::removedAll, this, &CategoriesModel::removeAll);
        }
    }
}


CategoriesController *CategoriesModel::getCategoriesController() const
{
    return m_controller;
}
