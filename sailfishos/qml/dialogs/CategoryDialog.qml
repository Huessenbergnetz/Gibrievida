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

Dialog {
    id: catDialog

    property int databaseId: -1
    property alias name: nameField.text
    property alias color: colorChoosed.color

    Component.onCompleted: {
        if (databaseId < 0) {
            color = categories.createRandomColor()
        }
    }

    canAccept: nameField.text.length > 0

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

            Row {
                width: parent.width

                Rectangle {
                    id: colorChoosed
                    height: nameField.height
                    width: height
                }

                TextField {
                    id: nameField
                    width: parent.width - colorChoosed.width - Theme.horizontalPageMargin
                    label: qsTr("Name"); placeholderText: label
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: nameField.focus = false
                }

            }

            ColorPicker {
                id: colorChooser
                onColorChanged: catDialog.color = color
            }
        }

    }

    onAccepted: {
        if (databaseId > -1) {
            categories.edit(databaseId, nameField.text, colorChooser.color)
        } else {
            categories.add(nameField.text, colorChoosed.color)
        }
    }
}

