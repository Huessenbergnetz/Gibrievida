#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include "basecontroller.h"
#include "helpers.h"

class QTimer;

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
    Q_PROPERTY(int currentMinRepetitions READ getCurrentMinRepetitions NOTIFY currentMinRepetitionsChanged)
    Q_PROPERTY(int currentMaxRepetitions READ getCurrentMaxRepetitions NOTIFY currentMaxRepetitionsChanged)
    Q_PROPERTY(bool currentDistanceActive READ hasCurrentDistanceActive NOTIFY currentDistanceActiveChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
    Q_PROPERTY(int currentDuration READ getCurrentDuration NOTIFY currentDurationChanged)
    Q_PROPERTY(QString currentDurationString READ getCurrentDurationString NOTIFY currentDurationStringChanged)
public:
    explicit RecordsController(QObject *parent = nullptr);
    ~RecordsController();

    Q_INVOKABLE int add(int activity, const QString &note = QString());
    Q_INVOKABLE void finish();
    Q_INVOKABLE void remove(int databaseId, int activity, int category);
    Q_INVOKABLE void removeByActivity(int activity, int category);
    Q_INVOKABLE void removeAll();

    int getCurrentId() const;
    int getCurrentActivityId() const;
    QString getCurrentActivityName() const;
    int getCurrentCategoryId() const;
    QString getCurrentCategoryName() const;
    QString getCurrentCategoryColor() const;
    QDateTime getCurrentStartTime() const;
    int getCurrentRepetitions() const;
    float getCurrentDistance() const;
    int getCurrentMinRepetitions() const;
    int getCurrentMaxRepetitions() const;
    bool hasCurrentDistanceActive() const;
    bool isVisible() const;
    int getCurrentDuration() const;
    QString getCurrentDurationString() const;

    void setVisible(bool visible);

    Q_INVOKABLE void increaseRepetitions();
    Q_INVOKABLE void decreaseRepetitions();

signals:
    void finished(int databaseId, int activity, int category);
    void removed(int databaseId, int activity, int category);
    void removedByActivity(int activity, int category);
    void removedAll();

    void currentIdChanged(int currentId);
    void currentActivityIdChanged(int currentActivityId);
    void currentActivityNameChanged(const QString &currentActivityName);
    void currentCategoryIdChanged(int currentCategoryId);
    void currentCategoryNameChanged(const QString &currentCategoryName);
    void currentCategoryColorChanged(const QString &currentCategoryColor);
    void currentStartTimeChanged(const QDateTime &currentStartTime);
    void currentRepetitionsChanged(int currentRepetitions);
    void currentDistanceChanged(float currentDistance);
    void currentMinRepetitionsChanged(int currentMinRepetitions);
    void currentMaxRepetitionsChanged(int currentMaxRepetitions);
    void currentDistanceActiveChanged(bool currentDistanceActive);
    void currentDurationChanged(int currentDuration);
    void currentDurationStringChanged(const QString &currentDurationString);

private slots:
    void updateDuration();

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
    void setCurrentMinRepetitions(int currentMinRepetitions);
    void setCurrentMaxRepetitions(int currentMaxRepetitions);
    void setCurrentDistanceActive(bool currentDistanceActive);
    void setCurrentDuration(int currentDuration);
    void setCurrentDurationString(const QString &currentDurationString);

    void init();
    void startStopTimer();


    int m_currentId;
    int m_currentActivityId;
    QString m_currentActivityName;
    int m_currentCategoryId;
    QString m_currentCategoryName;
    QString m_currentCategoryColor;
    QDateTime m_currentStartTime;
    int m_currentRepetitions;
    float m_currentDistance;
    int m_currentMinRepetitions;
    int m_currentMaxRepetitions;
    bool m_currentDistanceActive;
    bool m_visible;
    int m_currentDuration;
    QString m_currentDurationString;

    QTimer *m_timer;

    Helpers helpers;
};

}

#endif // RECORDSCONTROLLER_H
