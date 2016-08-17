#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>

namespace Gibrievida {

/*!
 * \brief Stores information about a single category.
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
