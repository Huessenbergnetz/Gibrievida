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
