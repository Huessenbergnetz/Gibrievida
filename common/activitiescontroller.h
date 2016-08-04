#ifndef ACTIVITIESCONTROLLER_H
#define ACTIVITIESCONTROLLER_H

#include <QObject>
#include "basecontroller.h"

namespace Gibrievida {

class ActivitiesController : public BaseController
{
    Q_OBJECT
public:
    explicit ActivitiesController(QObject *parent = nullptr);
    ~ActivitiesController();

    Q_INVOKABLE int add(const QString &name, int category, int minRepeats, int maxRepeats, bool distance);
    Q_INVOKABLE bool edit(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance);
    Q_INVOKABLE bool remove(int databaseId, int category);
    Q_INVOKABLE bool removeAll();

signals:
    void added(int databaseId, const QString &name, int category, int minRepeats, int maxRepeats, bool distance);
    void edited(int databaseId, const QString &name, int oldCategory, int newCategory, int minRepeats, int maxRepeats, bool distance);
    void removed(int databaseId, int category);
    void removedAll();

private:
    Q_DISABLE_COPY(ActivitiesController)
};

}

#endif // ACTIVITIESCONTROLLER_H
