#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDateTime>

namespace Gibrievida {

class Activity;

/*!
 * \brief The Record class.
 */
class Record : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int databaseId READ databaseId NOTIFY databaseIdChanged)
    Q_PROPERTY(Gibrievida::Activity *activity READ activity NOTIFY activityChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(uint duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(uint repetitions READ repetitions WRITE setRepetitions NOTIFY repetitionsChanged)
    Q_PROPERTY(double distance READ distance WRITE setDistance NOTIFY distanceChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
public:
    explicit Record(QObject *parent = nullptr);
    explicit Record(int databaseId, const QDateTime &start, const QDateTime &end, uint duration, uint repetitions, double distance, const QString &note, QObject *parent = nullptr);
    ~Record();

    int databaseId() const;
    Activity *activity() const;
    QDateTime start() const;
    QDateTime end() const;
    uint duration() const;
    uint repetitions() const;
    double distance() const;
    QString note() const;

    void setDatabaseId(int nDatabaseId);
    void setActivity(Activity *nActivity);
    void setStart(const QDateTime &nStart);
    void setEnd(const QDateTime &nEnd);
    void setDuration(uint nDuration);
    void setRepetitions(uint nRepetitions);
    void setDistance(double nDistance);
    void setNote(const QString &nNote);

    bool isValid() const;


signals:
    void databaseIdChanged(int databaseId);
    void activityChanged(Activity *activity);
    void startChanged(const QDateTime &start);
    void endChanged(const QDateTime &end);
    void durationChanged(uint duration);
    void repetitionsChanged(uint repetitions);
    void distanceChanged(double distance);
    void noteChanged(const QString &note);

private:
    Q_DISABLE_COPY(Record)
    int m_databaseId;
    Activity *m_activity;
    QDateTime m_start;
    QDateTime m_end;
    uint m_duration;
    uint m_repetitions;
    double m_distance;
    QString m_note;

};

}

#endif // RECORD_H
