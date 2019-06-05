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
import de.huessenbergnetz.hbnsc 1.0

AboutPage {

    appTitle: "Gibrievida"
    appDescription: qsTr("Gibrievida is an activity tracker.")
    appHomepage: "https://www.buschmann23.de/entwicklung/anwendungen/gibrievida/"
    appCopyrightYearFrom: "2016"
    appCopyrightYearTo: "2019"
    appCopyrightHolder: "Matthias Fehring"
    appLicense: "GNU General Public License, Version 3"
    appLicenseFile: "GPLv3.qml"

    contributorsModel: ContributorsModel {}
    contributorsAvatarBasePath: "/usr/share/harbour-gibrievida/icons/contributors"

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Buschtrommel/Gibrievida/issues/"

    contactCompany: "Hüssenbergnetz"
    contactName: "Matthias Fehring"
    contactStreet: "Zum Südholz"
    contactHouseNo: "8"
    contactZIP: "34439"
    contactCity: "Willebadessen"
    contactCountry: qsTr("Germany")
    contactEmail: Qt.atob("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU=")
    contactWebsite: "www.huessenbergnetz.de"
    contactWebsiteLink: "https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Gibrievida&pk_kwd=AboutPage"

    bugUrl: "https://github.com/Buschtrommel/Gibrievida/issues"
    translateUrl: "https://www.transifex.com/huessenbergnetz/gibrievida"

//    licensesModel: ListModel {
//        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/juskiddink/sounds/74920/'>Start sound 1</a> by juskiddink is licensed under <a href='https://creativecommons.org/licenses/by/3.0/'>Creative Commons Attribution 3.0 Unported</a>.") }
//        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/Vendarro/sounds/329912/'>Start sound 2</a> by Vendarro is <a href='https://creativecommons.org/publicdomain/zero/1.0/'>public domain</a>.") }
//        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/tec%20studios/sounds/99625/'>Start sound 3</a> by tect studios is <a href='https://creativecommons.org/publicdomain/zero/1.0/'>public domain</a>.") }
//        ListElement { text: QT_TR_NOOP("<a href='https://freesound.org/people/Michel%20Hollicardo/sounds/52593/'>Start sound 4</a> by Michel Hollicardo is licensed under the <a href='https://creativecommons.org/licenses/sampling+/1.0/'>Sampling+ License</a>.") }
//        ListElement { text: QT_TR_NOOP("<a href='http://kde.org'>All other sounds</a> by KDE project are licensed under <a href='http://www.gnu.org/licenses/gpl-2.0.html'>GNU General Public License 2.0</a>.") }
//    }
}

