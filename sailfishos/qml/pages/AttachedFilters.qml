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
    id: filtersPage

    property RecordsModel recModel: null

    property string oldOrder
    property string oldOrderBy

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            oldOrder = recModel.order
            oldOrderBy = recModel.orderBy
        }

        if (status === PageStatus.Deactivating && (oldOrder !== recModel.order || oldOrderBy !== recModel.orderBy)) {
            recModel.update()
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: sortingCol.height + Theme.paddingLarge

        Column {
            id: sortingCol
            width: parent.width

            PageHeader {
                page: filtersPage
                title: qsTr("Sorting")
//                description: qsTr("Sorting will be applied after moving back to list.")
            }

//            Label {
//                text:  qsTr("Sorting")
//                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
//                horizontalAlignment: Text.AlignRight
//                color: Theme.highlightColor
//            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                text: qsTr("Sorting will be applied after moving back to list.")
                wrapMode: Text.WordWrap
            }

            SectionHeader { text: qsTr("Sort by") }

            TextSwitch { text: qsTr("Start time"); automaticCheck: false; onClicked: recModel.orderBy = "r.start"; checked: recModel.orderBy === "r.start" }
            TextSwitch { text: qsTr("Duration"); automaticCheck: false; onClicked: recModel.orderBy = "r.duration"; checked: recModel.orderBy === "r.duration" }
            TextSwitch { text: qsTr("Repetitions"); automaticCheck: false; onClicked: recModel.orderBy = "r.repetitions"; checked: recModel.orderBy === "r.repetitions" }
            TextSwitch { text: qsTr("Distance"); automaticCheck: false; onClicked: recModel.orderBy = "r.distance"; checked: recModel.orderBy === "r.distance" }

            SectionHeader { text: qsTr("Sort order") }

            TextSwitch { text: qsTr("Ascending"); automaticCheck: false; onClicked: recModel.order = "ASC"; checked: recModel.order === "ASC" }
            TextSwitch { text: qsTr("Descending"); automaticCheck: false; onClicked: recModel.order = "DESC"; checked: recModel.order === "DESC" }
        }
    }
}

