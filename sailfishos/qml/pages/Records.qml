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
    id: recordsManager

    Component.onCompleted: {
        recordsModel.update()
    }

    SilicaListView {
        id: recordsListView
        anchors.fill: parent
        spacing: Theme.paddingSmall


        PullDownMenu {
            MenuItem {
                text: qsTr("Remove all")
            }

            MenuItem {
                text: qsTr("Record activity")
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: recordsManager
        }

        model: RecordsModel {
            id: recordsModel
//            controller: categories
//            activitiesController: activities
        }

        header: PageHeader {
            title: qsTr("Records")
            page: recordsManager
        }

        delegate: ListItem {
            id: recManagerListItem
            width: ListView.view.width
            contentHeight: Theme.itemSizeSmall
            menu: contextMenu

            ListView.onRemove: animateRemoval(recManagerListItem)

            function remove() {
//                remorseAction(qsTr("Removing"), function() {categories.remove(model.databaseId)})
            }

            Rectangle {
                id: cColor
                anchors { left: parent.left; leftMargin: Theme.paddingSmall; top: parent.top; verticalCenter: parent.verticalCenter }
                width: Theme.itemSizeExtraSmall/5
                height: Theme.itemSizeSmall * 0.9
                color: categoryColor
            }

            Column {
                anchors { left: cColor.right; leftMargin: Theme.paddingMedium; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: parent.top }

                Row {
                    width: parent.width

                    Label {
                        id: aName
                        width: parent.width*0.6
                        text: activityName
                        color: recManagerListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                        truncationMode: TruncationMode.Fade
                    }

                    Text {
                        id: timeText
                        width: parent.width*0.4
                        anchors { verticalCenter: aName.verticalCenter }
                        //: date and time foramt, see http://doc.qt.io/qt-5/qml-qtqml-qt.html#formatDateTime-method
                        text: Qt.formatDateTime(start, qsTr("dd.MM.yyyy hh:mmap"))
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        verticalAlignment: Text.AlignRight
                    }
                }

                Row {
                    width: parent.width

                    Text {
                        id: cName
                        width: parent.width * 0.33
                        text: categoryName
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        elide: Text.ElideRight
                    }

                    Item {
                        width: parent.width * 0.33
                        height: durText.height

                        ImageHighlight {
                            id: durIcon
                            anchors { left: parent.left; top: parent.top; verticalCenter: durText.verticalCenter }
                            height: Theme.fontSizeExtraSmall
                            source: "image://theme/icon-s-duration"
                            highlighted: recManagerListItem.highlighted
                        }

                        Text {
                            id: durText
                            anchors { left: durIcon.right; leftMargin: Theme.paddingSmall }
                            text: durationString
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        }
                    }

                    Item {
                        id: repetitionItem
                        width: distanceItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                        height: repText.height
                        visible: repetitions > 0

                        ImageHighlight {
                            id: repIcon
                            anchors { left: parent.left; top: parent.top; verticalCenter: repText.verticalCenter }
                            height: Theme.fontSizeExtraSmall
                            source: "image://theme/icon-s-retweet"
                            highlighted: recManagerListItem.highlighted
                        }

                        Text {
                            id: repText
                            anchors { left: repIcon.right; leftMargin: Theme.paddingSmall; top: parent.top }
                            text: repetitions
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        }
                    }

                    Item {
                        id: distanceItem
                        width: repetitionItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                        height: distText.height
                        visible: distance > 0

                        ImageHighlight {
                            id: distIcon
                            anchors { left: parent.left; top: parent.top; verticalCenter: distText.verticalCenter }
                            height: Theme.fontSizeExtraSmall
                            source: "image://theme/icon-cover-transfers"
                            highlighted: recManagerListItem.highlighted
                        }

                        Text {
                            id: distText
                            anchors { left: distIcon.right; leftMargin: Theme.paddingSmall; top: parent.top }
                            text: distance
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        }
                    }
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Edit")
//                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/CategoryDialog.qml"), {databaseId: databaseId, name: model.name, color: model.color})
                    }
                    MenuItem {
                        text: qsTr("Remove")
//                        onClicked: remove()
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: recordsListView.count === 0
            text: qsTr("No records")
            hintText: qsTr("Use the pull down menu to add new records")
        }

        RemorsePopup {
            id: remorse
        }
    }
}
