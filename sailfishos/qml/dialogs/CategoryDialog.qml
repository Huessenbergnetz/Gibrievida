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

    property Category category: null

    Component.onCompleted: {
        if (category) {
            nameField.text = category.name
            chosedColor.color = category.color
        } else {
            chosedColor.color = categories.createRandomColor()
        }
    }

    canAccept: nameField.text.length > 0

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
            spacing: Theme.paddingLarge

            DialogHeader {
                acceptText: category ? qsTr("Edit") : qsTr("Add")
                flickable: catDialogFlick
                dialog: catDialog
            }

            Row {
                width: parent.width

                Rectangle {
                    id: chosedColor
                    height: nameField.height
                    width: height
                }

                TextField {
                    id: nameField
                    width: parent.width - chosedColor.width - Theme.horizontalPageMargin
                    label: qsTr("Name"); placeholderText: label
                    EnterKey.iconSource: "image://theme/icon-m-enter-close"
                    EnterKey.onClicked: nameField.focus = false
                }

            }

            ColorPicker {
                id: colorChooser
                onColorChanged: {
                    chosedColor.color = color
                }
            }
        }

    }

    onAccepted: {
        if (category) {
            category.name = nameField.text
            category.color = chosedColor.color
            categories.update(category)
        } else {
            categories.add(nameField.text, chosedColor.color)
        }
    }
}

