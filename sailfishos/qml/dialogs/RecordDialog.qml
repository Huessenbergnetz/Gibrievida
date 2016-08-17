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
import harbour.gibrievida 1.0
import "../common"

Dialog {
    id: recDialog

    property int databaseId: -1
    property alias activityId: activity.chosenValue
    property alias activityName: activity.value
    property alias note: noteText.text
    property alias start: startTime.dateTime
    property alias duration: durationButton.duration
    property alias durationString: durationButton.value
    property alias repetitions: repetitionsField.text

    canAccept: databaseId < 0 ? (activity.chosenValue > 0) : repetitionsField.acceptableInput

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
                acceptText: (databaseId < 0) ? qsTr("Add") : qsTr("Edit")
            }

            ValueButton {
                id: activity
                label: qsTr("Activity")
                value: qsTr("Please select")

                property int chosenValue

                onClicked: {
                    var dialog = pageStack.push(actChoserComp, {chosenValue: activity.chosenValue, chosenText: activity.value, label: activity.label})
                    dialog.accepted.connect(function() {activity.chosenValue = dialog.chosenValue; activity.value = dialog.chosenText})
                }

                Component {
                    id: actChoserComp

                    Page {
                        id: actChoser

                        signal accepted()

                        property int chosenValue
                        property string chosenText
                        property alias label: pHeader.title

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
                            model: ActivitiesModel { id: activitiesModel }

                            delegate: ListItem {
                                id: listItem
                                width: ListView.view.width
                                contentHeight: Theme.itemSizeSmall

                                ListView.onAdd: AddAnimation { target: listItem }
                                ListView.onRemove: animateRemoval(listItem)

                                highlighted: model.databaseId === actChoser.chosenValue

                                onClicked: {
                                    actChoser.chosenValue = model.databaseId
                                    actChoser.chosenText = model.name
                                    actChoser.accepted()
                                    search.text = ""
                                    search.focus = false
                                    pageStack.pop()
                                }

                                Column {
                                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                                    Label {
                                        text: model.name
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
                                            color: model.categoryColor
                                        }

                                        Text {
                                            id: cName
                                            anchors { left: cColor.right; leftMargin: Theme.paddingMedium; top: parent.top }
                                            text: model.categoryName
                                            font.pixelSize: Theme.fontSizeExtraSmall
                                            color: listItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                                        }

                                        Text {
                                            id: recordsCount
                                            anchors { right: parent.right; top: parent.top }
                                            text: qsTr("%n record(s)", "", model.records)
                                            font.pixelSize: Theme.fontSizeExtraSmall
                                            color: listItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                                        }

                                        ImageHighlight {
                                            id: cRepeats
                                            anchors { verticalCenter: cName.verticalCenter; right: recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                                            source: "image://theme/icon-cover-sync"
                                            width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                                            highlighted: listItem.highlighted
                                            visible: (model.minRepeats > 0 && model.maxRepeats > 0)
                                        }

                                        ImageHighlight {
                                            id: cDistance
                                            anchors { verticalCenter: cName.verticalCenter; right: cRepeats.visible ? cRepeats.left : recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                                            source: "image://theme/icon-cover-transfers"
                                            width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                                            highlighted: listItem.highlighted
                                            visible: model.distance
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
                visible: databaseId > 0
            }

            DurationButton{
                id: durationButton
                visible: databaseId > 0
                label: qsTr("Duration")
            }

            TextField {
                id: repetitionsField
                width: parent.width
                visible: databaseId > 0
                label: qsTr("Repetitions"); placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1 }
            }

            TextArea {
                id: noteText
                width: parent.width
                height: 2 * Theme.itemSizeExtraLarge
                placeholderText: qsTr("Note")
                label: qsTr("Note")
            }
        }
    }

    onAccepted: {
        if (databaseId > -1) {

        } else {
            records.add(activityId, note)
        }
    }
}

