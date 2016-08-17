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
