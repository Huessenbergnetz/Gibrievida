#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QObject>
#include "dbmodel.h"

namespace Gibrievida {

class CategoriesController;
class ActivitiesController;

struct Category {
    int databaseId;
    QString name;
    QString color;
    int activities;
};

class CategoriesModel : public DBModel
{
    Q_OBJECT
public:
    explicit CategoriesModel(QObject *parent = nullptr);
    ~CategoriesModel();

    enum Roles {
        DatabaseId = Qt::UserRole + 1,
        Name,
        Color,
        Activities
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
    void add(int databaseId, const QString &name, const QString &color);
    void edit(int databaseId, const QString &name, const QString &color);
    void remove(int databaseId);
    void removeAll();

    void addActivity(int databaseId, const QString &name, int category, int minRepeats, int maxRepeats, bool distance);
    void editActivity(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance);
    void removeActivity(int databaseId, int category);
    void removeAllActivities();

private:
    QList<Category*> m_categories;
    void init();
    void clear();
    int find(int databaseId);
    CategoriesController *m_controller;
    ActivitiesController *m_actsController;

    Q_DISABLE_COPY(CategoriesModel)
};

}
Q_DECLARE_TYPEINFO(Gibrievida::Category, Q_PRIMITIVE_TYPE);

#endif // CATEGORIESMODEL_H