#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QSqlDatabase>

namespace Gibrievida {

class BaseController : public QObject
{
    Q_OBJECT
public:
    explicit BaseController(QObject *parent = nullptr);
    ~BaseController();

protected:
    bool connectDb();
    QSqlDatabase m_db;

private:
    Q_DISABLE_COPY(BaseController)
};

}

#endif // BASECONTROLLER_H
