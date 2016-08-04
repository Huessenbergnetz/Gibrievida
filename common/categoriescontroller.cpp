#include "categoriescontroller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

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
 * \brief Edits the category at \a databaseId.
 *
 * Returns true and emits the edited() signal if the operation was successful.
 */
bool CategoriesController::edit(int databaseId, const QString &name, const QString &color)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("UPDATE categories SET name = ?, color = ? WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(name);
    q.addBindValue(color);
    q.addBindValue(databaseId);

    if (!q.exec()) {
        return false;
    }

    emit edited(databaseId, name, color);

    return true;
}



/*!
 * \brief Removes the category at \a databaseId.
 *
 * Returns true and emits the removed() signal if the operation was successful.
 */
bool CategoriesController::remove(int databaseId)
{
    if (!connectDb()) {
        return false;
    }

    QSqlQuery q(m_db);

    if (!q.prepare(QStringLiteral("DELETE FROM categories WHERE id = ?"))) {
        return false;
    }

    q.addBindValue(databaseId);

    if (!q.exec()) {
        return false;
    }

    emit removed(databaseId);

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
