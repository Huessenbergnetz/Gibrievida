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

Dialog {
    id: catDialog

    property Activity activity: null
    property int oldCategoryId

    canAccept: (nameField.text.length > 0 && categoryButton.chosenCategory && minRepeatsField.acceptableInput && maxRepeatsField.acceptableInput)

    CategoriesModel { id: categoriesModel }

    Component.onCompleted: {
        if (activity) {
            oldCategoryId = activity.category.databaseId
            categoryButton.chosenCategory = activity.category
            categoryButton.value = activity.category.name
        }
    }

    SilicaFlickable {
        id: catDialogFlick
        anchors.fill: parent
        contentHeight: catDialogCol.height

        VerticalScrollDecorator {
            flickable: catDialogFlick
            page: catDialog
        }

        Column {
            id: catDialogCol
            width: parent.width
            spacing: Theme.paddingSmall

            DialogHeader {
                dialog: catDialog
                flickable: catDialogFlick
                acceptText: activity ? qsTr("Edit") : qsTr("Add")
            }

            TextField {
                id: nameField
                width: parent.width
                text: activity ? activity.name : ""
                label: qsTr("Name"); placeholderText: label
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: nameField.focus = false
            }

            ValueButton {
                id: categoryButton
                label: qsTr("Category")
                value: qsTr("Please select")

                property Category chosenCategory: null

                onChosenCategoryChanged: {
                    if (chosenCategory) {
                        categoryButton.value = chosenCategory.name
                    } else {
                        categoryButton.value = qsTr("Please select")
                    }
                }

                onClicked: {
                    var dialog = pageStack.push(catChoserComp, {chosenCategory: categoryButton.chosenCategory, label: categoryButton.label, model: categoriesModel})
                    dialog.accepted.connect(function() {categoryButton.chosenCategory = dialog.chosenCategory})
                }

                Component {
                    id: catChoserComp
                    Page {
                        id: catChoser

                        signal accepted()

                        property Category chosenCategory: null
                        property alias label: pHeader.title
                        property alias model: list.model

                        PageHeader {
                            id: pHeader
                            page: catChoser
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

                            VerticalScrollDecorator { flickable: list; page: catChoser }

                            delegate: ListItem {
                                id: listItem
                                width: parent.width
                                contentHeight: Theme.itemSizeSmall

                                ListView.onAdd: AddAnimation { target: listItem }
                                ListView.onRemove: animateRemoval(listItem)

                                highlighted: catChoser.chosenCategory ? model.item.databaseId === catChoser.chosenCategory.databaseId : false

                                Rectangle {
                                    id: catColor
                                    anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                    color: model.item.color
                                    width: Theme.itemSizeExtraSmall
                                    height: width
                                }

                                Column {
                                    anchors { left: catColor.right; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                                    Label {
                                        text: model.item.name
                                        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                                    }

                                    Text {
                                        text: qsTr("%n activitie(s)", "", model.item.activities)
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                                    }
                                }

                                onClicked: {
                                    catChoser.chosenCategory = model.item
                                    catChoser.accepted()
                                    search.text = ""
                                    search.focus = false
                                    pageStack.pop()
                                }
                            }
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Distance tracking")
            }

            TextSwitch {
                id: distanceSwitch
                text: qsTr("Enable distance tracking")
                checked: activity ? activity.useDistance : false
                description: qsTr("Distance tracking is done by GPS/GLONASS. The recording will start when the application has found a valid and accurate position.")
            }

            SectionHeader {
                text: qsTr("Repetition")
            }

            Text {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin }
                width: parent.width - (2 * Theme.horizontalPageMargin)
                font.pixelSize: Theme.fontSizeExtraSmall
                text: qsTr("To activate repetitions for this activity, enter minimum and maximum repetitions greater than 0.")
                color: Theme.secondaryColor
                wrapMode: Text.WordWrap
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                TextField {
                    id: minRepeatsField
                    width: (parent.width - parent.spacing)/2
                    label: qsTr("Minimum"); placeholderText: label
                    text: activity ? activity.minRepeats : "0"
                    EnterKey.iconSource: parseInt(minRepeatsField.text) > 0 ? "image://theme/icon-m-enter-next" : "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: parseInt(minRepeatsField.text) > 0 ? maxRepeatsField.focus = true : minRepeatsField.focus = false
                    validator: IntValidator { bottom: 0 }
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                TextField {
                    id: maxRepeatsField
                    width: (parent.width - parent.spacing)/2
                    label: qsTr("Maximum"); placeholderText: label
                    text: activity ? activity.maxRepeats : "0"
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: maxRepeatsField.focus = false
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 0 }
                    enabled: parseInt(minRepeatsField.text) > 0
                }
            }

            SectionHeader {
                text: qsTr("Repetitions sensor")
            }

            ComboBox {
                id: sensorChoser
                currentIndex: activity ? activity.sensorType : 0
                width: parent.width
                label: qsTr("Use sensor")
                menu: ContextMenu {
                    MenuItem { text: qsTr("No sensor") }
                    MenuItem { text: qsTr("Proximity") }
                    MenuItem { text: qsTr("Up and down (upright)") }
                    MenuItem { text: qsTr("Up and down (lying)") }
                }
                description: qsTr("The selected sensor will be used to increase repetitions.")
                onCurrentIndexChanged: setSensorChoserDescription()
                function setSensorChoserDescription() {
                    switch (sensorChoser.currentIndex) {
                    case 1:
                        sensorChoser.description = qsTr("Use the proximity sensor to increase repetitions by moving near to your device. Can be used for push-ups for example.")
                        break;
                    case 2:
                        sensorChoser.description = qsTr("Move up and down while holding your device upright (portrait mode) to increase repetitions. Can be used for squats for example.")
                        break;
                    case 3:
                        sensorChoser.description = qsTr("Move up and down while holding your device with front or back to the ground to increase repetitions. Can be used for squats for example.")
                        break;
                    default:
                        sensorChoser.description = qsTr("The selected sensor will be used to increase repetitions.")
                        break;
                    }
                }
            }

            TextField {
                id: sensorDelayField
                enabled: sensorChoser.currentIndex === 2
                width: parent.width
                label: qsTr("Sensor delay in milliseconds"); placeholderText: label
                text: activity ? activity.sensorDelay : "0"
                validator: IntValidator { bottom: 0 }
                inputMethodHints: Qt.ImhDigitsOnly
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: sensorDelayField.focus = false
            }

            Text {
                height: text.length ? (implicitHeight + Theme.paddingLarge) : 0
                opacity: sensorDelayField.enabled ? 1.0 : 0.4
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.Wrap
                color: Theme.secondaryColor
                text: qsTr("The sensor delay in milliseconds defines how often the sensor will be checked to increase the repetitions. You should choose a value that fits to the speed you repeat the activitiy. Maybe you have to test some values to find the perfect fitting delay. For example, try 1800 as a value for squats together with up and down sensor.")
            }
        }
    }

    onAccepted: {
        if (activity) {
            activity.name = nameField.text
            activity.minRepeats = parseInt(minRepeatsField.text)
            activity.maxRepeats = parseInt(maxRepeatsField.text)
            activity.useDistance = distanceSwitch.checked
            activity.category.databaseId = categoryButton.chosenCategory.databaseId
            activity.category.name = categoryButton.chosenCategory.name
            activity.category.color = categoryButton.chosenCategory.color
            activity.sensorType = sensorChoser.currentIndex
            activity.sensorDelay = parseInt(sensorDelayField.text)
            activities.update(activity, oldCategoryId)
        } else {
            activities.add(nameField.text, categoryButton.chosenCategory, parseInt(minRepeatsField.text), parseInt(maxRepeatsField.text), distanceSwitch.checked, sensorChoser.currentIndex, parseInt(sensorDelayField.text))
        }
    }
}

