#ifndef CATEGORIESFILTERMODEL_H
#define CATEGORIESFILTERMODEL_H

#include <QObject>
#include "filtermodel.h"

namespace Gibrievida {

class CategoriesModel;
class CategoriesController;
class ActivitiesController;

class CategoriesFilterModel : public FilterModel
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::CategoriesController *controller READ getController WRITE setController)
    Q_PROPERTY(Gibrievida::ActivitiesController *activitiesController READ getActivitiesController WRITE setActivitiesController)
public:
    explicit CategoriesFilterModel(QObject *parent = nullptr);
    ~CategoriesFilterModel();

    CategoriesController *getController() const;
    ActivitiesController *getActivitiesController() const;

    void setController(CategoriesController *controller);
    void setActivitiesController(ActivitiesController *activitiesController);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    CategoriesModel *m_catsModel;
    Q_DISABLE_COPY(CategoriesFilterModel)
};

}

#endif // CATEGORIESFILTERMODEL_H
