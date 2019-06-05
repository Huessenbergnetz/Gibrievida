/*
    Gibrievida - An activity tracker
    Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
    https://github.com/Huessenbergnetz/Gibrievida

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

import QtQuick 2.2
import Sailfish.Silica 1.0

ValueButton {
    id: timeButton

    property date dateTime
    property date nullTime: new Date(0)
    property bool highlightError: false

    width: parent.width
    value: dateTime.getTime() === 0
           ? qsTr("Select date")
           //: Time format. See: http://doc.qt.io/qt-5/qdatetime.html#toString
           : Qt.formatDateTime(dateTime, qsTr("d MMM yyyy hh:mm"))
    valueColor: highlightError ? "red" : Theme.highlightColor
    onClicked: {
        var dialog = pageStack.push(dateTimeDialogComp, {initialTime: (dateTime.getTime() === 0) ? new Date() : dateTime })
        dialog.accepted.connect(function() { dateTime = dialog.dateTime })
    }

    Component {
        id: dateTimeDialogComp

        Dialog {
            id: dateTimeDialog
            property date initialTime: new Date()
            property date currentDateTime: new Date()
            property int hour
            property int minute
            property int year
            property int month
            property int day
            property date dateTime: new Date(year, month-1, day, hour, minute)

            onOpened: {
                timeChooser.hour = initialTime.getHours()
                timeChooser.minute = initialTime.getMinutes()
            }


            SilicaFlickable {
                anchors.fill: parent
                contentHeight: col.height

                Column {
                    id: col
                    width: parent.width

                    DialogHeader {
                        id: dHeader
                        defaultAcceptText: qsTr("Select date and time")
                        acceptText: dateChooser.dateText + " " + timeChooser.timeText
                    }

                    TimePicker {
                        id: timeChooser
                        anchors.horizontalCenter: parent.horizontalCenter
                        onHourChanged: dateTimeDialog.hour = timeChooser.hour
                        onMinuteChanged: dateTimeDialog.minute = timeChooser.minute
                    }

                    DatePicker {
                        id: dateChooser
                        date: initialTime
                        onYearChanged: dateTimeDialog.year = dateChooser.year
                        onMonthChanged: dateTimeDialog.month = dateChooser.month
                        onDayChanged: dateTimeDialog.day = dateChooser.day
                    }
                }
            }
        }
    }
}

