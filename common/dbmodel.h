#ifndef DBMODEL_H
#define DBMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSqlDatabase>

namespace Gibrievida {

class DBModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ isInOperation WRITE setInOperation NOTIFY inOperationChanged)
public:
    explicit DBModel(QObject *parent = nullptr);
    ~DBModel();

    bool isInOperation() const;

protected:
    bool connectDb();
    QSqlDatabase m_db;
    void setInOperation(bool inOperation);

signals:
    void inOperationChanged(bool inOperation);

private:
    Q_DISABLE_COPY(DBModel)
    bool m_inOperation;
};

}

#endif // DBMODEL_H
