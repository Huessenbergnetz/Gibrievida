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
    appDescription: qsTr("Gibrievida is an actvitiy tracker.")
    appHomepage: "http://gibrievida.buschmann23.de"
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

}

