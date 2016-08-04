#ifndef ACTIVITIESFILTERMODEL_H
#define ACTIVITIESFILTERMODEL_H

#include <QObject>
#include "filtermodel.h"

namespace Gibrievida {

class ActivitiesModel;
class ActivitiesController;
class CategoriesController;

class ActivitiesFilterModel : public FilterModel
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::ActivitiesController *activitiesController READ getActivitiesController WRITE setActivitiesController)
    Q_PROPERTY(Gibrievida::CategoriesController *categoriesController READ getCategoriesController WRITE setCategoriesController)
public:
    explicit ActivitiesFilterModel(QObject *parent = nullptr);
    ~ActivitiesFilterModel();

    ActivitiesController *getActivitiesController() const;
    CategoriesController *getCategoriesController() const;

    void setActivitiesController(ActivitiesController *activitiesController);
    void setCategoriesController(CategoriesController *categoriesController);

private:
    ActivitiesModel *m_actsModel;
    Q_DISABLE_COPY(ActivitiesFilterModel)
};

}

#endif // ACTIVITIESFILTERMODEL_H
