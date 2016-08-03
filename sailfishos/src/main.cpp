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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#ifdef QT_DEBUG
#include <QtDebug>
#endif

#ifndef CLAZY
#include <sailfishapp.h>
#endif

#include "../common/globals.h"

int main(int argc, char *argv[])
{
    #ifndef CLAZY
        QGuiApplication* app = SailfishApp::application(argc, argv);
    #else
        QGuiApplication* app = new QGuiApplication(argc, argv);
    #endif

//    app->setOrganizationName(QStringLiteral("harbour-arka"));
//    app->setOrganizationDomain(QStringLiteral("buschmann23.de"));
    app->setApplicationName(QStringLiteral("harbour-gibrievida"));
    app->setApplicationDisplayName(QStringLiteral("Gibrievida"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifndef CLAZY
    QQuickView *view = SailfishApp::createView();

    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-gibrievida.qml")));

    view->show();
#endif

    return app->exec();
}

