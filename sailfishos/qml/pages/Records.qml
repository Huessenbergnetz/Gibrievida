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

    property Activity activity: null
    property Category category: null

    Component.onCompleted: {
        if (category) {
            recordsModel.categoryId = category.databaseId
        } else if (activity) {
            recordsModel.activityId = activity.databaseId
        }

        recordsModel.update()
    }

    onStatusChanged: {
        if (status === PageStatus.Active && !canNavigateForward) {
            pageStack.pushAttached(Qt.resolvedUrl("AttachedFilters.qml"), {recModel: recordsModel})
        }
    }

    SilicaListView {
        id: recordsListView
        anchors.fill: parent
        spacing: Theme.paddingSmall
        clip: true


        PullDownMenu {
            MenuItem {
                text: qsTr("Remove all")
                enabled: recordsListView.count > 0
                onClicked: {
                    if (category) {
                        remorse.execute(qsTr("Removing all"), function() {records.removeByCategory(category)})
                    } else if (activity) {
                        remorse.execute(qsTr("Removing all"), function() {records.removeByActivity(activity)})
                    } else {
                        remorse.execute(qsTr("Removing all"), function() {records.removeAll()})
                    }
                }
            }

            MenuItem {
                text: records.current ? qsTr("Finish recording") : qsTr("Record activity")
                onClicked: records.current ? records.finish() : pageStack.push(Qt.resolvedUrl("../dialogs/RecordDialog.qml"))
            }
        }

        VerticalScrollDecorator {
            flickable: parent
            page: recordsManager
        }

        model: RecordsModel {
            id: recordsModel
            recordsController: records
            activitiesController: activities
            categoriesController: categories
        }

        header: PageHeader {
            title: qsTr("Records")
            page: recordsManager
            description: category ? category.name : activity ? activity.name : qsTr("All")
        }

        BusyIndicator {
            size: BusyIndicatorSize.Large
            anchors.centerIn: parent
            running: visible
            visible: recordsModel.inOperation
        }

        delegate: ListItem {
            id: recManagerListItem
            width: ListView.view.width
            contentHeight: Theme.itemSizeSmall
            menu: contextMenu

            ListView.onAdd: AddAnimation { target: recManagerListItem }
            ListView.onRemove: animateRemoval(recManagerListItem)

            onClicked: pageStack.push(Qt.resolvedUrl("Record.qml"), {record: model.item, comingFromList: true})

            Connections {
                target: model.item
                onStartRemoving: recManagerListItem.remove()
            }

            function remove() {
                remorseAction(qsTr("Removing"), function() {records.remove(model.item)})
            }

            Rectangle {
                id: cColor
                anchors { left: parent.left; leftMargin: Theme.paddingSmall; top: parent.top; verticalCenter: parent.verticalCenter }
                width: Theme.itemSizeExtraSmall/5
                height: Theme.itemSizeSmall * 0.9
                color: item.activity.category.color
            }

            Column {
                anchors { left: cColor.right; leftMargin: Theme.paddingMedium; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: parent.top }

                Row {
                    width: parent.width

                    Label {
                        id: aName
                        width: parent.width*0.6
                        text: item.activity.name
                        color: recManagerListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                        truncationMode: TruncationMode.Fade
                    }

                    Text {
                        id: timeText
                        width: parent.width*0.4
                        anchors { verticalCenter: aName.verticalCenter }
                        text: helpers.relativeTimeString(item.start)
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    width: parent.width

                    Text {
                        id: cName
                        width: parent.width * 0.33
                        text: item.activity.category.name
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
                            text: helpers.createDurationString(item.duration)
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        }
                    }

                    Item {
                        id: repetitionItem
                        width: distanceItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                        height: repText.height
                        visible: item.repetitions > 0

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
                            text: item.repetitions
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: recManagerListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        }
                    }

                    Item {
                        id: distanceItem
                        width: repetitionItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                        height: distText.height
                        visible: item.distance > 0.0

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
                            text: helpers.toDistanceString(item.distance)
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
                        onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/RecordDialog.qml"), {record: model.item})
                    }
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: remove()
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: recordsListView.count === 0 && !recordsModel.inOperation
            text: qsTr("No records")
            hintText: qsTr("Use the pull down menu to add new records")
        }

        RemorsePopup {
            id: remorse
        }
    }
}

