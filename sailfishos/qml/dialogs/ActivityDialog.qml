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

Dialog {
    id: catDialog

    property int databaseId: -1
    property alias name: nameField.text
    property int oldCategoryId
    property alias categoryId: category.chosenValue
    property alias categoryName: category.value
    property alias minRepeats: minRepeatsField.text
    property alias maxRepeats: maxRepeatsField.text
    property alias distance: distanceSwitch.checked

    canAccept: (nameField.text.length > 0 && category.chosenValue > 0)

    Component.onCompleted: {
        oldCategoryId = categoryId
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: catDialogCol.height

        Column {
            id: catDialogCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                acceptText: (databaseId < 0) ? qsTr("Add") : qsTr("Edit")
            }

            TextField {
                id: nameField
                width: parent.width
                label: qsTr("Name"); placeholderText: label
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: nameField.focus = false
            }

            ValueButton {
                id: category
                label: qsTr("Category")
                value: qsTr("Please select")

                property int chosenValue

                onClicked: {
                    var dialog = pageStack.push(catChoserComp, {chosenValue: category.chosenValue, chosenText: category.value, label: category.label})
                    dialog.accepted.connect(function()  {category.chosenValue = dialog.chosenValue; category.value = dialog.chosenText})
                }

                Component {
                    id: catChoserComp
                    Page {
                        id: catChoser

                        signal accepted()

                        property int chosenValue
                        property string chosenText
                        property alias label: pHeader.title

                        PageHeader {
                            id: pHeader
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
                            model: CategoriesModel { id: categoriesModel }

                            delegate: ListItem {
                                id: listItem
                                width: parent.width
                                contentHeight: Theme.itemSizeSmall

                                ListView.onAdd: AddAnimation { target: listItem }
                                ListView.onRemove: animateRemoval(listItem)

                                highlighted: model.databaseId === catChoser.chosenValue

                                Rectangle {
                                    id: catColor
                                    anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                    color: model.color
                                    width: Theme.itemSizeExtraSmall
                                    height: width
                                }

                                Column {
                                    anchors { left: catColor.right; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                                    Label {
                                        text: model.name
                                        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                                    }

                                    Text {
                                        text: qsTr("%n activitie(s)", "", model.activities)
                                        font.pixelSize: Theme.fontSizeExtraSmall
                                        color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                                    }
                                }

                                onClicked: {
                                    catChoser.chosenValue = model.databaseId
                                    catChoser.chosenText = model.name
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

            TextSwitch {
                id: distanceSwitch
                text: qsTr("Distance tracking")
            }

            SectionHeader {
                text: qsTr("Repitition")
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                TextField {
                    id: minRepeatsField
                    width: (parent.width - parent.spacing)/2
                    label: qsTr("Minimum"); placeholderText: label
                    text: "0"
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: minRepeatsField.focus = false
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                TextField {
                    id: maxRepeatsField
                    width: (parent.width - parent.spacing)/2
                    label: qsTr("Maximum"); placeholderText: label
                    text: "0"
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: maxRepeatsField.focus = false
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }
        }
    }

    onAccepted: {
        if (databaseId > -1) {
            activities.edit(databaseId, name, oldCategoryId, categoryId, parseInt(minRepeats), parseInt(maxRepeats), distance)
        } else {
            activities.add(name, categoryId, parseInt(minRepeats), parseInt(maxRepeats), distance)
        }
    }
}

