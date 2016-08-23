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
    id: backupPage

    SilicaListView {
        id: backupList
        PullDownMenu {
            busy: backupModel.inOperation
            MenuItem {
                text: qsTr("Remove all")
                enabled: !backupModel.inOperation
                onClicked: remorse.execute(qsTr("Deleting all"), function() {backupModel.removeAll()})
            }

            MenuItem {
                text: qsTr("Create")
                onClicked: backupModel.create()
                enabled: !backupModel.inOperation
            }
        }

        header: PageHeader { title: qsTr("Database backups") }

        anchors.fill: parent

        model: BackupModel { id: backupModel }

        VerticalScrollDecorator { page: backupPage; flickable: backupList }

        delegate: ListItem {
            id: backupListItem
            width: parent.width
            contentHeight: Theme.itemSizeMedium
            menu: contextMenu

            ListView.onRemove: animateRemoval(backupListItem)
            ListView.onAdd: AddAnimation { target: backupListItem }

            function remove() {
                remorseAction("Deleting", function () {backupModel.remove(index)})
            }

            function restore() {
                remorseAction("Restoring", function() {backupModel.restore(index)})
            }

            Column {
                id: backupListItemCol
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

                Row {
                    width: parent.width

                    Label {
                        id: dateLabel
                        width: implicitWidth
                        //: backup list entry date, for format options visit http://doc.qt.io/qt-5/qml-qtqml-qt.html#formatDateTime-method
                        text: Qt.formatDate(time, qsTr("d. MMMM yyyy"))
                        textFormat: Text.PlainText
                        color: backupListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }

                    Label {
                        id: timeLabel
                        width: parent.width - dateLabel.width
                        horizontalAlignment: Text.AlignRight
                        //: backup list entry time, for format options visit http://doc.qt.io/qt-5/qml-qtqml-qt.html#formatDateTime-method
                        text: Qt.formatTime(time, qsTr("hh:mmap"))
                        textFormat: Text.PlainText
                        color: backupListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                }

                Text {
                    width: parent.width
                    color: backupListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    text: size
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: remove()
                        enabled: !backupModel.inOperation
                    }

                    MenuItem {
                        text: qsTr("Restore")
                        onClicked: restore()
                        enabled: !backupModel.inOperation
                    }
                }
            }
        }

        ViewPlaceholder {
            enabled: backupList.count === 0
            text: qsTr("No backups")
            hintText: qsTr("Use the pull down menu to add new backups")
        }
    }

    RemorsePopup {
        id: remorse
    }
}

