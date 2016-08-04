#include "dbmodel.h"
#include <QStandardPaths>
#include <QSqlError>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

#include "globals.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new database model.
 */
DBModel::DBModel(QObject *parent) : QAbstractListModel(parent)
{
    m_db = QSqlDatabase::database();
    m_inOperation = false;
}


/*!
 * \brief Destroys the database model.
 */
DBModel::~DBModel()
{

}



/*!
 * \property DBModel::inOperation
 * \brief Returns true while the model is operating.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>isInOperation() const</TD></TR><TR><TD>void</TD><TD>setInOperation(bool inOperation)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
 */

/*!
 * \fn void DBModel::inOperationChanged(bool inOperation)
 * \brief Part of the \link DBModel::inOperation inOperation \endlink property.
 */

/*!
 * \brief Part of the \link DBModel::inOperation inOperation \endlink property.
 */
bool DBModel::isInOperation() const { return m_inOperation; }

/*!
 * \brief Part of the \link DBModel::inOperation inOperation \endlink property.
 */
void DBModel::setInOperation(bool inOperation)
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
 * \brief Establish database connection.
 *
 * Returns true if the database connection has been established successfull, otherwise returns false.
 */
bool DBModel::connectDb()
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
