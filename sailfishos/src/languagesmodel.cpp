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

#include "languagesmodel.h"

using namespace Gibrievida;

LanguagesModel::LanguagesModel(QObject *parent) :
    Hbnsc::LanguageModel({QStringLiteral("de"),
                         QStringLiteral("en_GB"),
                         QStringLiteral("en_US"),
                         QStringLiteral("fr"),
                         QStringLiteral("it"),
                         QStringLiteral("nl"),
                         QStringLiteral("nl_BE"),
                         QStringLiteral("sv")}, parent)
{

}

LanguagesModel::~LanguagesModel()
{

}
