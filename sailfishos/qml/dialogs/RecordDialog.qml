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
import harbour.gibrievida 1.0
import "../common"

Dialog {
    id: recDialog

    objectName: "RecordDialog"

    property Record record: null

    property int oldActivityId

    ActivitiesModel { id: activitiesModel }

    canAccept: record ? activityButton.chosenActivity && repetitionsField.acceptableInput && distanceField.acceptableInput : activityButton.chosenActivity

    Component.onCompleted: {
        if (record) {
            oldActivityId = record.activity.databaseId
            activityButton.chosenActivity = record.activity
            activityButton.value = record.activity.name
        } else {
            records.prepare()
        }
    }

    SilicaFlickable {
        id: recDialogFlick
        anchors.fill: parent
        contentHeight: recDialogCol.height

        VerticalScrollDecorator {
            flickable: recDialogFlick
            page: recDialog
        }

        Column {
            id: recDialogCol
            width: parent.width
            spacing: Theme.paddingMedium

            DialogHeader {
                dialog: recDialog
                flickable: recDialogFlick
                acceptText: record ? qsTr("Edit") : qsTr("Start")
            }

            ValueButton {
                id: activityButton
                label: qsTr("Activity")
                value: qsTr("Please select")

                property Activity chosenActivity: null

                onChosenActivityChanged: {
                    if (chosenActivity) {
                        activityButton.value = chosenActivity.name
                    } else {
                        activityButton.value = qsTr("Please select")
                    }
                }

                onClicked: {
                    var dialog = pageStack.push(actChoserComp, {chosenActivity: activityButton.chosenActivity, label: activityButton.label, model: activitiesModel})
                    dialog.accepted.connect(function() {activityButton.chosenActivity = dialog.chosenActivity})
                }

                Component {
                    id: actChoserComp

                    Page {
                        id: actChoser

                        signal accepted()

                        property Activity chosenActivity: null
                        property alias label: pHeader.title
                        property alias model: list.model

                        PageHeader {
                            id: pHeader
                            page: actChoser
                        }

                        SearchField {
                            id: search
                            width: parent.width
                            anchors { top: pHeader.bottom; left: parent.left; right: parent.right }
                            placeholderText: qsTr("Search")
                            EnterKey.iconSource: "image://theme/icon-m-enter-close"
                            EnterKey.onClicked: search.focus = false
                            onTextChanged: list.model.search = text
                        }

                        ListView {
                            id: list
                            anchors { top: search.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
                            contentWidth: parent.width
                            clip: true

                            VerticalScrollDecorator { flickable: list; page: actChoser }

                            delegate: ListItem {
                                id: listItem
                                width: ListView.view.width
                                contentHeight: Theme.itemSizeSmall

                                ListView.onAdd: AddAnimation { target: listItem }
                                ListView.onRemove: animateRemoval(listItem)

                                highlighted: actChoser.chosenActivity ? model.item.databaseId === actChoser.chosenActivity.databaseId : false

                                onClicked: {
                                    actChoser.chosenActivity = model.item
                                    actChoser.accepted()
                                    search.text = ""
                                    search.focus = false
                                    pageStack.pop()
                                }

                                Column {
                                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                                    Label {
                                        text: model.item.name
                                        width: parent.width
                                        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                                    }

                                    Item {
                                        width: parent.width
                                        height: cName.height

                                        Rectangle {
                                            id: cColor
                                            anchors { left: parent.left; verticalCenter: cName.verticalCenter; top: parent.top }
                                            width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                                            color: model.item.category.color
                                        }

                                        Text {
                                            id: cName
                                            anchors { left: cColor.right; leftMargin: Theme.paddingMedium; top: parent.top }
                                            text: model.item.category.name
                                            font.pixelSize: Theme.fontSizeExtraSmall
                                            color: listItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                                        }

                                        Text {
                                            id: recordsCount
                                            anchors { right: parent.right; top: parent.top }
                                            text: qsTr("%n record(s)", "", model.item.records)
                                            font.pixelSize: Theme.fontSizeExtraSmall
                                            color: listItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                                        }

                                        ImageHighlight {
                                            id: cRepeats
                                            anchors { verticalCenter: cName.verticalCenter; right: recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                                            source: "image://theme/icon-s-retweet"
                                            width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                                            highlighted: listItem.highlighted
                                            visible: model.item.useRepeats
                                        }

                                        ImageHighlight {
                                            id: cDistance
                                            anchors { verticalCenter: cName.verticalCenter; right: cRepeats.visible ? cRepeats.left : recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                                            source: "image://theme/icon-cover-transfers"
                                            width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                                            highlighted: listItem.highlighted
                                            visible: model.item.useDistance
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            DateTimeButton {
                id: startTime
                label: qsTr("Start time")
                visible: record
                dateTime: record ? record.start : new Date()
            }

            DurationButton{
                id: durationButton
                visible: record
                label: qsTr("Duration")
                duration: record ? record.duration : 0
            }

            TextField {
                id: repetitionsField
                width: parent.width
                visible: record && activityButton.chosenActivity.useRepeats
                label: qsTr("Repetitions"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: record && activityButton.chosenActivity.useRepeats ? 1 : 0 }
                text: record && activityButton.chosenActivity.useRepeats ? record.repetitions : "0"
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: repetitionsField.focus = false
            }

            TextField {
                id: distanceField
                width: parent.width
                visible: record && activityButton.chosenActivity.useDistance
                label: qsTr("Distance in %1").arg(config.distanceMeasurement === Locale.MetricSystem ? qsTr("metres") : qsTr("yards")); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: DoubleValidator { bottom: record && activityButton.chosenActivity.useDistance ? 0.01 : 0.0; locale: Qt.locale().string }
                text: record && activityButton.chosenActivity.useDistance ? config.distanceMeasurement === Locale.MetricSystem ? record.distance : helpers.toYards(record.distance) : "0"
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: distanceField.focus = false
            }

            Slider {
                id: finishByProximitySlider
                width: parent.width
                visible: !record
                minimumValue: 0.0; maximumValue: 5.0
                stepSize: 1.0
                valueText: value > 0 ? qsTr("%n second(s)", "", value) : qsTr("Disabled")
                label: qsTr("Finish on covering")
            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                visible: !record
                opacity: finishByProximitySlider.value > 0 ? 1.0 : 0.4
                color: Theme.secondaryColor
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeExtraSmall
                text: qsTr("You can finish this recording by covering the proximity sensor of your device. Lay it on it's front or put it in your pocket to finish a recording.")
            }

            TextArea {
                id: noteText
                width: parent.width
                height: 2 * Theme.itemSizeExtraLarge
                placeholderText: qsTr("Note")
                label: qsTr("Note")
                text: record ? record.note : ""
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: noteText.focus = false
            }
        }
    }

    acceptDestination: !record ? Qt.resolvedUrl("../pages/Record.qml") : null
    acceptDestinationProperties: !record ? {record: records.current} : {}
    acceptDestinationAction: !record ? PageStackAction.Replace : PageStackAction.Push

    onAccepted: {
        if (record) {
            record.updateActivity(activityButton.chosenActivity)
            record.start = startTime.dateTime
            record.updateDuration(durationButton.duration)
            record.updateRepetitions(parseInt(repetitionsField.text))
            if (config.distanceMeasurement === Locale.MetricSystem) {
                record.updateDistance(parseFloat(distanceField.text))
            } else {
                record.updateDistance(helpers.toMetres(parseFloat(distanceField.text)))
            }
            record.note = noteText.text
            records.update(record, oldActivityId)
        } else {
            records.add(activityButton.chosenActivity, noteText.text, finishByProximitySlider.value)
        }
    }

    onRejected: {
        if (!record) {
            records.cancel()
        }
    }
}

