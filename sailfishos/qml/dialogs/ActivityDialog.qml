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

//    property int databaseId: -1
//    property alias name: nameField.text
//    property int oldCategoryId
//    property alias categoryId: category.chosenValue
//    property alias categoryName: category.value
//    property alias minRepeats: minRepeatsField.text
//    property alias maxRepeats: maxRepeatsField.text
//    property alias distance: distanceSwitch.checked

    property Activity activity: null
    property int oldCategoryId

//    canAccept: (nameField.text.length > 0 && category.chosenValue > 0)
    canAccept: (nameField.text.length > 0 && categoryButton.chosenCategory && minRepeatsField.acceptableInput && maxRepeatsField.acceptableInput)

    CategoriesModel { id: categoriesModel }

    Component.onCompleted: {
//        oldCategoryId = categoryId
        if (activity) {
            oldCategoryId = activity.category.databaseId
            categoryButton.chosenCategory = activity.category
            categoryButton.value = activity.category.name
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: catDialogCol.height

        Column {
            id: catDialogCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
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

//                property int chosenValue
                property Category chosenCategory: null

                onChosenCategoryChanged: {
                    if (chosenCategory) {
                        categoryButton.value = chosenCategory.name
                    } else {
                        categoryButton.value = qsTr("Please select")
                    }
                }

                onClicked: {
//                    var dialog = pageStack.push(catChoserComp, {chosenValue: categoryButton.chosenValue, chosenText: categoryButton.value, label: categoryButton.label})
//                    dialog.accepted.connect(function()  {categoryButton.chosenValue = dialog.chosenValue; categoryButton.value = dialog.chosenText})
                    var dialog = pageStack.push(catChoserComp, {chosenCategory: categoryButton.chosenCategory, label: categoryButton.label, model: categoriesModel})
                    dialog.accepted.connect(function() {categoryButton.chosenCategory = dialog.chosenCategory})
                }

                Component {
                    id: catChoserComp
                    Page {
                        id: catChoser

                        signal accepted()

//                        property int chosenValue
//                        property string chosenText
                        property Category chosenCategory: null
                        property alias label: pHeader.title
                        property alias model: list.model

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

                            delegate: ListItem {
                                id: listItem
                                width: parent.width
                                contentHeight: Theme.itemSizeSmall

                                ListView.onAdd: AddAnimation { target: listItem }
                                ListView.onRemove: animateRemoval(listItem)

//                                highlighted: model.databaseId === catChoser.chosenValue
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
//                                    catChoser.chosenValue = model.databaseId
//                                    catChoser.chosenText = model.name
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

            TextSwitch {
                id: distanceSwitch
                text: qsTr("Distance tracking")
                checked: activity ? activity.useDistance : false
            }

            SectionHeader {
                text: qsTr("Repetition")
            }

            Row {
                width: parent.width
                spacing: Theme.paddingSmall

                TextField {
                    id: minRepeatsField
                    width: (parent.width - parent.spacing)/2
                    label: qsTr("Minimum"); placeholderText: label
                    text: activity ? activity.minRepeats : "0"
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: minRepeatsField.focus = false
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
            activity.category.activities = categoryButton.chosenCategory.activities
            activities.update(activity, oldCategoryId)
        } else {
            activities.add(nameField.text, categoryButton.chosenCategory, parseInt(minRepeatsField.text), parseInt(maxRepeatsField.text), distanceSwitch.checked)
        }

//        if (databaseId > -1) {
//            activities.edit(databaseId, name, oldCategoryId, categoryId, parseInt(minRepeats), parseInt(maxRepeats), distance)
//        } else {
//            activities.add(name, categoryId, parseInt(minRepeats), parseInt(maxRepeats), distance)
//        }
    }
}

