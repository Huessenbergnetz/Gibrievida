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
