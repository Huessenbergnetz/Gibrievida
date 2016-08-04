#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>

namespace Gibrievida {

/*!
 * \brief Manages the initialization of the local SQLite database.
 */
class DBManager : public QThread
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool createDatabase();
    QString m_dbpath;

    Q_DISABLE_COPY(DBManager)
};

}

#endif // DBMANAGER_H
