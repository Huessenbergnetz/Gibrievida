#ifndef ACTIVITIESCONTROLLER_H
#define ACTIVITIESCONTROLLER_H

#include <QObject>
#include "basecontroller.h"

namespace Gibrievida {

class Category;
class Activity;

class ActivitiesController : public BaseController
{
    Q_OBJECT
public:
    explicit ActivitiesController(QObject *parent = nullptr);
    ~ActivitiesController();

    Q_INVOKABLE int add(const QString &name, Gibrievida::Category *c, int minRepeats, int maxRepeats, bool useDistance);
    Q_INVOKABLE bool update(Gibrievida::Activity *a, int oldCategoryId);
    Q_INVOKABLE bool remove(Gibrievida::Activity *a);
    Q_INVOKABLE bool removeAll();

signals:
    void added(int databaseId, const QString &name, Category *c, int minRepeats, int maxRepeats, bool useDistance);
    void updated(Activity *a, int oldCategoryId);
    void removed(int databaseId, int category);
    void removedAll();

private:
    Q_DISABLE_COPY(ActivitiesController)
};

}

#endif // ACTIVITIESCONTROLLER_H
