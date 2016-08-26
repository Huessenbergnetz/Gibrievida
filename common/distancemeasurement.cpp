#include "distancemeasurement.h"
#include <QGeoSatelliteInfoSource>
#include <QGeoSatelliteInfo>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <math.h>
#include <QTimer>

#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Gibrievida;

/*!
 * \brief Constructs a new DistanceMeasurement object.
 */
DistanceMeasurement::DistanceMeasurement(int updateInterval, QObject *parent) :
    QObject(parent),
    m_positionSource(QGeoPositionInfoSource::createDefaultSource(this)),
    m_satelliteSource(QGeoSatelliteInfoSource::createDefaultSource(this)),
    m_signalLostTimer(new QTimer(this)),
    m_updateInterval(updateInterval),
    m_satellitesInView(0),
    m_satellitesInUse(0),
    m_initialPositionAvailable(false),
    m_accuracy(0.0),
    m_lastUpdate(-1)
{
#ifdef QT_DEBUG
    qDebug() << "Constructing a new" << this;
#endif

    m_signalLostTimer->setTimerType(Qt::VeryCoarseTimer);
    m_signalLostTimer->setInterval(180000);
    m_signalLostTimer->setSingleShot(false);

    connect(m_signalLostTimer, &QTimer::timeout, this, &DistanceMeasurement::positionSignalLost);

    m_positionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);

    connect(m_satelliteSource, &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &DistanceMeasurement::satellitesInUseUpdated);
    connect(m_satelliteSource, &QGeoSatelliteInfoSource::satellitesInViewUpdated, this, &DistanceMeasurement::satellitesInViewUpdated);

    connect(m_positionSource, &QGeoPositionInfoSource::positionUpdated, this, &DistanceMeasurement::positionUpdated);

    m_positionSource->setUpdateInterval(updateInterval);
    m_satelliteSource->setUpdateInterval(updateInterval);

    m_positionSource->startUpdates();
    m_satelliteSource->startUpdates();
}


/*!
 * \brief Deconstructs the DistanceMeasurement object.
 */
DistanceMeasurement::~DistanceMeasurement()
{
#ifdef QT_DEBUG
    qDebug() << "Destroying" << this;
#endif
}



void DistanceMeasurement::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    setSatellitesInUse(satellites.count());
}




void DistanceMeasurement::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
    setSatellitesInView(satellites.count());
}


void DistanceMeasurement::positionUpdated(const QGeoPositionInfo &update)
{
    if (!update.isValid()) {
        return;
    }

    qreal accuracy = update.attribute(QGeoPositionInfo::HorizontalAccuracy);

    if (isnan(accuracy)) {
        return;
    }

    setAccuracy(accuracy);

    if (accuracy > 15.0) {
#ifdef QT_DEBUG
        qDebug() << "Accuracy not good enough";
#endif
        return;
    }

    if (!m_lastPosition.isValid()) {
#ifdef QT_DEBUG
        qDebug() << "Got initial position";
#endif
        setInitialPositionAvailable(true);

        m_lastUsedPosition = update;
        m_lastPosition = update;
    }

    qreal currentDistance = m_lastPosition.coordinate().distanceTo(update.coordinate());
    qreal distance = m_lastUsedPosition.coordinate().distanceTo(update.coordinate());
    qreal speed = currentDistance / (double)m_lastPosition.timestamp().secsTo(update.timestamp());

    m_lastPosition = update;

#ifdef QT_DEBUG
    qDebug() << "Current distance:" << currentDistance;
#endif

    if (currentDistance < ((qreal)m_updateInterval/1000.0)) {
#ifdef QT_DEBUG
        qDebug() << "Current distance to low";
#endif
        setLastUpdate(m_lastUsedPosition.timestamp().secsTo(QDateTime::currentDateTime()));
        return;
    }

//    if (currentDistance < 3.0) {
//#ifdef QT_DEBUG
//        qDebug() << "Current distance to low";
//#endif
//        return;
//    }

#ifdef QT_DEBUG
    qDebug() << "Distance" << distance;
#endif

    m_lastUsedPosition = update;

    setLastUpdate(m_lastUsedPosition.timestamp().secsTo(QDateTime::currentDateTime()));

    m_signalLostTimer->start();

    emit gotDistance(distance);
    emit gotSpeed(speed);


}



/*!
 * \property DistanceMeasurement::satellitesInView
 * \brief Number of satellites that are currently in view.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>satellitesInView() const</TD></TR><TR><TD>void</TD><TD>setSatellitesInView(int nSatellitesInView)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>satellitesInViewChanged(int satellitesInView)</TD></TR></TABLE>
 */

/*!
 * \fn void DistanceMeasurement::satellitesInViewChanged(int satellitesInView)
 * \brief Part of the \link DistanceMeasurement::satellitesInView satellitesInView \endlink property.
 */

/*!
 * \brief Part of the \link DistanceMeasurement::satellitesInView satellitesInView \endlink property.
 */
int DistanceMeasurement::satellitesInView() const { return m_satellitesInView; }

/*!
 * \brief Part of the \link DistanceMeasurement::satellitesInView satellitesInView \endlink property.
 */
void DistanceMeasurement::setSatellitesInView(int nSatellitesInView)
{
    if (nSatellitesInView != m_satellitesInView) {
        m_satellitesInView = nSatellitesInView;
#ifdef QT_DEBUG
        qDebug() << "Changed satellitesInView to" << m_satellitesInView;
#endif
        emit satellitesInViewChanged(satellitesInView());
    }
}




/*!
 * \property DistanceMeasurement::satellitesInUse
 * \brief Number of satellites that are currently in use.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>satellitesInUse() const</TD></TR><TR><TD>void</TD><TD>setSatellitesInUse(int nSatellitesInUse)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>satellitesInUseChanged(int satellitesInUse)</TD></TR></TABLE>
 */

/*!
 * \fn void DistanceMeasurement::satellitesInUseChanged(int satellitesInUse)
 * \brief Part of the \link DistanceMeasurement::satellitesInUse satellitesInUse \endlink property.
 */

/*!
 * \brief Part of the \link DistanceMeasurement::satellitesInUse satellitesInUse \endlink property.
 */
int DistanceMeasurement::satellitesInUse() const { return m_satellitesInUse; }

/*!
 * \brief Part of the \link DistanceMeasurement::satellitesInUse satellitesInUse \endlink property.
 */
void DistanceMeasurement::setSatellitesInUse(int nSatellitesInUse)
{
    if (nSatellitesInUse != m_satellitesInUse) {
        m_satellitesInUse = nSatellitesInUse;
#ifdef QT_DEBUG
        qDebug() << "Changed satellitesInUse to" << m_satellitesInUse;
#endif
        emit satellitesInUseChanged(satellitesInUse());
    }
}


/*!
 * \property DistanceMeasurement::initialPositionAvailable
 * \brief Returns true if the initial position has been set.
 *
 * \par Access functions:
 * <TABLE><TR><TD>bool</TD><TD>isInitialPositionAvailable() const</TD></TR><TR><TD>void</TD><TD>setInitialPositionAvailable(bool initialPositionAvailable)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>initialPositionAvailableChanged(bool initialPositionAvailable)</TD></TR></TABLE>
 */

/*!
 * \fn void DistanceMeasurement::initialPositionAvailableChanged(bool initialPositionAvailable)
 * \brief Part of the \link DistanceMeasurement::initialPositionAvailable initialPositionAvailable \endlink property.
 */

/*!
 * \brief Part of the \link DistanceMeasurement::initialPositionAvailable initialPositionAvailable \endlink property.
 */
bool DistanceMeasurement::isInitialPositionAvailable() const { return m_initialPositionAvailable; }

/*!
 * \brief Part of the \link DistanceMeasurement::initialPositionAvailable initialPositionAvailable \endlink property.
 */
void DistanceMeasurement::setInitialPositionAvailable(bool initialPositionAvailable)
{
    if (initialPositionAvailable != m_initialPositionAvailable) {
        m_initialPositionAvailable = initialPositionAvailable;
#ifdef QT_DEBUG
        qDebug() << "Changed initialPositionAvailable to" << m_initialPositionAvailable;
#endif
        emit initialPositionAvailableChanged(isInitialPositionAvailable());
    }
}


/*!
 * \property DistanceMeasurement::accuracy
 * \brief Returns the current accuracy of the returned position data.
 *
 * \par Access functions:
 * <TABLE><TR><TD>qreal</TD><TD>accuracy() const</TD></TR><TR><TD>void</TD><TD>setAccuracy(qreal nAccuracy)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>accuracyChanged(qreal accuracy)</TD></TR></TABLE>
 */

/*!
 * \fn void DistanceMeasurement::accuracyChanged(const double &accuracy)
 * \brief Part of the \link DistanceMeasurement::accuracy accuracy \endlink property.
 */

/*!
 * \brief Part of the \link DistanceMeasurement::accuracy accuracy \endlink property.
 */
qreal DistanceMeasurement::accuracy() const { return m_accuracy; }

/*!
 * \brief Part of the \link DistanceMeasurement::accuracy accuracy \endlink property.
 */
void DistanceMeasurement::setAccuracy(qreal nAccuracy)
{
    if (nAccuracy != m_accuracy) {
        m_accuracy = nAccuracy;
#ifdef QT_DEBUG
        qDebug() << "Changed accuracy to" << m_accuracy;
#endif
        emit accuracyChanged(accuracy());
    }
}



/*!
 * \property DistanceMeasurement::lastUpdate
 * \brief Seconds gone since the last valid position update.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>lastUpdate() const</TD></TR><TR><TD>void</TD><TD>setLastUpdate(int nLastUpdate)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>lastUpdateChanged(int lastUpdate)</TD></TR></TABLE>
 */

/*!
 * \fn void DistanceMeasurement::lastUpdateChanged(int lastUpdate)
 * \brief Part of the \link DistanceMeasurement::lastUpdate lastUpdate \endlink property.
 */

/*!
 * \brief Part of the \link DistanceMeasurement::lastUpdate lastUpdate \endlink property.
 */
int DistanceMeasurement::lastUpdate() const { return m_lastUpdate; }

/*!
 * \brief Part of the \link DistanceMeasurement::lastUpdate lastUpdate \endlink property.
 */
void DistanceMeasurement::setLastUpdate(int nLastUpdate)
{
    if (nLastUpdate != m_lastUpdate) {
        m_lastUpdate = nLastUpdate;
#ifdef QT_DEBUG
        qDebug() << "Changed lastUpdate to" << m_lastUpdate;
#endif
        emit lastUpdateChanged(lastUpdate());
    }
}





