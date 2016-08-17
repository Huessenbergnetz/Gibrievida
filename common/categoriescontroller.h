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

#ifndef CATEGORIESCONTROLLER_H
#define CATEGORIESCONTROLLER_H

#include <QObject>
#include "basecontroller.h"

namespace Gibrievida {

class Category;

class CategoriesController : public BaseController
{
    Q_OBJECT
public:
    explicit CategoriesController(QObject *parent = nullptr);
    ~CategoriesController();

    Q_INVOKABLE int add(const QString &name, const QString &color);
    Q_INVOKABLE bool update(Gibrievida::Category *c);
    Q_INVOKABLE bool remove(Gibrievida::Category *c);
    Q_INVOKABLE bool removeAll();
    Q_INVOKABLE QString createRandomColor();

signals:
    void added(int databaseId, const QString &name, const QString &color);
    void updated(Category *c);
    void removed(int databaseId);
    void removedAll();

private:
    QString m_colorParts;
    Q_DISABLE_COPY(CategoriesController)
};

}

#endif // CATEGORIESCONTROLLER_H
