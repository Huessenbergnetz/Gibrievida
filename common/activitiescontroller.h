/*
    Gibrievida - An activity tracker
    Copyright (C) 2016 Buschtrommel/Matthias Fehring http://gibrievida.buschmann23.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
