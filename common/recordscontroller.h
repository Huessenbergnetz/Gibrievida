#ifndef RECORDSCONTROLLER_H
#define RECORDSCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include "basecontroller.h"
#include "helpers.h"

class QTimer;

namespace Gibrievida {

class Record;
class Activity;

class RecordsController : public BaseController
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::Record *current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
public:
    explicit RecordsController(QObject *parent = nullptr);
    ~RecordsController();

    Q_INVOKABLE int add(Gibrievida::Activity *activity, const QString &note = QString());
    Q_INVOKABLE void finish();
    Q_INVOKABLE void remove(Gibrievida::Record *r);
    Q_INVOKABLE void removeByActivity(int activity, int category);
    Q_INVOKABLE void removeAll();

    Record *current() const;
    bool isVisible() const;

    void setVisible(bool visible);

    Q_INVOKABLE void increaseRepetitions();
    Q_INVOKABLE void decreaseRepetitions();

signals:
    void finished(Record *record);
    void removed(int databaseId, int activity, int category);
    void removedByActivity(int activity, int category);
    void removedAll();

    void currentChanged(Record *current);

private slots:
    void updateDuration();

private:
    Q_DISABLE_COPY(RecordsController)

    void setCurrent(Record *nCurrent);

    void init();
    void startStopTimer();

    Record *m_current;
    bool m_visible;

    QTimer *m_timer;

    Helpers helpers;
};

}

#endif // RECORDSCONTROLLER_H
