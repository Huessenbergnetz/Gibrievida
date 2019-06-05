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

ListModel {
    ListElement {
        name: "Matthias Fehring (Buschmann)"
        role: QT_TR_NOOP("Main developer, Gibrievida creator")
        section: QT_TR_NOOP("Author")
        image: "buschmann.png"
        website: "http://www.buschmann23.de"
        twitter: "buschmann23"
        github: "buschmann23"
    }

    ListElement {
        name: "Åke Engelbrektson"
        role: QT_TR_NOOP("Swedish translation")
        section: QT_TR_NOOP("Thanks to")
        website: "https://svenskasprakfiler.se"
    }

    ListElement {
        name: "Jean Clovis"
        role: QT_TR_NOOP("French translation")
        section: QT_TR_NOOP("Thanks to")
        image: "jean-clovis.png"
        github: "jean-clovis"
    }

    ListElement {
        name: "Francesco Vaccaro (ghostofasmile)"
        role: QT_TR_NOOP("Italian translation")
        section: QT_TR_NOOP("Thanks to")
    }

    ListElement {
        name: "Nathan Follens"
        role: QT_TR_NOOP("Dutch translation")
        section: QT_TR_NOOP("Thanks to")
    }

    ListElement {
        name: "Stefano (palikao)"
        role: QT_TR_NOOP("Italian translation")
        section: QT_TR_NOOP("Thanks to")
        image: "palikao.png"
        tmo: "64054"
    }
}

