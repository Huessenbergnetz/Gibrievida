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

#include "filtermodel.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new filter model.
 */
FilterModel::FilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_inOperation = false;
}


/*!
 * \brief Destroys the filter model.
 */
FilterModel::~FilterModel()
{

}



/*!
 * \property FilterModel::inOperation
 * \brief Returns true while the underlying model is in operation.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>isInOperation() const</TD></TR><TR><TD>void</TD><TD>setInOperation(bool inOperation)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
 */

/*!
 * \fn void FilterModel::inOperationChanged(bool inOperation)
 * \brief Part of the \link FilterModel::inOperation inOperation \endlink property.
 */

/*!
 * \brief Part of the \link FilterModel::inOperation inOperation \endlink property.
 */
bool FilterModel::isInOperation() const { return m_inOperation; }

/*!
 * \brief Part of the \link FilterModel::inOperation inOperation \endlink property.
 */
void FilterModel::setInOperation(bool inOperation)
{
    if (inOperation != m_inOperation) {
        m_inOperation = inOperation;
#ifdef QT_DEBUG
        qDebug() << "Changed inOperation to" << m_inOperation;
#endif
        emit inOperationChanged(isInOperation());
    }
}




/*!
 * \property FilterModel::search
 * \brief The search string to filter the underlying model.
 *
 * \par Access functions:
 * <TABLE><TR><TD>QString</TD><TD>getSearch() const</TD></TR><TR><TD>void</TD><TD>setSearch(const QString &search)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>searchChanged(const QString &search)</TD></TR></TABLE>
 */

/*!
 * \fn void FilterModel::searchChanged(const QString &search)
 * \brief Part of the \link FilterModel::search search \endlink property.
 */

/*!
 * \brief Part of the \link FilterModel::search search \endlink property.
 */
QString FilterModel::getSearch() const { return m_search; }

/*!
 * \brief Part of the \link FilterModel::search search \endlink property.
 */
void FilterModel::setSearch(const QString &search)
{
    if (search != m_search) {
        m_search = search;
#ifdef QT_DEBUG
        qDebug() << "Changed search to" << m_search;
#endif
        setFilterFixedString(getSearch());
        emit searchChanged(getSearch());
    }
}
