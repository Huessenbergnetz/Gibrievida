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

#include "categoriescontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include "category.h"

using namespace Gibrievida;

/*!
 * \brief Constructs a new categories controller.
 */
CategoriesController::CategoriesController(QObject *parent) : BaseController(parent)
{
    m_colorParts = QStringLiteral("0123456789abcdef");
}


/*!
 * \brief Destroys the categories controller.
 */
CategoriesController::~CategoriesController()
{

}


/*!
 * \brief Adds a new category to the database.
 *
 * On success this will return an integery > 0 and emit the added() signal.
 */
int CategoriesController::add(const QString &name, const QString &color)
{
    if (!connectDb()) {
        return -1;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("INSERT INTO categories (name, color) VALUES (?, ?)"))) {
        return -1;
    }

    q.addBindValue(name);
    q.addBindValue(color);

    if (!q.exec()) {
        return -1;
    }

    int id = q.lastInsertId().toInt();

    emit added(id, name, color);

    return id;
}


/*!
 * \brief Updates the category \c in the database.
 *
 * Returns true and emits the updated() signal if the operation was successful.
 */
bool CategoriesController::update(Category *c)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE categories SET name = ?, color = ? WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(c->name());
    q.addBindValue(c->color());
    q.addBindValue(c->databaseId());

    if (!q.exec()) {
        return false;
    }

    emit updated(c);

    return true;
}



/*!
 * \brief Removes the category at \a c from the database.
 *
 * Returns true and emits the removed() signal if the operation was successful.
 */
bool CategoriesController::remove(Category *c)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM categories WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(c->databaseId());

    if (!q.exec()) {
        return false;
    }

    emit removed(c->databaseId());

    return true;
}


/*!
 * \brief Removes all categories from the database.
 *
 * Returns true and emits the removedAll() signal if the operation was successful.
 */
bool CategoriesController::removeAll()
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("DELETE FROM categories"))) {
        return false;
    }

    emit removedAll();

    return true;
}


/*!
 * \brief Returns a random color string.
 */
QString CategoriesController::createRandomColor()
{
    QString randColor(QLatin1String("#"));

    for (int i = 0; i < 6; ++i) {
        int randNumber = qrand() % 16;
        randColor.append(m_colorParts[randNumber]);
    }

    return randColor;
}
