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
