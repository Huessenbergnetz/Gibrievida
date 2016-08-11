#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include "basecontroller.h"

namespace Gibrievida {

class RecordsController : public BaseController
{
    Q_OBJECT
    Q_PROPERTY(int currentId READ getCurrentId NOTIFY currentIdChanged)
    Q_PROPERTY(int currentActivityId READ getCurrentActivityId NOTIFY currentActivityIdChanged)
    Q_PROPERTY(QString currentActivityName READ getCurrentActivityName NOTIFY currentActivityNameChanged)
    Q_PROPERTY(int currentCategoryId READ getCurrentCategoryId NOTIFY currentCategoryIdChanged)
    Q_PROPERTY(QString currentCategoryName READ getCurrentCategoryName NOTIFY currentCategoryNameChanged)
    Q_PROPERTY(QString currentCategoryColor READ getCurrentCategoryColor NOTIFY currentCategoryColorChanged)
    Q_PROPERTY(QDateTime currentStartTime READ getCurrentStartTime NOTIFY currentStartTimeChanged)
    Q_PROPERTY(int currentRepetitions READ getCurrentRepetitions NOTIFY currentRepetitionsChanged)
    Q_PROPERTY(float currentDistance READ getCurrentDistance NOTIFY currentDistanceChanged)
public:
    explicit RecordsController(QObject *parent = nullptr);
    ~RecordsController();

    int add(int activity);
    void finish(int databaseId, int repetitions = 0, float distance = 0.0, QString note = QString());

    int getCurrentId() const;
    int getCurrentActivityId() const;
    QString getCurrentActivityName() const;
    int getCurrentCategoryId() const;
    QString getCurrentCategoryName() const;
    QString getCurrentCategoryColor() const;
    QDateTime getCurrentStartTime() const;
    int getCurrentRepetitions() const;
    float getCurrentDistance() const;

signals:
    void added(int databaseId, int activity, const QDateTime &start);

    void currentIdChanged(int currentId);
    void currentActivityIdChanged(int currentActivityId);
    void currentActivityNameChanged(const QString &currentActivityName);
    void currentCategoryIdChanged(int currentCategoryId);
    void currentCategoryNameChanged(const QString &currentCategoryName);
    void currentCategoryColorChanged(const QString &currentCategoryColor);
    void currentStartTimeChanged(const QDateTime &currentStartTime);
    void currentRepetitionsChanged(int currentRepetitions);
    void currentDistanceChanged(float currentDistance);

private:
    Q_DISABLE_COPY(RecordsController)

    void setCurrentId(int currentId);
    void setCurrentActivityId(int currentActivityId);
    void setCurrentActivityName(const QString &currentActivityName);
    void setCurrentCategoryId(int currentCategoryId);
    void setCurrentCategoryName(const QString &currentCategoryName);
    void setCurrentCategoryColor(const QString &currentCategoryColor);
    void setCurrentStartTime(const QDateTime &currentStartTime);
    void setCurrentRepetitions(int currentRepetitions);
    void setCurrentDistance(float currentDistance);


    int m_currentId;
    int m_currentActivityId;
    QString m_currentActivityName;
    int m_currentCategoryId;
    QString m_currentCategoryName;
    QString m_currentCategoryColor;
    QDateTime m_currentStartTime;
    int m_currentRepetitions;
    float m_currentDistance;
};

}

#endif // RECORDSCONTROLLER_H
