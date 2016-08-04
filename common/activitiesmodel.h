#ifndef ACTIVITIESMODEL_H
#define ACTIVITIESMODEL_H

#include <QObject>
#include "dbmodel.h"

namespace Gibrievida {

class ActivitiesController;
class CategoriesController;

struct Activity {
    int databaseId;
    QString name;
    int minRepeats;
    int maxRepeats;
    bool distance;
    int categoryId;
    QString categoryName;
    QString categoryColor;
    int records;
};

class ActivitiesModel : public DBModel
{
    Q_OBJECT
public:
    explicit ActivitiesModel(QObject *parent = nullptr);
    ~ActivitiesModel();

    enum Roles {
        DatabaseId = Qt::UserRole + 1,
        Name,
        MinRepeats,
        MaxRepeats,
        Distance,
        CategoryId,
        CategoryName,
        CategoryColor,
        Records
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    void setCategoriesController(CategoriesController *controller);
    CategoriesController *getCategoriesController() const;

    void setActivitiesController(ActivitiesController *controller);
    ActivitiesController *getActivitiesController() const;

public slots:
    void add(int databaseId, const QString &name, int category, int minRepeats, int maxRepeats, bool distance);
    void edit(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance);
    void remove(int databaseId, int category);
    void removeAll();

    void editCategory(int category, const QString &name, const QString &color);
    void removeCategory(int category);

private:
    QList<Activity*> m_activities;

    void init();
    void clear();

    int find(int databaseId);

    ActivitiesController *m_actsController;
    CategoriesController *m_catsController;

    Q_DISABLE_COPY(ActivitiesModel)
};

}
Q_DECLARE_TYPEINFO(Gibrievida::Activity, Q_PRIMITIVE_TYPE);

#endif // ACTIVITIESMODEL_H
