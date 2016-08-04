#ifndef CATEGORIESFILTERMODEL_H
#define CATEGORIESFILTERMODEL_H

#include <QObject>
#include "filtermodel.h"

namespace Gibrievida {

class CategoriesModel;
class CategoriesController;

class CategoriesFilterModel : public FilterModel
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::CategoriesController *controller READ getController WRITE setController)
public:
    explicit CategoriesFilterModel(QObject *parent = nullptr);
    ~CategoriesFilterModel();

    CategoriesController *getController() const;

    void setController(CategoriesController *controller);

private:
    CategoriesModel *m_catsModel;
    Q_DISABLE_COPY(CategoriesFilterModel)
};

}

#endif // CATEGORIESFILTERMODEL_H
