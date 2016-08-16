#include "activitiesfiltermodel.h"
#include "activitiesmodel.h"

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
    setFilterRole(ActivitiesModel::Name);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);
    setSortRole(ActivitiesModel::Name);
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