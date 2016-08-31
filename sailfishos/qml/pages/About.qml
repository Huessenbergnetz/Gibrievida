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
import "../btcomponents"

AboutPage {

    appTitle: "Gibrievida"
    appVersion: Qt.application.version
    appDescription: qsTr("Gibrievida is an activity tracker.")
    appHomepage: "https://www.buschmann23.de/entwicklung/anwendungen/gibrievida/"
    appCopyright: "© 2016, Buschtrommel"
    appLicense: qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-3.0.en.html'>GNU GPL v3</a>")

    contributorsModel: ContributorsModel {}
    contributorsAvatarBasePath: "/usr/share/harbour-gibrievida/icons/contributors"

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Buschtrommel/Gibrievida/issues/"

    contactCompany: "Buschtrommel"
    contactName: "Matthias Fehring"
    contactStreet: "Engelhardstraße"
    contactHouseNo: "32"
    contactZIP: "81369"
    contactCity: qsTr("Munich")
    contactCountry: qsTr("Germany")
    contactEmail: "kontakt@buschmann23.de"
    contactWebsite: "www.buschmann23.de"

    bugUrl: "https://github.com/Buschtrommel/Gibrievida/issues"
    translateUrl: "https://www.transifex.com/buschtrommel/gibrievida"

    licensesModel: ListModel {
        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/juskiddink/sounds/74920/'>Start sound 1</a> by juskiddink is licensed under <a href='https://creativecommons.org/licenses/by/3.0/'>Creative Commons Attribution 3.0 Unported</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/Vendarro/sounds/329912/'>Start sound 2</a> by Vendarro is <a href='https://creativecommons.org/publicdomain/zero/1.0/'>public domain</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/tec%20studios/sounds/99625/'>Start sound 3</a> by tect studios is <a href='https://creativecommons.org/publicdomain/zero/1.0/'>public domain</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/Michel%20Hollicardo/sounds/52593/'>Start sound 4</a> by Michel Hollicardo is licensed under the <a href='https://creativecommons.org/licenses/sampling+/1.0/'>Sampling+ License</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='http://kde.org'>All other sounds</a> by KDE project are licensed under <a href='http://www.gnu.org/licenses/gpl-2.0.html'>GNU General Public License 2.0</a>.") }
    }
}

