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

Page {
    id: activityManager

    Column {
        id: headerContainer
        width: activityManager.width

        PageHeader {
            title: qsTr("Activities")
            page: activityManager
        }

        SearchField {
            id: searchField
            width: parent.width
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: searchField.focus = false
            visible: activitiesListView.count > 0

            Binding {
                target: activitiesModel
                property: "search"
                value: searchField.text.trim()
            }
        }
    }

    SilicaListView {
        id: activitiesListView
        anchors.fill: parent
        currentIndex: -1


        PullDownMenu {
            MenuItem {
                text: qsTr("Remove all")
                onClicked: remorse.execute(qsTr("Removing all"), function() {activities.removeAll()})
                enabled: activitiesListView.count > 0
            }

            MenuItem {
                text: qsTr("Add activity")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/ActivityDialog.qml"))
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: activityManager
        }

        model: ActivitiesModel {
            id: activitiesModel
            activitiesController: activities
            categoriesController: categories
            recordsController: records
        }

        header: Item {
            id: header
            width: headerContainer.width
            height: headerContainer.height
            Component.onCompleted: headerContainer.parent = header
        }

        delegate: ListItem {
            id: actsManagerItem
            width: ListView.view.width
            contentHeight: Theme.itemSizeSmall
            menu: contextMenu

            ListView.onRemove: animateRemoval(actsManagerItem)
            ListView.onAdd: AddAnimation { target: actsManagerItem }

            onClicked: pageStack.push(Qt.resolvedUrl("Records.qml"), {activity: model.item})

            function remove() {
                remorseAction(qsTr("Removing"), function() {activities.remove(item)})
            }

            Column {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                Label {
                    text: item.name
                    width: parent.width
                    color: actsManagerItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Item {
                    width: parent.width
                    height: cName.height

                    Rectangle {
                        id: cColor
                        anchors { left: parent.left; verticalCenter: cName.verticalCenter; top: parent.top }
                        width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                        color: item.category.color
                    }

                    Text {
                        id: cName
                        anchors { left: cColor.right; leftMargin: Theme.paddingMedium; top: parent.top }
                        text: item.category.name
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: actsManagerItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                    }

                    Text {
                        id: recordsCount
                        anchors { right: parent.right; top: parent.top }
                        text: qsTr("%n record(s)", "", item.records)
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: actsManagerItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                    }

                    ImageHighlight {
                        id: cRepeats
                        anchors { verticalCenter: cName.verticalCenter; right: recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                        source: "image://theme/icon-s-retweet"
                        highlighted: actsManagerItem.highlighted
                        visible: item.useRepeats
                    }

                    ImageHighlight {
                        id: cDistance
                        anchors { verticalCenter: cName.verticalCenter; right: cRepeats.visible ? cRepeats.left : recordsCount.left; rightMargin: Theme.paddingMedium; top: parent.top }
                        source: "image://theme/icon-cover-transfers"
                        width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                        highlighted: actsManagerItem.highlighted
                        visible: item.useDistance
                    }
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Start new record")
                        onClicked: {
                            records.prepare()
                            records.add(model.item)
                            pageStack.push(Qt.resolvedUrl("../pages/Record.qml"), {record: records.current})
                        }
                    }

                    MenuItem {
                        text: qsTr("Edit")
                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/ActivityDialog.qml"), {activity: model.item})
                    }
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: remove()
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: activitiesListView.count === 0
            text: qsTr("No activities")
            hintText: qsTr("Use the pull down menu to add new activities")
        }

        RemorsePopup {
            id: remorse
        }
    }
}

