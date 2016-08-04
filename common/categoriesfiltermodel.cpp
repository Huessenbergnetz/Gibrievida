#include "categoriesfiltermodel.h"
#include "categoriesmodel.h"

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
    setFilterRole(CategoriesModel::Name);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);
    setSortRole(CategoriesModel::Name);
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
