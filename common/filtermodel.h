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

#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

namespace Gibrievida {

/*!
 * \brief Base class for filter proxy models.
 *
 * This implements base functions like busy/inOpeartion state and search interface to QML.
 */
class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ isInOperation NOTIFY inOperationChanged)
    Q_PROPERTY(QString search READ getSearch WRITE setSearch NOTIFY searchChanged)
public:
    explicit FilterModel(QObject *parent = nullptr);
    ~FilterModel();

    bool isInOperation() const;
    QString getSearch() const;

    void setSearch(const QString &search);

public slots:
    void setInOperation(bool inOperation);

signals:
    void inOperationChanged(bool inOperation);
    void searchChanged(const QString &search);

private:
    bool m_inOperation;
    QString m_search;

    Q_DISABLE_COPY(FilterModel)
};

}

#endif // FILTERMODEL_H
