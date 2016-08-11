#include "configuration.h"
#include <QDebug>

using namespace Gibrievida;

/*!
 * \brief Constructs a new configuration object.
 */
Configuration::Configuration(QObject *parent) : QSettings(parent)
{
    m_currentRecording = value(QStringLiteral("recordings/current"), 0).toInt();
}


/*!
 * \brief Destroys the configuration object.
 */
Configuration::~Configuration()
{

}



/*!
 * \property Configuration::currentRecording
 * \brief Stores database id of the current active recording.
 *
 * \par Access functions:
 * <TABLE><TR><TD>int</TD><TD>getCurrentRecording() const</TD></TR><TR><TD>void</TD><TD>setCurrentRecording(int currentRecording)</TD></TR></TABLE>
 * \par Notifier signal:
 * <TABLE><TR><TD>void</TD><TD>currentRecordingChanged(int currentRecording)</TD></TR></TABLE>
 */

/*!
 * \fn void Configuration::currentRecordingChanged(int currentRecording)
 * \brief Part of the \link Configuration::currentRecording currentRecording \endlink property.
 */

/*!
 * \brief Part of the \link Configuration::currentRecording currentRecording \endlink property.
 */
int Configuration::getCurrentRecording() const { return m_currentRecording; }

/*!
 * \brief Part of the \link Configuration::currentRecording currentRecording \endlink property.
 */
void Configuration::setCurrentRecording(int currentRecording)
{
    if (currentRecording != m_currentRecording) {
        m_currentRecording = currentRecording;
        setValue(QStringLiteral("recordings/current"), m_currentRecording);
#ifdef QT_DEBUG
        qDebug() << "Changed currentRecording to" << m_currentRecording;
#endif
        emit currentRecordingChanged(getCurrentRecording());
    }
}
