#ifndef RECORDSMODEL_H
#define RECORDSMODEL_H

#include <QObject>
#include <QDateTime>
#include "dbmodel.h"
#include "helpers.h"

namespace Gibrievida {

class RecordsController;
class ActivitiesController;
class CategoriesController;
class Record;

class RecordsModel : public DBModel
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::RecordsController *recordsController READ getRecordsController WRITE setRecordsController)
    Q_PROPERTY(Gibrievida::CategoriesController *categoriesController READ getCategoriesController WRITE setCategoriesController)
    Q_PROPERTY(Gibrievida::ActivitiesController *activitiesController READ getActivitiesController WRITE setActivitiesController)
    Q_PROPERTY(int activityId READ getActivityId WRITE setActivityId)
    Q_PROPERTY(QString order READ getOrder WRITE setOrder)
    Q_PROPERTY(QString orderBy READ getOrderBy WRITE setOrderBy)
public:
    explicit RecordsModel(QObject *parent = nullptr);
    ~RecordsModel();

    enum Roles {
        Item = Qt::UserRole + 1
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    void setRecordsController(RecordsController *controller);
    RecordsController *getRecordsController() const;

    void setCategoriesController(CategoriesController *controller);
    CategoriesController *getCategoriesController() const;

    void setActivitiesController(ActivitiesController *controller);
    ActivitiesController *getActivitiesController() const;

    void setActivityId(int activityId);
    int getActivityId() const;

    void setOrder(const QString &order);
    QString getOrder() const;

    void setOrderBy(const QString &orderBy);
    QString getOrderBy() const;

public slots:
    void update();
    void finished(Record *record);
    void removed(int databaseId, int activity, int category);
    void removedByActivity(int activity, int category);
    void removedAll();

private:
    QList<Record*> m_records;

    RecordsController *m_recsController;
    CategoriesController *m_catsController;
    ActivitiesController *m_actsController;
    int m_activityId;
    QString m_order;
    QString m_orderBy;

    void clear();

    Helpers helpers;

    int find(int databaseId) const;
    QList<int> findByActivity(int activity) const;

    QString createDurationString(uint duration);

    Q_DISABLE_COPY(RecordsModel)
};

}

#endif // RECORDSMODEL_H
