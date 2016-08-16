#include "recordsmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QtCore/qmath.h>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new records model.
 */
RecordsModel::RecordsModel(QObject *parent) : DBModel(parent)
{
    m_recsController = nullptr;
    m_actsController = nullptr;
    m_catsController = nullptr;
    m_activityId = 0;
    m_order = QStringLiteral("DESC");
    m_orderBy = QStringLiteral("end");
}


/*!
 * \brief Destroys the model and all model data.
 */
RecordsModel::~RecordsModel()
{
    qDeleteAll(m_records);
    m_records.clear();
}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> RecordsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(DatabaseId, QByteArrayLiteral("databaseId"));
    roles.insert(ActivityId, QByteArrayLiteral("activityId"));
    roles.insert(ActivityName, QByteArrayLiteral("activityName"));
    roles.insert(CategoryId, QByteArrayLiteral("categoryId"));
    roles.insert(CategoryName, QByteArrayLiteral("categoryName"));
    roles.insert(CategoryColor, QByteArrayLiteral("categoryColor"));
    roles.insert(Start, QByteArrayLiteral("start"));
    roles.insert(End, QByteArrayLiteral("end"));
    roles.insert(Duration, QByteArrayLiteral("duration"));
    roles.insert(DurationString, QByteArrayLiteral("durationString"));
    roles.insert(Repetitions, QByteArrayLiteral("repetitions"));
    roles.insert(Distance, QByteArrayLiteral("distance"));
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

    Record *r = m_records.at(index.row());

    switch(role) {
    case DatabaseId:
        return QVariant::fromValue(r->databaseId);
    case ActivityId:
        return QVariant::fromValue(r->activityId);
    case ActivityName:
        return QVariant::fromValue(r->activityName);
    case CategoryId:
        return QVariant::fromValue(r->categoryId);
    case CategoryName:
        return QVariant::fromValue(r->categoryName);
    case CategoryColor:
        return QVariant::fromValue(r->categoryColor);
    case Start:
        return QVariant::fromValue(r->start);
    case End:
        return QVariant::fromValue(r->end);
    case Duration:
        return QVariant::fromValue(r->duration);
    case DurationString:
        return QVariant::fromValue(r->durationString);
    case Repetitions:
        return QVariant::fromValue(r->repetitions);
    case Distance:
        return QVariant::fromValue(r->distance);
    default:
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

    QString queryString("SELECT r.id, r.activity, a.name, a.category, c.name, c.color, r.start, r.end, r.duration, r.repetitions, r.distance FROM records r JOIN activities a ON a.id = r.activity JOIN categories c ON c.id = a.category");

    if (m_activityId > 0) {
        queryString.append(QLatin1String(" WHERE r.activity = ?"));
    }

    queryString.append(QLatin1String(" ORDER BY ")).append(m_orderBy).append(QLatin1String(" ")).append(m_order);

    QSqlQuery q(m_db);

    if (!q.prepare(queryString)) {
        setInOperation(false);
        qDebug() << q.lastError().text();
        return;
    }

    if (m_activityId > 0) {
        q.addBindValue(m_activityId);
    }

//    q.addBindValue(m_orderBy);
//    q.addBindValue(m_order);

    if (!q.exec()) {
        setInOperation(false);
        qDebug() << q.lastError().text();
        return;
    }

    QList<Record*> t_records;

    while (q.next()) {
        Record *r = new Record;
        r->databaseId = q.value(0).toInt();
        r->activityId = q.value(1).toInt();
        r->activityName = q.value(2).toString();
        r->categoryId = q.value(3).toInt();
        r->categoryName = q.value(4).toString();
        r->categoryColor = q.value(5).toString();
        r->start = QDateTime::fromTime_t(q.value(6).toUInt());
        r->end = QDateTime::fromTime_t(q.value(7).toUInt());
        r->duration = q.value(8).toUInt();
        r->durationString = createDurationString(r->duration);
        r->repetitions = q.value(9).toUInt();
        r->distance = q.value(10).toDouble();
        t_records.append(r);
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
 * \brief Creates a human readable string from a duration in seconds.
 */
QString RecordsModel::createDurationString(uint duration)
{
    if (duration <= 0) {
        return tr("invalid");
    }

    if (duration > 0 && duration < 60) {
        //: the s is the abbreviation for second(s)
        return tr("%1s").arg(duration);
    }

    uint secs = duration;

    float fDays = secs/86400;
    uint days = qFloor(fDays);

    secs = secs - (86400*days);

    float fHours = secs/3600;
    uint hours = qFloor(fHours);

    secs = secs - (3600*hours);

    float fMins = secs/60;
    int mins = qFloor(fMins);

    secs = secs - (60*mins);

    QString durString;

    if (days) {
        //: d is the abbreviation for day, used in duration time display.
        durString.append(tr("%1d").arg(days)).append(QLatin1String(" "));
    }

    if (hours) {
        //: h is the abbreviation for hour, used in duration time display
        durString.append(tr("%1h").arg(hours)).append(QLatin1String(" "));
    }

    if (mins) {
        //: m is the abbreviation for minute. Used in duration time display.
        durString.append(tr("%1m").arg(mins)).append(QLatin1String(" "));
    }

    if (secs) {
        //: s is the abbreviation for second. Used in duration time display.
        durString.append(tr("%1s").arg(secs));
    }

    return durString;
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
    m_recsController = recordsController;
#ifdef QT_DEBUG
    qDebug() << " Set recordsController to" << m_recsController;
#endif
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
    m_catsController = categoriesController;
#ifdef QT_DEBUG
    qDebug() << " Set categoriesController to" << m_catsController;
#endif
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
    m_actsController = activitiesController;
#ifdef QT_DEBUG
    qDebug() << " Set activitiesController to" << m_actsController;
#endif
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