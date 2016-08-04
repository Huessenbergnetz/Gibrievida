#include "basecontroller.h"
#include <QStandardPaths>
#include "globals.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new base controller.
 */
BaseController::BaseController(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::database();
}


/*!
 * \brief Destroys the base controller.
 */
BaseController::~BaseController()
{

}


/*!
 * \brief Establish database connection.
 *
 * Returns true if the database connection has been established successfull, otherwise returns false.
 */
bool BaseController::connectDb()
{
    if (m_db.isValid() && m_db.isOpen()) {
        return true;
    }

    if (!m_db.isValid()) {

        QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

        if (dirs.isEmpty()) {
            return false;
        }

        QString dbPath = dirs.first();
        dbPath.append(QLatin1String("/")).append(QLatin1String(APP_NAME)).append(QStringLiteral("/database.sqlite"));

        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
        m_db.setDatabaseName(dbPath);

        if (m_db.open()) {
            return true;
        } else {
            return false;
        }

    } else if (!m_db.isOpen()) {

        if (m_db.open()) {
            return true;
        } else {
            return false;
        }

    }

    return false;
}
