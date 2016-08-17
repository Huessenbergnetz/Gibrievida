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

#include "categoriesfiltermodel.h"
#include "categoriesmodel.h"
#include "category.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new categories filter model.
 *
 * This will also create the underlying CategoriesModel.
 */
CategoriesFilterModel::CategoriesFilterModel(QObject *parent) : FilterModel(parent)
{
    m_catsModel = new CategoriesModel(this);
    connect(m_catsModel, &DBModel::inOperationChanged, this, &FilterModel::setInOperation);
    setSourceModel(m_catsModel);
    setSortLocaleAware(true);
    setSortRole(CategoriesModel::Item);
    sort(0);
}


/*!
 * \brief Destroys the categories filter model.
 *
 * This will also destroy the underlying CategoriesModel.
 */
CategoriesFilterModel::~CategoriesFilterModel()
{

}


/*!
 * \brief Reimplemented from QSortFilterProxyModel.
 */
bool CategoriesFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Category *l = sourceModel()->data(left, CategoriesModel::Item).value<Category*>();
    Category *r = sourceModel()->data(right, CategoriesModel::Item).value<Category*>();

    return QString::localeAwareCompare(l->name(), r->name()) < 0;
}



/*!
 * \brief Reimplemented from QSortFilterProxyModel.
 */
bool CategoriesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);

    return sourceModel()->data(idx, CategoriesModel::Item).value<Category*>()->name().contains(filterRegExp());
}



/*!
 * \property CategoriesFilterModel::controller
 * \brief Sets the CategoriesController to perfrom the database  operations.
 *
 * \par Access functions:
 * <TABLE><TR><TD>CategoriesController*</TD><TD>getController() const</TD></TR><TR><TD>void</TD><TD>setController(CategoriesController *controller)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link CategoriesFilterModel::controller controller \endlink property.
 */
CategoriesController *CategoriesFilterModel::getController() const { return m_catsModel->getCategoriesController(); }

/*!
 * \brief Part of the \link CategoriesFilterModel::controller controller \endlink property.
 */
void CategoriesFilterModel::setController(CategoriesController *controller)
{
    m_catsModel->setCategoriesController(controller);
}



/*!
 * \property CategoriesFilterModel::activitiesController
 * \brief Sets the activities contoller to update the model on activity cahnges.
 *
 * \par Access functions:
 * <TABLE><TR><TD>ActivitiesController*</TD><TD>getActivitiesController() const</TD></TR><TR><TD>void</TD><TD>setActivitiesController(ActivitiesController *activitiesController)</TD></TR></TABLE>
 */

/*!
 * \brief Part of the \link CategoriesFilterModel::activitiesController activitiesController \endlink property.
 */
ActivitiesController *CategoriesFilterModel::getActivitiesController() const { return m_catsModel->getActivitiesController(); }

/*!
 * \brief Part of the \link CategoriesFilterModel::activitiesController activitiesController \endlink property.
 */
void CategoriesFilterModel::setActivitiesController(ActivitiesController *activitiesController)
{
   m_catsModel->setActivitiesController(activitiesController);
}
