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

import QtQuick 2.2
import Sailfish.Silica 1.0

CoverBackground {
    anchors.fill: parent

    onStatusChanged: gibrievida.updateVisibility()

    CoverPlaceholder {
        visible: !records.current || records.current.databaseId < 0
        text: qsTr("Gibrievida")
    }

    Column {
        visible: records.current && records.current.databaseId > 0
        anchors { left: parent.left; right: parent.right; top: parent.top; leftMargin: Theme.paddingMedium; topMargin: Theme.paddingMedium; rightMargin: Theme.paddingMedium }
        spacing: Theme.paddingSmall

        Label {
            text: records.current ? records.current.activity.name : ""
            width: parent.width
            truncationMode: TruncationMode.Fade
        }

        Item {
            width: parent.width
            height: cName.height

            Rectangle {
                id: cColor
                anchors { left: parent.left; verticalCenter: cName.verticalCenter }
                width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                color: records.current ? records.current.activity.category.color : "white"
            }

            Text {
                id: cName
                anchors { left: cColor.right; leftMargin: Theme.paddingMedium; top: parent.top; right: parent.right }
                text: records.current ? records.current.activity.category.name : ""
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                elide: Text.ElideRight
            }
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }


        Item {
            width: parent.width
            height: startTime.height

            Image {
                id: startTimeIcon
                source: "image://theme/icon-s-time"
                anchors { left: parent.left; verticalCenter: startTime.verticalCenter }
            }

            Text {
                id: startTime
                anchors { left: startTimeIcon.right; leftMargin: Theme.paddingMedium; top: parent.top; right: parent.right }
                text: records.current ? helpers.relativeTimeString(records.current.start) : ""
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
            }
        }

        Item {
            width: parent.width
            height: duration.height
            visible: records.current && records.current.duration > 0

            Image {
                id: durationIcon
                source: "image://theme/icon-s-duration"
                anchors { left: parent.left; verticalCenter: duration.verticalCenter }
            }

            Text {
                id: duration
                anchors { left: durationIcon.right; leftMargin: Theme.paddingMedium; top: parent.top; right: parent.right }
                text: records.current ? helpers.createDurationString(records.current.duration) : ""
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
            }
        }


        Item {
            width: parent.width
            height: repetitions.height
            visible: records.current && records.current.repetitions > 0

            Image {
                id: repetitionsIcon
                source: "image://theme/icon-s-retweet"
                anchors { left: parent.left; verticalCenter: repetitions.verticalCenter }
            }

            Text {
                id: repetitions
                anchors { left: repetitionsIcon.right; leftMargin: Theme.paddingMedium; top: parent.top; right: parent.right }
                text: records.current ? records.current.repetitions : ""
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
            }
        }
    }

    CoverActionList {
        id: defaultActionList
        enabled: !records.current || records.current.databaseId < 0

        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                if (pageStack.currentPage.objectName !== "RecordDialog") {
                    pageStack.push("../dialogs/RecordDialog.qml", {}, PageStackAction.Immediate)
                }
                gibrievida.activate()
            }
        }
    }

    CoverActionList {
        id: recordActionlist
        enabled: records.current && records.current.activity.useRepeats && records.current.databaseId > 0

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: records.finish()
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: records.increaseRepetitions()
        }
    }

    CoverActionList {
        id: recordWithoutRepeatActionlist
        enabled: records.current && !records.current.activity.useRepeats && records.current.databaseId > 0

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: records.finish()
        }
    }
}


