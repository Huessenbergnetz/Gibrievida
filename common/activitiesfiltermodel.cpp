/*
    Gibrievida - An activity tracker
    Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
    https://github.com/Huessenbergnetz/Gibrievida

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

#include "activitiesfiltermodel.h"
#include "activitiesmodel.h"
#include "activity.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new activities filter model.
 *
 * This will also create the underlying ActivitiesModel.
 */
ActivitiesFilterModel::ActivitiesFilterModel(QObject *parent) : FilterModel(parent)
{
    m_actsModel = new ActivitiesModel(this);
    connect(m_actsModel, &DBModel::inOperationChanged, this, &FilterModel::setInOperation);
    setSourceModel(m_actsModel);
    setSortLocaleAware(true);
    setSortRole(ActivitiesModel::Item);
    sort(0);
}


/*!
 * \brief Destroys the activity filter model.
 *
 * This will also destroy the underlying ActivitiesModel.
 */
ActivitiesFilterModel::~ActivitiesFilterModel()
{

}


/*!
 * \brief Reimplemented from QSortFilterProxyModel.
 */
bool ActivitiesFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Activity *l = sourceModel()->data(left, ActivitiesModel::Item).value<Activity*>();
    Activity *r = sourceModel()->data(right, ActivitiesModel::Item).value<Activity*>();

    return QString::localeAwareCompare(l->name(), r->name()) < 0;
}


/*!
 * \brief Reimplemented from QSortFilterProxyModel.
 */
bool ActivitiesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);

    return sourceModel()->data(idx, ActivitiesModel::Item).value<Activity*>()->name().contains(filterRegExp());
}



/*!
 * \property ActivitiesFilterModel::activitiesController
 * \brief Sets the activities contoller to update the model on activity cahnges.
 *
 * \par Access functions:
 * <TABLE><TR><TD>ActivitiesController*</TD><TD>getActivitiesController() const</TD></TR><TR><TD>void</TD><TD>setActivitiesController(ActivitiesController *activitiesController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link ActivitiesFilterModel::activitiesController activitiesController \endlink property.
 */
ActivitiesController *ActivitiesFilterModel::getActivitiesController() const { return m_actsModel->getActivitiesController(); }

/*!
 * \brief Part of the \link ActivitiesFilterModel::activitiesController activitiesController \endlink property.
 */
void ActivitiesFilterModel::setActivitiesController(ActivitiesController *activitiesController)
{
    m_actsModel->setActivitiesController(activitiesController);
}




/*!
 * \property ActivitiesFilterModel::categoriesController
 * \brief Sets the categories controller.
 *
 * \par Access functions:
 * <TABLE><TR><TD>CategoriesController*</TD><TD>getCategoriesController() const</TD></TR><TR><TD>void</TD><TD>setCategoriesController(CategoriesController *categoriesController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link ActivitiesFilterModel::categoriesController categoriesController \endlink property.
 */
CategoriesController *ActivitiesFilterModel::getCategoriesController() const { return m_actsModel->getCategoriesController(); }

/*!
 * \brief Part of the \link ActivitiesFilterModel::categoriesController categoriesController \endlink property.
 */
void ActivitiesFilterModel::setCategoriesController(CategoriesController *categoriesController)
{
    m_actsModel->setCategoriesController(categoriesController);
}



/*!
 * \property ActivitiesFilterModel::recordsController
 * \brief Sets the records controller.
 *
 * \par Access functions:
 * <TABLE><TR><TD>RecordsController*</TD><TD>getRecordsController() const</TD></TR><TR><TD>void</TD><TD>setRecordsController(RecordsController *recordsController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link ActivitiesFilterModel::recordsController recordsController \endlink property.
 */
RecordsController *ActivitiesFilterModel::getRecordsController() const { return m_actsModel->getRecordsController(); }

/*!
 * \brief Part of the \link ActivitiesFilterModel::recordsController recordsController \endlink property.
 */
void ActivitiesFilterModel::setRecordsController(RecordsController *recordsController)
{
   m_actsModel->setRecordsController(recordsController);
}
