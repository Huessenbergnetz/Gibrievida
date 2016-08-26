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

    objectName: "MainPage"

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }

            MenuItem {
                text: qsTr("Help")
                onClicked: pageStack.push(Qt.resolvedUrl("Help.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }

            MenuItem {
                text: records.current ? qsTr("Finish recording") : qsTr("Record activity")
                onClicked: records.current ? records.finish() : pageStack.push(Qt.resolvedUrl("../dialogs/RecordDialog.qml"))
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: mainPage.width

            PageHeader {
                title: "Gibrievida"
            }

            SectionHeader {
                text: qsTr("Current record")
            }

            Item {
                width: parent.width
                height: Theme.itemSizeMedium
                visible: !records.current

                Label {
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    text: qsTr("There is currently no active recording. Pull down to start a new recording.")
                    color: Theme.secondaryHighlightColor
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }

            BackgroundItem {
                id: curRec
                width: parent.width
                contentHeight: Theme.itemSizeSmall
                visible: records.current

                onClicked: pageStack.push(Qt.resolvedUrl("Record.qml"), {record: records.current})

                Rectangle {
                    id: cColor
                    anchors { left: parent.left; leftMargin: Theme.paddingSmall; top: parent.top; verticalCenter: parent.verticalCenter }
                    width: Theme.itemSizeExtraSmall / 5
                    height: Theme.itemSizeSmall * 0.9
                    color: records.current ? records.current.activity.category.color : ""
                }

                Column {
                    anchors { left: cColor.right; leftMargin: Theme.paddingMedium; right: parent.right; rightMargin: Theme.horizontalPageMargin; top: parent.top }

                    Row {
                        width: parent.width

                        Label {
                            id: aName
                            width: parent.width * 0.6
                            text: records.current ? records.current.activity.name : ""
                            color: curRec.highlighted ? Theme.highlightColor : Theme.primaryColor
                            truncationMode: TruncationMode.Fade
                        }

                        Text {
                            id: timeText
                            width: parent.width * 0.4
                            anchors { verticalCenter: aName.verticalCenter }
                            text: records.current ? helpers.relativeTimeString(records.current.start) : ""
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: curRec.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            horizontalAlignment: Text.AlignRight
                        }
                    }

                    Row {
                        width: parent.width

                        Text {
                            id: cName
                            width: parent.width * 0.33
                            text: records.current ? records.current.activity.category.name : ""
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: curRec.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            elide: Text.ElideRight
                        }

                        Item {
                            id: repetitionItem
                            width: distanceItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                            height: repText.height
                            visible: records.current && records.current.repetitions > 0

                            ImageHighlight {
                                id: repIcon
                                anchors { left: parent.left; top: parent.top; verticalCenter: repText.verticalCenter }
                                height: Theme.fontSizeExtraSmall
                                source: "image://theme/icon-s-retweet"
                                highlighted: curRec.highlighted
                            }

                            Text {
                                id: repText
                                anchors { left: repIcon.right; leftMargin: Theme.paddingSmall; top: parent.top }
                                text: records.current ? records.current.repetitions : ""
                                font.pixelSize: Theme.fontSizeExtraSmall
                                color: curRec.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            }
                        }

                        Item {
                            id: distanceItem
                            width: repetitionItem.visible ? (parent.width * 0.17) : (parent.width * 0.34)
                            height: distText.height
                            visible: records.current && records.current.distance > 0.0

                            ImageHighlight {
                                id: distIcon
                                anchors { left: parent.left; top: parent.top; verticalCenter: distText.verticalCenter }
                                height: Theme.fontSizeExtraSmall
                                source: "image://theme/icon-cover-transfers"
                                highlighted: curRec.highlighted
                            }

                            Text {
                                id: distText
                                anchors { left: distIcon.right; leftMargin: Theme.paddingSmall; top: parent.top }
                                text: records.current ? helpers.toDistanceString(records.current.distance) : ""
                                font.pixelSize: Theme.fontSizeExtraSmall
                                color: curRec.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            }
                        }

                        Item {
                            width: parent.width * 0.33
                            height: durText.height
                            visible: records.current && records.current.duration > 0

                            ImageHighlight {
                                id: durIcon
                                anchors { left: parent.left; top: parent.top; verticalCenter: durText.verticalCenter }
                                height: Theme.fontSizeExtraSmall
                                source: "image://theme/icon-s-duration"
                                highlighted: curRec.highlighted
                            }

                            Text {
                                id: durText
                                anchors { left: durIcon.right; leftMargin: Theme.paddingSmall }
                                text: records.current ? helpers.createDurationString(records.current.duration) : ""
                                font.pixelSize: Theme.fontSizeExtraSmall
                                color: curRec.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                            }
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Management")
            }

            Repeater {
                width: parent.width
                model: ListModel {
                    ListElement { name: QT_TR_NOOP("Categories"); icon: "image://theme/icon-m-tabs"; target: "Categories.qml" }
                    ListElement { name: QT_TR_NOOP("Activities"); icon: "image://theme/icon-m-watch"; target: "Activities.qml" }
                    ListElement { name: QT_TR_NOOP("Records"); icon: "image://theme/icon-m-gps"; target: "Records.qml" }
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
                        text: qsTranslate("MainPage",name)
                        color: catsItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                        anchors { left: catsIcon.right; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }
                    }
                }
            }
        }
    }
}

