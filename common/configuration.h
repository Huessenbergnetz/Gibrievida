#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>

namespace Gibrievida {

class Configuration : public QSettings
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = nullptr);
    ~Configuration();

private:
    Q_DISABLE_COPY(Configuration)
};

}

#endif // CONFIGURATION_H
