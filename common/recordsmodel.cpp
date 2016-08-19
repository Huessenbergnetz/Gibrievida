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

#include "recordsmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QtCore/qmath.h>
#include "recordscontroller.h"
#include "activitiescontroller.h"
#include "categoriescontroller.h"
#include "category.h"
#include "activity.h"
#include "record.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new empty records model.
 */
RecordsModel::RecordsModel(QObject *parent) : DBModel(parent)
{
    m_recsController = nullptr;
    m_actsController = nullptr;
    m_catsController = nullptr;
    m_activityId = 0;
    m_categoryId = 0;
    m_order = QStringLiteral("DESC");
    m_orderBy = QStringLiteral("start");
}


/*!
 * \brief Destroys the model and all model data.
 */
RecordsModel::~RecordsModel()
{

}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> RecordsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Item, QByteArrayLiteral("item"));
    return roles;
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
int RecordsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_records.count();
}


/*!
 * \brief Reimplemented from QAbstractListModel.
 */
QModelIndex RecordsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QVariant RecordsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (m_records.count()-1)) {
        return QVariant();
    }

    if (role == Item) {
        return QVariant::fromValue<Record*>(m_records.at(index.row()));
    } else {
        return QVariant();
    }
}




/*!
 * \brief Initializes the model data from the SQL database.
 */
void RecordsModel::update()
{
    setInOperation(true);

    clear();

    if (!connectDb()) {
        setInOperation(false);
        return;
    }

    QString queryString("SELECT r.id, r.activity, a.name, a.category, c.name, c.color, r.start, r.end, r.duration, r.repetitions, r.distance, a.minRepeats, a.maxRepeats, a.distance, r.note, r.tpr, r.maxSpeed, r.avgSpeed FROM records r JOIN activities a ON a.id = r.activity JOIN categories c ON c.id = a.category WHERE r.end > 0");

    if (m_categoryId > 0) {
        queryString.append(QLatin1String(" AND a.category = ?"));
    } else if (m_activityId > 0) {
        queryString.append(QLatin1String(" AND r.activity = ?"));
    }

    queryString.append(QLatin1String(" ORDER BY ")).append(m_orderBy).append(QLatin1String(" ")).append(m_order);

    QSqlQuery q(m_db);

    if (!q.prepare(queryString)) {
        setInOperation(false);
        return;
    }

    if (m_categoryId > 0) {
        q.addBindValue(m_categoryId);
    } else if (m_activityId > 0) {
        q.addBindValue(m_activityId);
    }

    if (!q.exec()) {
        setInOperation(false);
        return;
    }

    QList<Record*> t_records;

    while (q.next()) {
        Record *r = new Record(q.value(0).toInt(),
                               QDateTime::fromTime_t(q.value(6).toUInt()),
                               QDateTime::fromTime_t(q.value(7).toUInt()),
                               q.value(8).toUInt(),
                               q.value(9).toUInt(),
                               q.value(10).toDouble(),
                               q.value(14).toString(),
                               q.value(15).toFloat(),
                               q.value(16).toFloat(),
                               q.value(17).toFloat(),
                               this
                               );

        Activity *a = new Activity(q.value(1).toInt(), q.value(2).toString(), q.value(11).toInt(), q.value(12).toInt(), q.value(13).toBool(), 0, r);

        Category *c = new Category(q.value(3).toInt(), q.value(4).toString(), q.value(5).toString(), 0, a);

        a->setCategory(c);
        r->setActivity(a);

        if (r->isValid()) {
            t_records.append(r);
        } else {
            delete r;
        }

    }

    if (!t_records.isEmpty()) {

        beginInsertRows(QModelIndex(), 0, t_records.size()-1);

        m_records = t_records;

        endInsertRows();
    }

    setInOperation(false);
}


/*!
 * \brief Clears the model and removes all model items.
 */
void RecordsModel::clear()
{
    if (!m_records.isEmpty()) {

        beginRemoveRows(QModelIndex(), 0, rowCount()-1);

        qDeleteAll(m_records);
        m_records.clear();

        endRemoveRows();

    }
}



/*!
 * \property RecordsModel::recordsController
 * \brief Sets the records controller object.
 *
 * \par Access functions:
 * <TABLE><TR><TD>RecordsController*</TD><TD>getRecordsController() const</TD></TR><TR><TD>void</TD><TD>setRecordsController(RecordsController *recordsController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::recordsController recordsController \endlink property.
 */
RecordsController *RecordsModel::getRecordsController() const { return m_recsController; }

/*!
 * \brief Part of the \link RecordsModel::recordsController recordsController \endlink property.
 */
void RecordsModel::setRecordsController(RecordsController *recordsController)
{
    if (m_recsController) {
        disconnect(m_recsController, &RecordsController::finished, this, &RecordsModel::finished);
        disconnect(m_recsController, &RecordsController::removed, this, &RecordsModel::removed);
        disconnect(m_recsController, &RecordsController::removedAll, this, &RecordsModel::removedAll);
        disconnect(m_recsController, &RecordsController::removedByActivity, this, &RecordsModel::removedByActivity);
        disconnect(m_recsController, &RecordsController::removedByCategory, this, &RecordsModel::removedByCategory);
    }

    m_recsController = recordsController;
#ifdef QT_DEBUG
    qDebug() << " Set recordsController to" << m_recsController;
#endif

    if (m_recsController) {
        connect(m_recsController, &RecordsController::finished, this, &RecordsModel::finished);
        connect(m_recsController, &RecordsController::removed, this, &RecordsModel::removed);
        connect(m_recsController, &RecordsController::removedAll, this, &RecordsModel::removedAll);
        connect(m_recsController, &RecordsController::removedByActivity, this, &RecordsModel::removedByActivity);
        connect(m_recsController, &RecordsController::removedByCategory, this, &RecordsModel::removedByCategory);
    }
}




/*!
 * \property RecordsModel::categoriesController
 * \brief Sets the categories controller object.
 *
 * \par Access functions:
 * <TABLE><TR><TD>CategoriesController*</TD><TD>getCategoriesController() const</TD></TR><TR><TD>void</TD><TD>setCategoriesController(CategoriesController *categoriesController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::categoriesController categoriesController \endlink property.
 */
CategoriesController *RecordsModel::getCategoriesController() const { return m_catsController; }

/*!
 * \brief Part of the \link RecordsModel::categoriesController categoriesController \endlink property.
 */
void RecordsModel::setCategoriesController(CategoriesController *categoriesController)
{
    if (m_catsController) {
        disconnect(m_catsController, &CategoriesController::removed, this, &RecordsModel::removedByCategory);
        disconnect(m_catsController, &CategoriesController::removedAll, this, &RecordsModel::removedAll);
    }

    m_catsController = categoriesController;
#ifdef QT_DEBUG
    qDebug() << " Set categoriesController to" << m_catsController;
#endif

    if (m_catsController) {
        connect(m_catsController, &CategoriesController::removed, this, &RecordsModel::removedByCategory);
        connect(m_catsController, &CategoriesController::removedAll, this, &RecordsModel::removedAll);
    }
}




/*!
 * \property RecordsModel::activitiesController
 * \brief Sets the activities controller object.
 *
 * \par Access functions:
 * <TABLE><TR><TD>ActivitiesController*</TD><TD>getActivitiesController() const</TD></TR><TR><TD>void</TD><TD>setActivitiesController(ActivitiesController *activitiesController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::activitiesController activitiesController \endlink property.
 */
ActivitiesController *RecordsModel::getActivitiesController() const { return m_actsController; }

/*!
 * \brief Part of the \link RecordsModel::activitiesController activitiesController \endlink property.
 */
void RecordsModel::setActivitiesController(ActivitiesController *activitiesController)
{
    if (m_actsController) {
        disconnect(m_actsController, &ActivitiesController::removed, this, &RecordsModel::removedByActivity);
        disconnect(m_actsController, &ActivitiesController::removedAll, this, &RecordsModel::removedAll);
    }

    m_actsController = activitiesController;
#ifdef QT_DEBUG
    qDebug() << " Set activitiesController to" << m_actsController;
#endif

    if (m_actsController) {
        connect(m_actsController, &ActivitiesController::removed, this, &RecordsModel::removedByActivity);
        connect(m_actsController, &ActivitiesController::removedAll, this, &RecordsModel::removedAll);
    }
}




/*!
 * \property RecordsModel::activityId
 * \brief Sets the activity id to request records for.
 *
 * If the id is 0 (the default) all recrods will be returned.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getActivityId() const</TD></TR><TR><TD>void</TD><TD>setActivityId(int activityId)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::activityId activityId \endlink property.
 */
int RecordsModel::getActivityId() const { return m_activityId; }

/*!
 * \brief Part of the \link RecordsModel::activityId activityId \endlink property.
 */
void RecordsModel::setActivityId(int activityId)
{
    m_activityId = activityId;
#ifdef QT_DEBUG
    qDebug() << " Set activityId to" << m_activityId;
#endif
}



/*!
 * \property RecordsModel::categoryId
 * \brief Database ID of the category to request records for.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCategoryId() const</TD></TR><TR><TD>void</TD><TD>setCategoryId(int categoryId)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::categoryId categoryId \endlink property.
 */
int RecordsModel::getCategoryId() const { return m_categoryId; }

/*!
 * \brief Part of the \link RecordsModel::categoryId categoryId \endlink property.
 */
void RecordsModel::setCategoryId(int categoryId)
{
    m_categoryId = categoryId;
#ifdef QT_DEBUG
    qDebug() << " Set categoryId to" << m_categoryId;
#endif
}





/*!
 * \property RecordsModel::order
 * \brief Defines the order of the records.
 *
 * Should be eather ASC or DESC. Default is DESC.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getOrder() const</TD></TR><TR><TD>void</TD><TD>setOrder(const QString &order)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::order order \endlink property.
 */
QString RecordsModel::getOrder() const { return m_order; }

/*!
 * \brief Part of the \link RecordsModel::order order \endlink property.
 */
void RecordsModel::setOrder(const QString &order)
{
    m_order = order;
#ifdef QT_DEBUG
    qDebug() << " Set order to" << m_order;
#endif
}




/*!
 * \property RecordsModel::orderBy
 * \brief The table column to order the records by.
 *
 * Default is end.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getOrderBy() const</TD></TR><TR><TD>void</TD><TD>setOrderBy(const QString &orderBy)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link RecordsModel::orderBy orderBy \endlink property.
 */
QString RecordsModel::getOrderBy() const { return m_orderBy; }

/*!
 * \brief Part of the \link RecordsModel::orderBy orderBy \endlink property.
 */
void RecordsModel::setOrderBy(const QString &orderBy)
{
    m_orderBy = orderBy;
#ifdef QT_DEBUG
    qDebug() << " Set orderBy to" << m_orderBy;
#endif
}



/*!
 * \brief Returns the model index of the record identified by \c databaseId.
 */
int RecordsModel::find(int databaseId) const
{
    if (m_records.isEmpty()) {
        return -1;
    }

    int idx = -1;

    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records.at(i)->databaseId() == databaseId) {
            idx = i;
            break;
        }
    }

    return idx;
}


/*!
 * \brief Returns a list of model indeces of records that are part of \c activity.
 *
 * \c activity is the database ID of the Activity.
 */
QList<int> RecordsModel::findByActivity(int activity) const
{
    if (m_records.isEmpty()) {
        return QList<int>();
    }

    QList<int> idxs;

    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records.at(i)->activity()->databaseId() == activity) {
            idxs.append(i);
        }
    }

    return idxs;
}



/*!
 * \brief Returns a list of model indeces of records that are part of \c category.
 *
 * \c category is the database ID of the Category.
 */
QList<int> RecordsModel::findByCategory(int category) const
{
    if (m_records.isEmpty()) {
        return QList<int>();
    }

    QList<int> idxs;

    for (int i = 0; i < m_records.size(); ++i) {
        if (m_records.at(i)->activity()->category()->databaseId() == category) {
            idxs.append(i);
        }
    }

    return idxs;
}



/*!
 * \brief Adds a finished Record to the model.
 */
void RecordsModel::finished(Record *record)
{
    // let's check if the new record is part of the current model data
    // model might display all records or only records for a specific category or activity
    if ((m_activityId <= 0 && m_categoryId <= 0) || m_categoryId == record->activity()->category()->databaseId() || m_activityId == record->activity()->databaseId()) {

        if (record->isValid()) {
            record->setParent(this);
        } else {
            return;
        }

        if (m_orderBy == QLatin1String("start")) {

            if (m_order == QLatin1String("DESC")) {
                beginInsertRows(QModelIndex(), 0, 0);
                m_records.prepend(record);
                endInsertRows();
            } else if (m_order == QLatin1String("ASC")) {
                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_records.append(record);
                endInsertRows();
            } else {
                update();
            }

        } else {
            update();
        }

    }
}


/*!
 * \brief Removes the record identified by \c databaseId from the model.
 */
void RecordsModel::removed(int databaseId, int activity, int category)
{
    Q_UNUSED(activity)
    Q_UNUSED(category)

    int idx = find(databaseId);

    if (idx < 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);

    delete m_records.takeAt(idx);

    endRemoveRows();
}



/*!
 * \brief Removes all records that are part of the \c activity.
 *
 * \c activity is the database ID of the Activity.
 */
void RecordsModel::removedByActivity(int activity, int category)
{
    Q_UNUSED(category)

    if (m_activityId > 0 && m_activityId == activity) {
        clear();
        return;
    }

    QList<int> idxs = findByActivity(activity);

    if (idxs.isEmpty()) {
        return;
    }

    for (int i = 0; i < idxs.size(); ++i) {
        int idx = idxs.at(i) - i;
        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_records.takeAt(idx);

        endRemoveRows();
    }
}


/*!
 * \brief Removes all records that are part of the Category identified by \c categoryId.
 *
 * \c categoryId is the database ID of the Category.
 */
void RecordsModel::removedByCategory(int categoryId)
{
    if (m_categoryId > 0 && m_categoryId == categoryId) {
        clear();
        return;
    }

    QList<int> idxs = findByCategory(categoryId);

    if (idxs.isEmpty()) {
        return;
    }

    for (int i = 0; i < idxs.size(); ++i) {
        int idx = idxs.at(i) - i;
        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_records.takeAt(idx);

        endRemoveRows();
    }
}


/*!
 * \brief Removes all entries from the model.
 */
void RecordsModel::removedAll()
{
    clear();
}
