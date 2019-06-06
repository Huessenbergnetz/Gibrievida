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

#include "backupmodel.h"
#include "globals.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QStringList>
#include <QFileInfoList>
#include <QFileInfo>
#include <QCoreApplication>
#include <QStringBuilder>

using namespace Gibrievida;

/*!
 * \brief Constructs a new BackupModel object.
 *
 * The model will be populated with the content of the database folder.
 */
BackupModel::BackupModel(QObject *parent) : QAbstractListModel(parent)
{
#ifdef QT_DEBUG
    qDebug() << "Constructing a new" << this;
#endif

    m_inOperation = false;

    const QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

    if (!dirs.isEmpty()) {
        const QString dbpath = dirs.first() % QLatin1Char('/') % QCoreApplication::instance()->applicationName();
        m_dbdir.setPath(dbpath);
    }

    init();
}


/*!
 * \brief Destroys the BackupModel and clears the model data.
 */
BackupModel::~BackupModel()
{
#ifdef QT_DEBUG
    qDebug() << "Destroying" << this;
#endif

    qDeleteAll(m_backups);
    m_backups.clear();
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QHash<int, QByteArray> BackupModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Time, QByteArrayLiteral("time"));
    roles.insert(Path, QByteArrayLiteral("path"));
    roles.insert(Size, QByteArrayLiteral("size"));
    return roles;
}



/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
int BackupModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_backups.count();
}



/*!
 * \brief Reimplemented from QAbstractListModel.
 */
QModelIndex BackupModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}


/*!
 * \brief Reimplemented from QAbstractItemModel.
 */
QVariant BackupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > (rowCount()-1)) {
        return QVariant();
    }

    Backup *b = m_backups.at(index.row());

    switch (role) {
    case Time:
        return QVariant::fromValue(b->time);
    case Path:
        return QVariant::fromValue(b->path);
    case Size:
        return QVariant::fromValue(b->size);
    default:
        return QVariant();
    }
}



/*!
 * \brief Clears the model data and destroys all model items.
 */
void BackupModel::clear()
{
    if (m_backups.isEmpty()) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    qDeleteAll(m_backups);
    m_backups.clear();

    endRemoveRows();
}


/*!
 * \brief Initializes the model by reading the content of the database directory.
 */
void BackupModel::init()
{
    setInOperation(true);

    clear();

    QFileInfoList backupFiles = m_dbdir.entryInfoList(QStringList({QStringLiteral("*.backup.sqlite")}), QDir::Files|QDir::Readable|QDir::Writable, QDir::Name|QDir::Reversed);

    if (backupFiles.isEmpty()) {
        setInOperation(false);
        return;
    }

    m_backups.reserve(backupFiles.size());

    beginInsertRows(QModelIndex(), 0, m_backups.size()-1);

    Q_FOREACH(const QFileInfo &backup, backupFiles) {
        Backup *b = new Backup;
        b->time = QDateTime::fromString(backup.baseName(), QStringLiteral("yyyyMMddHHmmss"));
        b->path = backup.absoluteFilePath();
        b->size = m_locale.toString((float)backup.size()/1024.0f, 'g', 1).append(QLatin1String(" KiB"));
        m_backups.append(b);
    }

    endInsertRows();

    setInOperation(false);
}



/*!
 * \brief Creates a new backup.
 */
void BackupModel::create()
{
    setInOperation(true);

    QSqlDatabase db = QSqlDatabase::database();
    db.close();

    QFile currentDb(db.databaseName());

    QDateTime time = QDateTime::currentDateTime();
    QString path = m_dbdir.absoluteFilePath(time.toString(QStringLiteral("yyyyMMddHHmmss")).append(QLatin1String(".backup.sqlite")));

    if (currentDb.copy(path)) {

        QFileInfo fi(path);

        beginInsertRows(QModelIndex(), 0, 0);

        Backup *b = new Backup;
        b->time = time;
        b->path = path;
        b->size = m_locale.toString((float)fi.size()/1024.0f, 'g', 1).append(QLatin1String(" KiB"));

        m_backups.prepend(b);

        endInsertRows();

    }

    setInOperation(false);
}



/*!
 * \brief Restores the backup identified by \c index.
 */
void BackupModel::restore(int index)
{
    if (m_backups.isEmpty()) {
        return;
    }

    if (index > (rowCount()-1)) {
        return;
    }

    if (index < 0) {
        return;
    }

    setInOperation(true);

    QSqlDatabase db = QSqlDatabase::database();
    db.close();

    QFile currentDb(db.databaseName());
    QFile backupDb(m_backups.at(index)->path);

    if (currentDb.remove()) {
        backupDb.copy(db.databaseName());
    }

    setInOperation(false);
}



/*!
 * \brief Removes the backup identfied by \c index.
 */
void BackupModel::remove(int index)
{
    if (m_backups.isEmpty()) {
        return;
    }

    if (index > (rowCount()-1)) {
        return;
    }

    if (index < 0) {
        return;
    }

    setInOperation(true);

    QFile file(m_backups.at(index)->path);

    if (file.remove()) {
        beginRemoveRows(QModelIndex(), index, index);

        delete m_backups.takeAt(index);

        endRemoveRows();
    }

    setInOperation(false);
}



/*!
 * \brief Removes all backups.
 */
void BackupModel::removeAll()
{
    if (m_backups.isEmpty()) {
        return;
    }

    setInOperation(true);

    bool failed = false;

    for (int i = 0; i < m_backups.size(); ++i) {

        QFile f(m_backups.at(i)->path);
        if (!f.remove()) {
            failed = true;
        }

    }

    if (failed) {
        init();
    } else {
        clear();
    }

    setInOperation(false);
}



/*!
 * \property BackupModel::inOperation
 * \brief Returns true while the model is in operation.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR><TR><TD>void</TD><TD>setInOperation(bool nInOperation)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
 */

/*!
 * \fn void BackupModel::inOperationChanged(bool inOperation)
 * \brief Part of the \link BackupModel::inOperation inOperation \endlink property.
 */

/*!
 * \brief Part of the \link BackupModel::inOperation inOperation \endlink property.
 */
bool BackupModel::inOperation() const { return m_inOperation; }

/*!
 * \brief Part of the \link BackupModel::inOperation inOperation \endlink property.
 */
void BackupModel::setInOperation(bool nInOperation)
{
    if (nInOperation != m_inOperation) {
        m_inOperation = nInOperation;
#ifdef QT_DEBUG
        qDebug() << "Changed inOperation to" << m_inOperation;
#endif
        emit inOperationChanged(inOperation());
    }
}


