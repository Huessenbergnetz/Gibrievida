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

/*!
 * \brief Controller class to manage Record objects.
 *
 * This controller is used to manage single Record object like preparing new records, update active and finished records
 * as well as delete records.
 *
 * The workflow to create new records should be prepare(), add(), finish(). Active records can be canceled via cancel() method.
 */
class RecordsController : public BaseController
{
    Q_OBJECT
    Q_PROPERTY(Gibrievida::Record *current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
public:
    explicit RecordsController(QObject *parent = nullptr);
    ~RecordsController();

    Q_INVOKABLE void prepare();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE int add(Gibrievida::Activity *activity, const QString &note = QString());
    Q_INVOKABLE void finish();
    Q_INVOKABLE void update(Gibrievida::Record *r, int oldActivityId);
    Q_INVOKABLE void remove(Gibrievida::Record *r);
    Q_INVOKABLE void removeByActivity(Gibrievida::Activity *a);
    Q_INVOKABLE void removeAll();

    Record *current() const;
    bool isVisible() const;

    void setVisible(bool visible);

    Q_INVOKABLE void increaseRepetitions();
    Q_INVOKABLE void decreaseRepetitions();

signals:
    /*!
     * \brief Emitted if a Record has been finished successfully and it's data has been written to database.
     */
    void finished(Record *record);
    /*!
     * \brief Emitted if a Record has been updated successfully in the database.
     */
    void updated(Record *record, int oldActivityId);
    /*!
     * \brief Emitted if a Record has been removed successfully from the database.
     */
    void removed(int databaseId, int activity, int category);
    /*!
     * \brief Emitted if a set of records has been removed successfully from the database.
     */
    void removedByActivity(int activity, int category);
    /*!
     * \brief Emitted if all records have been removed successfully from the database.
     */
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
