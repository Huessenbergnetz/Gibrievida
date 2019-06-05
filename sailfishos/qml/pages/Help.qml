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

Page {
    id: helpPage

    SilicaFlickable {
        id: helpPageFlick
        anchors.fill: parent
        contentHeight: helpPageCol.height + Theme.paddingLarge

        VerticalScrollDecorator { flickable: helpPageFlick; page: helpPage }

        Column {
            id: helpPageCol
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                title: qsTr("Help")
                page: helpPage
            }

            SectionHeader {
                text: qsTr("Legend")
            }

            Row {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                spacing: Theme.paddingSmall

                Image {
                    id: repetitionsIcon
                    width: Theme.fontSizeSmall; height: Theme.fontSizeSmall;
                    smooth: true
                    source: "image://theme/icon-s-retweet"
                }

                Text {
                    width: parent.width - repetitionsIcon.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Number of a repetitions saved for a record, for example the number of push-ups you performed. In the activity list this icon indicates that the activity support repetitions.")
                    color: Theme.primaryColor
                }
            }



            Row {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                spacing: Theme.paddingSmall

                Image {
                    id: distanceIcon
                    width: Theme.fontSizeSmall; height: Theme.fontSizeSmall;
                    smooth: true
                    source: "image://theme/icon-cover-transfers"
                }

                Text {
                    width: parent.width - distanceIcon.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Distance you moved while creating the record. In the activity list this icon indicates that the activity support distance saving.")
                    color: Theme.primaryColor
                }
            }

            Row {
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                spacing: Theme.paddingSmall

                Image {
                    id: durationIcon
                    width: Theme.fontSizeSmall; height: Theme.fontSizeSmall;
                    smooth: true
                    source: "image://theme/icon-s-duration"
                }

                Text {
                    width: parent.width - durationIcon.width
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("Duration of the recorded activity.")
                    color: Theme.primaryColor
                }
            }


            SectionHeader { text: qsTr("Concept") }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("Gibrievida wants to help you to record your sportive or daily activities (or whatever you want to use it for). It is based on categories, activities and records. Categories group activities by topic, like sport. An activity is like a template for a record. The record itself is a set of data composed of an activity together with starting and end time, duration and optional repetitions, distance or notes.")
            }

            SectionHeader { text: qsTr("Workflow") }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("At first you need a category. In that category you can add your first activity. After you have your first activity you can start recording. For example, create a category Sport and an activity Push-Ups. In the activity you set a minimum and maximum repetitions count greater than zero to activate repetitions count. Now you are ready to record your daily Push-Ups.")
            }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("If you track an activity that has neither repetitions nor distance measurement, you can close the application completely while doing your stuff. The started activity will be stored to the database and will be loaded when you start the application again.")
            }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("When deleting categories or activities, all connected data will be deleted, too.")
            }

            SectionHeader { text: qsTr("Distance measurement") }

            Text {
                font.pixelSize: Theme.fontSizeSmall
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("By default, GPS/GLONASS is used to track your distance if you record an activity with distance measurement activated. The recording will only start if the application can find a valid initial position.")
            }
        }
    }
}

