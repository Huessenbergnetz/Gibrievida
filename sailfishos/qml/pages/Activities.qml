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

            Binding {
                target: activitiesModel
                property: "search"
                value: searchField.text.trim()
            }
        }
    }

    SilicaListView {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Remove all")
                onClicked: remorse.execute(qsTr("Removing all"), function() {activities.removeAll()})
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

            ListView.onRemove: animateRemoval(catManagerListItem)

            function remove() {
                remorseAction(qsTr("Removing"), function() {activites.remove(model.databaseId)})
            }

            Column {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                Label {
                    text: name
                    width: parent.width
                    color: actsManagerItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Row {
                    width: parent.width
                    spacing: Theme.paddingMedium

                    Rectangle {
                        id: cColor
                        anchors { verticalCenter: cName.verticalCenter }
                        width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                        color: categoryColor
                    }

                    Text {
                        id: cName
                        width: parent.width - cColor.width - cRepeats.width - cDistance.width - ((parent.visibleChildren  - 1) * parent.spacing)
                        text: categoryName
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: actsManagerItem.highlighted ? Theme.secondaryHighlightColor: Theme.secondaryColor
                    }

                    ImageHighlight {
                        id: cRepeats
                        anchors { verticalCenter: cName.verticalCenter }
                        source: "image://theme/icon-s-sync"
                        width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                        highlighted: actsManagerItem.highlighted
                        visible: (minRepeats > 0 && maxRepeats > 0)
                    }

                    ImageHighlight {
                        id: cDistance
                        anchors { verticalCenter: cName.verticalCenter }
                        source: "image://theme/icon-cover-transfers"
                        width: Theme.fontSizeExtraSmall; height: Theme.fontSizeExtraSmall
                        highlighted: actsManagerItem.highlighted
                        visible: distance
                    }
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Edit")
                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/ActivityDialog.qml"), {databaseId: model.databaseId, name: model.name, categoryId: model.categoryId, categoryName: model.categoryName, minRepeats: model.minRepeats, maxRepeats: model.maxRepeats, distance: model.distance})
                    }
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: remove()
                    }
                }
            }
        }

        RemorsePopup {
            id: remorse
        }
    }
}

