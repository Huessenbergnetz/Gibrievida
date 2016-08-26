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

#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QDateTime>

namespace Gibrievida {

class Activity;

/*!
 * \brief Represents a single data record.
 *
 * A Record is a data set that represents an Activity at a specific \link Record::start start \endlink time together
 * with the \link Record::duration duration \endlink of the activity. It is active as long as no \link Record::end end \endlink time has been set.
 *
 * Records are managed by the RecordsController and accessible through the RecordsModel.
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
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)
    Q_PROPERTY(float tpr READ tpr NOTIFY tprChanged)
    Q_PROPERTY(float maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(float avgSpeed READ avgSpeed WRITE setAvgSpeed NOTIFY avgSpeedChanged)
public:
    explicit Record(QObject *parent = nullptr);
    explicit Record(int databaseId, const QDateTime &start, const QDateTime &end, uint duration, uint repetitions, double distance, const QString &note, float tpr, float maxSpeed, float avgSpeed, QObject *parent = nullptr);
    ~Record();

    int databaseId() const;
    Activity *activity() const;
    QDateTime start() const;
    QDateTime end() const;
    uint duration() const;
    uint repetitions() const;
    double distance() const;
    QString note() const;
    bool isActive() const;
    float tpr() const;
    float maxSpeed() const;
    float avgSpeed() const;

    void setDatabaseId(int nDatabaseId);
    void setActivity(Activity *nActivity);
    void setStart(const QDateTime &nStart);
    void setEnd(const QDateTime &nEnd);
    void setDuration(uint nDuration);
    void setRepetitions(uint nRepetitions);
    void setDistance(double nDistance);
    void setNote(const QString &nNote);
    void setTpr(float nTpr);
    void setMaxSpeed(float nMaxSpeed);
    void setAvgSpeed(float nAvgSpeed);

    Q_INVOKABLE bool isValid() const;
    Q_INVOKABLE void updateDuration(uint nDuration);
    Q_INVOKABLE void updateRepetitions(uint nRepetitions);
    Q_INVOKABLE void updateActivity(Gibrievida::Activity *activity);
    Q_INVOKABLE void updateDistance(double nDistance);
    Q_INVOKABLE void remove();

public slots:
    void addDistance(double dist);


signals:
    void databaseIdChanged(int databaseId);
    void activityChanged(Activity *activity);
    void startChanged(const QDateTime &start);
    void endChanged(const QDateTime &end);
    void durationChanged(uint duration);
    void repetitionsChanged(uint repetitions);
    void distanceChanged(double distance);
    void noteChanged(const QString &note);
    void activeChanged(bool active);
    void tprChanged(float tpr);
    void maxSpeedChanged(float maxSpeed);
    void avgSpeedChanged(float avgSpeed);
    /*!
     * \brief Emitted by remove() to indicate that the user wants to delete the record.
     */
    void startRemoving();

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
    bool m_active;
    float m_tpr;
    float m_maxSpeed;
    float m_avgSpeed;

    void setActive(bool active);

};

}

#endif // RECORD_H
