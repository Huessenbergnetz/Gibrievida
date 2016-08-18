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

#ifndef DBMODEL_H
#define DBMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSqlDatabase>

namespace Gibrievida {

/*!
 * \brief Base model class for all database models.
 *
 * This class provides methods to connect to the SQL database as well as indicating busy models.
 */
class DBModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ isInOperation WRITE setInOperation NOTIFY inOperationChanged)
public:
    explicit DBModel(QObject *parent = nullptr);
    ~DBModel();

    bool isInOperation() const;

protected:
    bool connectDb();
    QSqlDatabase m_db;
    void setInOperation(bool inOperation);

signals:
    void inOperationChanged(bool inOperation);

private:
    Q_DISABLE_COPY(DBModel)
    bool m_inOperation;
};

}

#endif // DBMODEL_H
