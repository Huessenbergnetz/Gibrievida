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

#ifndef ACTIVITIESFILTERMODEL_H
#define ACTIVITIESFILTERMODEL_H

#include <QObject>
#include "filtermodel.h"

namespace Gibrievida {

class ActivitiesModel;
class ActivitiesController;
class CategoriesController;
class RecordsController;

/*!
 * \brief Proxy model to filter the ActivitiesModel.
 */
class ActivitiesFilterModel : public FilterModel
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::ActivitiesController *activitiesController READ getActivitiesController WRITE setActivitiesController)
    Q_PROPERTY(Gibrievida::CategoriesController *categoriesController READ getCategoriesController WRITE setCategoriesController)
    Q_PROPERTY(Gibrievida::RecordsController *recordsController READ getRecordsController WRITE setRecordsController)
public:
    explicit ActivitiesFilterModel(QObject *parent = nullptr);
    ~ActivitiesFilterModel();

    ActivitiesController *getActivitiesController() const;
    CategoriesController *getCategoriesController() const;
    RecordsController *getRecordsController() const;

    void setActivitiesController(ActivitiesController *activitiesController);
    void setCategoriesController(CategoriesController *categoriesController);
    void setRecordsController(RecordsController *recordsController);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    ActivitiesModel *m_actsModel;
    Q_DISABLE_COPY(ActivitiesFilterModel)
};

}

#endif // ACTIVITIESFILTERMODEL_H
