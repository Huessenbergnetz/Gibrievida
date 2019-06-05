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

#ifndef BACKUPMODEL_H
#define BACKUPMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDateTime>
#include <QDir>
#include <QLocale>

namespace Gibrievida {

/*!
 * \brief Contains information about a single backup.
 */
struct Backup {
    QDateTime time; /**< The time the backup has been created. */
    QString path; /**< Full path to the backup file. */
    QString size; /**< Size of the database backup file. */
};


/*!
 * \brief Contains database backup file data.
 *
 * This model contains data about database backup files created by the user. It also provides methods
 * to create, restore and delete database backup files.
 *
 * Backup files are stored in the database folder and are following a specific naming convention:
 * \c YYYYMMDDHHMMSS.backup.sqlite
 */
class BackupModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ inOperation WRITE setInOperation NOTIFY inOperationChanged)
public:
    explicit BackupModel(QObject *parent = nullptr);
    ~BackupModel();

    /*!
     * \brief The model roles.
     *
     * For access from QML, use the enum name starting lowercase.
     */
    enum Roles {
        Time = Qt::UserRole + 1,    /**< The time the backup hase been created. */
        Path,                       /**< The full path to the backup file. */
        Size                        /**< Size of the database backup file. */
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QVariant data(const QModelIndex &index = QModelIndex(), int role = Qt::UserRole) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    Q_INVOKABLE void create();
    Q_INVOKABLE void restore(int index);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void removeAll();

    bool inOperation() const;
    void setInOperation(bool nInOperation);

signals:
    void inOperationChanged(bool inOperation);

private:
    Q_DISABLE_COPY(BackupModel)

    QList<Backup*> m_backups;
    bool m_inOperation;

    QDir m_dbdir;
    QLocale m_locale;

    void init();
    void clear();

};

}

#endif // BACKUPMODEL_H
