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
import "../common"

Page {
    id: mainPage

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
            }

            MenuItem {
                text: qsTr("Track activity")
            }
        }

        contentHeight: column.height

        Column {
            id: column

            width: mainPage.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: "Gibrievida"
            }

            Repeater {
                width: parent.width
                model: ListModel {
                    ListElement { name: QT_TR_NOOP("Categories"); icon: "image://theme/icon-m-levels"; target: "Categories.qml" }
                    ListElement { name: QT_TR_NOOP("Activities"); icon: "image://theme/icon-m-watch"; target: "Activities.qml" }
                }
                delegate: BackgroundItem {
                    id: catsItem
                    width: column.width
                    contentHeight: Theme.itemSizeSmall

                    onClicked: pageStack.push(Qt.resolvedUrl(target))

                    ImageHighlight {
                        id: catsIcon
                        source: icon
                        highlighted: catsItem.highlighted
                        anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    }

                    Label {
                        text: qsTranslate("mainPage",name)
                        color: catsItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                        anchors { left: catsIcon.right; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    }
                }
            }
        }
    }
}

