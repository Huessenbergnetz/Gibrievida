#ifndef CATEGORIESCONTROLLER_H
#define CATEGORIESCONTROLLER_H

#include <QObject>
#include "basecontroller.h"

namespace Gibrievida {

class CategoriesController : public BaseController
{
    Q_OBJECT
public:
    explicit CategoriesController(QObject *parent = nullptr);
    ~CategoriesController();

    Q_INVOKABLE int add(const QString &name, const QString &color);
    Q_INVOKABLE bool edit(int databaseId, const QString &name, const QString &color);
    Q_INVOKABLE bool remove(int databaseId);
    Q_INVOKABLE bool removeAll();
    Q_INVOKABLE QString createRandomColor();

signals:
    void added(int databaseId, const QString &name, const QString &color);
    void edited(int databaseId, const QString &name, const QString &color);
    void removed(int databaseId);
    void removedAll();

private:
    QString m_colorParts;
    Q_DISABLE_COPY(CategoriesController)
};

}

#endif // CATEGORIESCONTROLLER_H
