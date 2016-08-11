#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>

namespace Gibrievida {

class Configuration : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int currentRecording READ getCurrentRecording WRITE setCurrentRecording NOTIFY currentRecordingChanged)
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration();

    int getCurrentRecording() const;
    void setCurrentRecording(int currentRecording);

signals:
    void currentRecordingChanged(int currentRecording);

private:
    Q_DISABLE_COPY(Configuration)
    int m_currentRecording;
};

}

#endif // CONFIGURATION_H
