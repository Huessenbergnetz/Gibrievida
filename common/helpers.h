#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>

namespace Gibrievida {

class Helpers : public QObject
{
    Q_OBJECT
public:
    explicit Helpers(QObject *parent = nullptr);
    ~Helpers();

    Q_INVOKABLE QString createDurationString(uint duration);
    Q_INVOKABLE QString relativeTimeString(const QDateTime &time);

private:
    Q_DISABLE_COPY(Helpers)
};

}

#endif // HELPERS_H
