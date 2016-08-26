#ifndef DISTANCEMEASUREMENT_H
#define DISTANCEMEASUREMENT_H

#include <QObject>
#include <QGeoPositionInfo>

class QGeoPositionInfoSource;
class QGeoSatelliteInfoSource;
class QGeoSatelliteInfo;
class QTimer;

namespace Gibrievida {

class DistanceMeasurement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int satellitesInView READ satellitesInView NOTIFY satellitesInViewChanged)
    Q_PROPERTY(int satellitesInUse READ satellitesInUse NOTIFY satellitesInUseChanged)
    Q_PROPERTY(bool initialPositionAvailable READ isInitialPositionAvailable NOTIFY initialPositionAvailableChanged)
    Q_PROPERTY(qreal accuracy READ accuracy NOTIFY accuracyChanged)
    Q_PROPERTY(int lastUpdate READ lastUpdate NOTIFY lastUpdateChanged)
public:
    explicit DistanceMeasurement(int updateInterval = 10000, QObject *parent = nullptr);
    ~DistanceMeasurement();

    int satellitesInView() const;
    int satellitesInUse() const;
    bool isInitialPositionAvailable() const;
    qreal accuracy() const;
    int lastUpdate() const;

signals:
    void satellitesInViewChanged(int satellitesInView);
    void satellitesInUseChanged(int satellitesInUse);
    void gotDistance(double distance);
    void gotSpeed(qreal speed);
    void initialPositionAvailableChanged(bool initialPositionAvailable);
    void accuracyChanged(qreal accuracy);
    void lastUpdateChanged(int lastUpdate);
    void positionSignalLost();

private slots:
    void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites);
    void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &satellites);
    void positionUpdated(const QGeoPositionInfo &update);

private:
    Q_DISABLE_COPY(DistanceMeasurement)

    void setSatellitesInView(int nSatellitesInView);
    void setSatellitesInUse(int nSatellitesInUse);
    void setInitialPositionAvailable(bool initialPositionAvailable);
    void setAccuracy(qreal nAccuracy);
    void setLastUpdate(int nLastUpdate);


    QGeoPositionInfoSource *m_positionSource;
    QGeoSatelliteInfoSource *m_satelliteSource;
    QTimer *m_signalLostTimer;

    int m_updateInterval;
    int m_satellitesInView;
    int m_satellitesInUse;
    bool m_initialPositionAvailable;
    double m_accuracy;
    int m_lastUpdate;

    QGeoPositionInfo m_lastPosition;
    QGeoPositionInfo m_lastUsedPosition;
};

}

#endif // DISTANCEMEASUREMENT_H
