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
#include "../common/dbmanager.h"
#include "../common/categoriescontroller.h"
#include "../common/categoriesfiltermodel.h"
#include "../common/activitiescontroller.h"
#include "../common/activitiesfiltermodel.h"
#include "../common/recordsmodel.h"

int main(int argc, char *argv[])
{
#ifndef CLAZY
    QGuiApplication* app = SailfishApp::application(argc, argv);
#else
    QGuiApplication* app = new QGuiApplication(argc, argv);
#endif

    app->setApplicationName(QStringLiteral(APP_NAME));
    app->setApplicationDisplayName(QStringLiteral("Gibrievida"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

    Gibrievida::DBManager *dbm = new Gibrievida::DBManager();
    QObject::connect(dbm, &QThread::finished, dbm, &QObject::deleteLater);
    dbm->start(QThread::LowPriority);

    qmlRegisterUncreatableType<Gibrievida::CategoriesController>("harbour.gibrievida", 1, 0, "CategoriesController", QStringLiteral("CategoriesController can not be created."));
    qmlRegisterType<Gibrievida::CategoriesFilterModel>("harbour.gibrievida", 1, 0, "CategoriesModel");

    qmlRegisterUncreatableType<Gibrievida::ActivitiesController>("harbour.gibrievida", 1, 0, "ActivitiesController", QStringLiteral("ActivitiesController can not be created."));
    qmlRegisterType<Gibrievida::ActivitiesFilterModel>("harbour.gibrievida", 1, 0, "ActivitiesModel");

    qmlRegisterType<Gibrievida::RecordsModel>("harbour.gibrievida", 1, 0, "RecordsModel");

#ifndef CLAZY
    QQuickView *view = SailfishApp::createView();
#else
    QQuickView *view = new QQuickView();
#endif

    Gibrievida::CategoriesController catsController;
    Gibrievida::ActivitiesController actsController;

    view->rootContext()->setContextProperty(QStringLiteral("categories"), &catsController);
    view->rootContext()->setContextProperty(QStringLiteral("activities"), &actsController);

#ifndef CLAZY
    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/harbour-gibrievida.qml")));
#endif

    view->show();

    return app->exec();
}

