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

#include "licensesmodel.h"

using namespace Gibrievida;

LicensesModel::LicensesModel(QObject *parent) : Hbnsc::LicenseModel(parent)
{
    add(QStringLiteral("Gibrievida Translations"),
        QStringLiteral("Gibrievida Translators"),
        QString(),
        QUrl(QStringLiteral("https://www.transifex.com/huessenbergnetz/gibrievida/")),
        tr("The translations for Gibrievida are provided by the community. To see who is responsible for which translation, open the contributors page."),
        QStringLiteral("Creative Commons Attribution 4.0 International Public License"),
        QStringLiteral("CC-BY-4_0.qml"),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/Gibrievida/blob/master/LICENSE.translations")),
        QUrl());

    add(QStringLiteral("Start Sound 1"),
        QStringLiteral("juskiddink"),
        QString(),
        QUrl(QStringLiteral("https://freesound.org/people/juskiddink/sounds/74920/")),
        QString(),
        QStringLiteral("Creative Commons Attribution 3.0 Unported License"),
        QStringLiteral("CC-BY-3_0.qml"),
        QUrl(),
        QUrl());

    add(QStringLiteral("Start Sound 2"),
        QStringLiteral("Vendarro"),
        QString(),
        QUrl(QStringLiteral("https://freesound.org/people/Vendarro/sounds/329912/")),
        QString(),
        QStringLiteral("CC0 1.0 Universal Public Domain Dedication"),
        QStringLiteral("CC0-1_0.qml"),
        QUrl(),
        QUrl());

    add(QStringLiteral("Start Sound 3"),
        QStringLiteral("tect studios"),
        QString(),
        QUrl(QStringLiteral("https://freesound.org/people/tec%20studios/sounds/99625/")),
        QString(),
        QStringLiteral("CC0 1.0 Universal Public Domain Dedication"),
        QStringLiteral("CC0-1_0.qml"),
        QUrl(),
        QUrl());

    add(QStringLiteral("Start Sound 4"),
        QStringLiteral("Michel Hollicardo"),
        QString(),
        QUrl(QStringLiteral("https://freesound.org/people/Michel%20Hollicardo/sounds/52593/")),
        QString(),
        QStringLiteral("Creative Commons Sampling Plus 1.0 Public License"),
        QStringLiteral("CC-SP-1_0.qml"),
        QUrl(),
        QUrl());

    add(QStringLiteral("Other Sounds"),
        QStringLiteral("KDE Project"),
        QString(),
        QUrl(QStringLiteral("http://kde.org")),
        QString(),
        QStringLiteral("GNU General Public License, Version 2"),
        QStringLiteral("GPLv2.qml"),
        QUrl(),
        QUrl());

    addSQLite();

    sortLicenses();
}

LicensesModel::~LicensesModel()
{

}
