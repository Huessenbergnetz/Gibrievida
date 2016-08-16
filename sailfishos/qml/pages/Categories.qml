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

Page {
    id: categoryManager

    Column {
        id: headerContainer
        width: categoryManager.width

        PageHeader {
            title: qsTr("Categories")
            page: categoryManager
        }

        SearchField {
            id: searchField
            width: parent.width
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
            EnterKey.onClicked: searchField.focus = false

            Binding {
                target: categoryModel
                property: "search"
                value: searchField.text.trim()
            }
        }
    }

    SilicaListView {
        id: categoriesListView
        anchors.fill: parent
        currentIndex: -1

        PullDownMenu {
            MenuItem {
                text: qsTr("Remove all")
                onClicked: remorse.execute(qsTr("Removing all"), function() {categories.removeAll()})
            }

            MenuItem {
                text: qsTr("Add category")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CategoryDialog.qml"))
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: categoryManager
        }

        model: CategoriesModel {
            id: categoryModel
            controller: categories
            activitiesController: activities
        }

        header: Item {
            id: header
            width: headerContainer.width
            height: headerContainer.height
            Component.onCompleted: headerContainer.parent = header
        }

        delegate: ListItem {
            id: catManagerListItem
            width: ListView.view.width
            contentHeight: Theme.itemSizeSmall
            menu: contextMenu

            ListView.onRemove: animateRemoval(catManagerListItem)

            function remove() {
                remorseAction(qsTr("Removing"), function() {categories.remove(model.databaseId)})
            }

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
                    text: name
                    color: catManagerListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Text {
                    text: qsTr("%n activitie(s)", "", activities)
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: catManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Edit")
                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CategoryDialog.qml"), {databaseId: databaseId, name: model.name, color: model.color})
                    }
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: remove()
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: categoriesListView.count === 0
            text: qsTr("No categories")
            hintText: qsTr("Use the pull down menu to add new categories")
        }

        RemorsePopup {
            id: remorse
        }
    }
}

