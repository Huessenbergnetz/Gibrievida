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

/*
  Types:
  0 - New
  1 - Improved/Enhanced
  2 - Fixed
  3 - Note
*/

ListModel {
    ListElement {
        version: "0.3.1"
        date: 1472721541000
        entries: [
            ListElement { type: 0; issue: ""; description: "Italian translation by ghostofasmile and palikao" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.3.0"
        date: 1472207571000
        entries: [
            ListElement { type: 0; issue: "1"; description: "implement distance measurement via GPS/GLONASS" },
            ListElement { type: 0; issue: ""; description: "Swedish translation by Åke Engelbrektson" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version: "0.2.1"
        date: 1472040309000
        entries: [
            ListElement { type: 2; issue: "11"; description: "subsequent entries are not deleted when deleting categories or activities" }
        ]
    }

    ListElement {
        version: "0.2.0"
        date: 1472024163000
        entries: [
            ListElement { type: 0; issue: "8"; description: "simple database backup by copying SQLite file to backup file" },
            ListElement { type: 0; issue: "7"; description: "optional finishing sound if app is not visible to user" },
            ListElement { type: 0; issue: "6"; description: "optional finish your recording by using the proximity sensor" },
            ListElement { type: 0; issue: "2"; description: "use the acceloremeter to add new repetitions" },
            ListElement { type: 0; issue: "5"; description: "start a new recording directly from the activities list" },
            ListElement { type: 0; issue: "4"; description: "use the proximity sensor to add new repetitions" },
            ListElement { type: 2; issue: "10"; description: "update activities list on finishing a recording" }
        ]
    }

    ListElement {
        version: "0.1.0"
        date: 1471868364000
        entries: [
            ListElement { type: 3; issue: ""; description: "first released version" }
        ]
    }
}

