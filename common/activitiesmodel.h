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

#ifndef ACTIVITIESMODEL_H
#define ACTIVITIESMODEL_H

#include <QObject>
#include "dbmodel.h"

namespace Gibrievida {

class ActivitiesController;
class CategoriesController;
class RecordsController;
class Category;
class Activity;
class Record;

/*!
 * \brief Model containing a set of Activity objects.
 *
 * On cunstruction, the model will load all available data from the SQL database, no further actions are needed.
 */
class ActivitiesModel : public DBModel
{
    Q_OBJECT
public:
    explicit ActivitiesModel(QObject *parent = nullptr);
    ~ActivitiesModel();

    /*!
     * \brief The model roles.
     *
     * Use the enumeration name starting lowercase to access the role from QML.
     */
    enum Roles {
        Item = Qt::UserRole + 1 /*!< The main model item. Pointer to an Activity object. */
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    void setCategoriesController(CategoriesController *controller);
    CategoriesController *getCategoriesController() const;

    void setActivitiesController(ActivitiesController *controller);
    ActivitiesController *getActivitiesController() const;

    void setRecordsController(RecordsController *controller);
    RecordsController *getRecordsController() const;

public slots:
    void add(int databaseId, const QString &name, Category *c, int minRepeats, int maxRepeats, bool distance, int sensorType, int sensorDelay);
    void remove(int databaseId, int category);
    void removeAll();

    void updateCategory(Category *c);
    void removeCategory(int category);

    void recordRemoved(int record, int activity, int category);
    void recordsRemovedByActivity(int activity, int category);
    void recordsRemovedByCategory(int category);
    void recordFinished(Record *r);

private:
    QList<Activity*> m_activities;

    void init();
    void clear();

    int find(int databaseId);

    ActivitiesController *m_actsController;
    CategoriesController *m_catsController;
    RecordsController *m_recsController;

    Q_DISABLE_COPY(ActivitiesModel)
};

}

#endif // ACTIVITIESMODEL_H
