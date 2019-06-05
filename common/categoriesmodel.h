/*
    Gibrievida - An activity tracker
    Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
    https://github.com/Huessenbergnetz/Gibrievida

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

#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QObject>
#include "dbmodel.h"

namespace Gibrievida {

class CategoriesController;
class ActivitiesController;
class Category;
class Activity;

/*!
 * \brief Model containing a set of Category objects.
 *
 * The model will be populated on construction from the SQL database.
 */
class CategoriesModel : public DBModel
{
    Q_OBJECT
public:
    explicit CategoriesModel(QObject *parent = nullptr);
    ~CategoriesModel();

    /*!
     * \brief The model roles.
     *
     * Use the enumeration name starting lowercase to access the role from QML.
     */
    enum Roles {
        Item = Qt::UserRole + 1 /*!< The main model item. Pointer to a Category object. */
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    void setCategoriesController(CategoriesController *controller);
    CategoriesController *getCategoriesController() const;

    void setActivitiesController(ActivitiesController *controller);
    ActivitiesController *getActivitiesController() const;

public slots:
    void add(int databaseId, const QString &name, const QString &color);
    void remove(int databaseId);
    void removeAll();

    void addActivity(int databaseId, const QString &name, Category *c, int minRepeats, int maxRepeats, bool useDistance);
    void updateActivity(Activity *a, int oldCategoryId);
    void removeActivity(int databaseId, int category);
    void removeAllActivities();

private:
    QList<Category*> m_categories;
    void init();
    void clear();
    int find(int databaseId);
    CategoriesController *m_controller;
    ActivitiesController *m_actsController;

    Q_DISABLE_COPY(CategoriesModel)
};

}

#endif // CATEGORIESMODEL_H
