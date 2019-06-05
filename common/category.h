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

#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>

namespace Gibrievida {

/*!
 * \brief Stores information about a single Category.
 *
 * A Category is the main root for every Activity and Record. Activities belong to categories and records belong to activities.
 * A Category is defined by a \link Category::name name \endlink and a \link Category::color color \endlink.
 *
 * Categories are managed by the CategoriesController aber accessible through the CategoriesFilterModel.
 */
class Category : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int databaseId READ databaseId WRITE setDatabaseId NOTIFY databaseIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int activities READ activities NOTIFY activitiesChanged)
public:
    explicit Category(QObject *parent = nullptr);
    explicit Category(int databaseId, const QString &name, const QString &color, int activities, QObject *parent = nullptr);
    explicit Category(Category *other, QObject *parent = nullptr);
    ~Category();

    int databaseId() const;
    QString name() const;
    QString color() const;
    int activities() const;

    void setDatabaseId(int nDatabaseId);
    void setName(const QString &nName);
    void setColor(const QString &nColor);
    void setActivities(int nActivities);

    Q_INVOKABLE bool isValid() const;
    void increaseActivities();
    void decreaseActivities();


signals:
    void databaseIdChanged(int databaseId);
    void nameChanged(const QString &name);
    void colorChanged(const QString &color);
    void activitiesChanged(int activities);

private:
    Q_DISABLE_COPY(Category)
    int m_databaseId;
    QString m_name;
    QString m_color;
    int m_activities;

};

}

#endif // CATEGORY_H
