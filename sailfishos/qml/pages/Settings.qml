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
import QtMultimedia 5.0
import harbour.gibrievida 1.0
import "../common"

Page {
    id: settingsPage

    SilicaFlickable {
        id: settingsFlick
        PullDownMenu {
            MenuItem {
                text: qsTr("Database backups")
                onClicked: pageStack.push(Qt.resolvedUrl("Backups.qml"))
            }
        }

        anchors.fill: parent
        contentHeight: settingsCol.height

        VerticalScrollDecorator {
            page: settingsPage
            flickable: settingsFlick
        }

        Column {
            id: settingsCol
            width: parent.width

            PageHeader {
                title: qsTr("Settings")
                page: settingsPage
            }

            ComboBox {
                width: parent.width
                currentIndex: config.distanceMeasurement
                label: qsTr("Measurement system")
                menu: ContextMenu {
                    MenuItem { text: qsTr("Metric") }
                    MenuItem { text: qsTr("Imperial US") }
                    MenuItem { text: qsTr("Imperial UK") }
                }
                onCurrentIndexChanged: config.distanceMeasurement = currentIndex
                description: qsTr("Distances are stored internally in metres. If you choose a different measurement system, the application will convert the data on the fly.")
            }

            ComboBox {
                id: clickSoundChoser
                width: parent.width
                currentIndex: config.repetitionClickSound
                label: qsTr("Repetition click sound")
                menu: ContextMenu {
                    MenuItem { text: qsTr("Disabled") }
                    Repeater {
                        model: 9
                        MenuItem {
                            text: qsTr("Sound %1").arg(model.index + 1)
                            onClicked: {
                                clickSoundPlayer.source = "/usr/share/harbour-gibrievida/sounds/sound" + (model.index + 1) + ".wav"
                                clickSoundPlayer.play()
                            }
                        }
                    }
                }
                onCurrentIndexChanged: config.repetitionClickSound = currentIndex
                description: qsTr("The sound will be played if you click on a button or area to increase the repetition count of an acitivity.")
            }

            ComboBox {
                id: finishingSoundChoser
                width: parent.width
                currentIndex: config.finishingSound
                label: qsTr("Finishing sound")
                menu: ContextMenu {
                    MenuItem { text: qsTr("Disabled") }
                    Repeater {
                        model: 4
                        MenuItem {
                            text: qsTr("Sound %1").arg(model.index + 1)
                            onClicked: {
                                clickSoundPlayer.source = "/usr/share/harbour-gibrievida/sounds/finish" + (model.index + 1) + ".oga"
                                clickSoundPlayer.play()
                            }
                        }
                    }
                }
                onCurrentIndexChanged: config.finishingSound = currentIndex
                description: qsTr("The sound will be played if a recording has been finished by using a sensor, like the proximity sensor, and the application ist not visible to the user.")
            }

            ComboBox {
                id: startSoundChoser
                width: parent.width
                currentIndex: config.startSound
                label: qsTr("Start sound")
                menu: ContextMenu {
                    MenuItem { text: qsTr("Disabled") }
                    Repeater {
                        model: 4
                        MenuItem {
                            text: qsTr("Sound %1").arg(model.index + 1)
                            onClicked: {
                                clickSoundPlayer.source = "/usr/share/harbour-gibrievida/sounds/start" + (model.index + 1) + ".oga"
                                clickSoundPlayer.play()
                            }
                        }
                    }
                }
                onCurrentIndexChanged: config.startSound = currentIndex
                description: qsTr("The start sound will be played when a valid initial position has been found via GPS/GLONASS to start a record with distance measurement.")
            }

            ComboBox {
                id: signalLostSoundChoser
                width: parent.width
                currentIndex: config.signalLostSound
                label: qsTr("Signal lost sound")
                menu: ContextMenu {
                    MenuItem { text: qsTr("Disabled") }
                    Repeater {
                        model: 3
                        MenuItem {
                            text: qsTr("Sound %1").arg(model.index + 1)
                            onClicked: {
                                clickSoundPlayer.source = "/usr/share/harbour-gibrievida/sounds/signallost" + (model.index + 1) + ".oga"
                                clickSoundPlayer.play()
                            }
                        }
                    }
                }
                onCurrentIndexChanged: config.signalLostSound = currentIndex
                description: qsTr("The sound will be played if you are recording an activity with distance measurement and there was no valid and accurate position available for more than three minutes. The application will still continue tracking and will also try find a new position. So, this is an informational warning sound.")
            }

            Audio {
                id: clickSoundPlayer
            }

            ComboBox {
                id: langChoser
                label: qsTr("Language")
                description: qsTr("To change the language, you have to restart the application.")
                width: parent.width
                menu: ContextMenu {
                    Repeater {
                        model: LanguageModel {id: langModel }
                        MenuItem { text: model.name; readonly property string value: model.code }
                    }
                }
                onCurrentIndexChanged: { if (currentItem) { config.language = currentItem.value } }
                currentIndex: langModel.findIndex(config.language)
            }
        }
    }
}

