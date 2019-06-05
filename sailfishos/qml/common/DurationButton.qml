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
    id: durationButton

    property int duration: 0

    property int days: 0
    property int hours: 0
    property int minutes: 0
    property int seconds: 0

    function setDurationParts() {
        if (duration > 0) {
            var secs = duration

            var fDays = secs/86400
            days = Math.floor(fDays)

            secs = secs - (86400*days)

            var fHours = secs/3600
            hours = Math.floor(fHours)

            secs = secs - (3600*hours)

            var fMins = secs/60
            minutes = Math.floor(fMins)

            seconds = secs - (60*minutes)

            value = helpers.createDurationString(duration)
        }
    }

    Component.onCompleted: setDurationParts()

    onClicked: {
        var dialog = pageStack.push(durationDialogComp, {days: days, hours: hours, minutes: minutes, seconds: seconds})
        dialog.accepted.connect(function() {
            days = dialog.days
            hours = dialog.hours
            minutes = dialog.minutes
            seconds = dialog.seconds
            duration = dialog.duration
            value = dialog.durationString
        })
    }

    Component {
        id: durationDialogComp

        Dialog {
            id: durationDialog

            property int days
            property int hours
            property int minutes
            property int seconds
            property int duration
            property string durationString

            onDaysChanged: calcDuration()
            onHoursChanged: calcDuration()
            onMinutesChanged: calcDuration()
            onSecondsChanged: calcDuration()

            function calcDuration() {
                duration = seconds + (minutes * 60) + (hours * 3600) + (days * 86400)
                durationString = helpers.createDurationString(duration)
            }

            SilicaFlickable {
                id: durationDialogFlick
                anchors.fill: parent

                Column {
                    id: durationDialogCol
                    width: parent.width
                    spacing: Theme.paddingMedium

                    DialogHeader {
                        defaultAcceptText: qsTr("Select duration")
                        acceptText: durationString
                        dialog: durationDialog
                        flickable: durationDialogFlick
                    }

                    TextField {
                        id: daysInput
                        label: qsTr("Days"); placeholderText: label
                        text: days
                        width: parent.width/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        EnterKey.iconSource: "image://theme/icon-m-enter-next"
                        EnterKey.onClicked: hoursInput.focus = true
                        onTextChanged: if (acceptableInput) { days = parseInt(text) }
                    }

                    TextField {
                        id: hoursInput
                        label: qsTr("Hours"); placeholderText: label
                        text: hours
                        width: parent.width/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        EnterKey.iconSource: "image://theme/icon-m-enter-next"
                        EnterKey.onClicked: minutesInput.focus = true
                        onTextChanged: if (acceptableInput) { hours = parseInt(text) }
                    }

                    TextField {
                        id: minutesInput
                        label: qsTr("Minutes"); placeholderText: label
                        text: minutes
                        width: parent.width/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        EnterKey.iconSource: "image://theme/icon-m-enter-next"
                        EnterKey.onClicked: secondsInput.focus = true
                        onTextChanged: if (acceptableInput) { minutes = parseInt(text) }
                    }

                    TextField {
                        id: secondsInput
                        label: qsTr("Seconds"); placeholderText: label
                        text: seconds
                        width: parent.width/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        EnterKey.iconSource: "image://theme/icon-m-enter-close"
                        EnterKey.onClicked: secondsInput.focus = false
                        onTextChanged: if (acceptableInput) { seconds = parseInt(text) }
                    }
                }
            }
        }
    }
}

