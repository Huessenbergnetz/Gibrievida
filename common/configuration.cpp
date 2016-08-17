#include "configuration.h"
#include <QDebug>

using namespace Gibrievida;

/*!
 * \brief Constructs a new configuration object.
 */
Configuration::Configuration(QObject *parent) : QSettings(parent)
{

}


/*!
 * \brief Destroys the configuration object.
 */
Configuration::~Configuration()
{

}

