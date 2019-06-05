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
#include <QFile>
#include <QTextStream>
#endif

#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QTranslator>

#ifndef CLAZY
#include <sailfishapp.h>
#endif

#include "hbnsc.h"
#include "hbnsciconprovider.h"

#include "../common/globals.h"
#include "../common/dbmanager.h"
#include "../common/category.h"
#include "../common/categoriescontroller.h"
#include "../common/categoriesfiltermodel.h"
#include "../common/activity.h"
#include "../common/activitiescontroller.h"
#include "../common/activitiesfiltermodel.h"
#include "../common/record.h"
#include "../common/recordscontroller.h"
#include "../common/recordsmodel.h"
#include "../common/helpers.h"
#include "../common/configuration.h"
#include "../common/languagemodel.h"
#include "../common/backupmodel.h"
#include "../common/distancemeasurement.h"


#ifdef QT_DEBUG
void gibrievidaMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    QByteArray time = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss:zzz")).toLocal8Bit();
    QRegularExpression re(QStringLiteral("(\\w*)::"));
    QByteArray className = re.match(QString(context.function)).captured(1).toLocal8Bit();
    QString txt;

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s: %s: Debug: %s\n", time.constData(), className.constData(), localMsg.constData());
        txt = QStringLiteral("%1: %2: Debug: %3").arg(time, className, msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s: %s: Warning: %s\n", time.constData(), className.constData(), localMsg.constData());
        txt = QStringLiteral("%1: %2: Warning: %3").arg(time, className, msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        txt = QStringLiteral("Critical: %1 (%2:%3, %4)").arg(msg, context.file, QString::number(context.line), context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        txt = QStringLiteral("Fatal: %1 (%2:%3, %4)").arg(msg, context.file, QString::number(context.line), context.function);
        abort();
    }

    QFile logFile(QDir::homePath().append(QStringLiteral("/gibrievida.log")));
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&logFile);
    ts << txt << endl;
}
#endif


int main(int argc, char *argv[])
{
#ifndef CLAZY
    std::unique_ptr<QGuiApplication> app(SailfishApp::application(argc, argv));
#else
    auto app = std::make_unique<QGuiApplication>(argc, argv);
#endif

    app->setApplicationName(QStringLiteral(APP_NAME));
    app->setApplicationDisplayName(QStringLiteral("Gibrievida"));
    app->setApplicationVersion(QStringLiteral(VERSION_STRING));

#ifdef QT_DEBUG
    QFile::remove(QDir::homePath().append(QStringLiteral("/gibrievida.log")));
    qInstallMessageHandler(gibrievidaMessageHandler);
#endif

    Gibrievida::DBManager *dbm = new Gibrievida::DBManager();
    QObject::connect(dbm, &QThread::finished, dbm, &QObject::deleteLater);
    dbm->start(QThread::LowPriority);

    Gibrievida::Configuration config;
    if (!config.language().isEmpty()) {
        QLocale::setDefault(QLocale(config.language()));
    }

#ifndef CLAZY
    {
        QLocale loc;
        auto translator = new QTranslator(app.get());
        if (translator->load(loc.name(), SailfishApp::pathTo(QStringLiteral("l10n")).toString(QUrl::RemoveScheme))) {
            app->installTranslator(translator);
        }
    }
#endif
    Hbnsc::loadTranslations();


    qmlRegisterType<Gibrievida::Category>("harbour.gibrievida", 1, 0, "Category");
    qmlRegisterUncreatableType<Gibrievida::CategoriesController>("harbour.gibrievida", 1, 0, "CategoriesController", QStringLiteral("CategoriesController can not be created."));
    qmlRegisterType<Gibrievida::CategoriesFilterModel>("harbour.gibrievida", 1, 0, "CategoriesModel");

    qmlRegisterType<Gibrievida::Activity>("harbour.gibrievida", 1, 0, "Activity");
    qmlRegisterUncreatableType<Gibrievida::ActivitiesController>("harbour.gibrievida", 1, 0, "ActivitiesController", QStringLiteral("ActivitiesController can not be created."));
    qmlRegisterType<Gibrievida::ActivitiesFilterModel>("harbour.gibrievida", 1, 0, "ActivitiesModel");

    qmlRegisterType<Gibrievida::Record>("harbour.gibrievida", 1, 0, "Record");
    qmlRegisterUncreatableType<Gibrievida::RecordsController>("harbour.gibrievida", 1, 0, "RecordsController", QStringLiteral("RecordsController can not be created."));
    qmlRegisterType<Gibrievida::RecordsModel>("harbour.gibrievida", 1, 0, "RecordsModel");

    qmlRegisterType<Gibrievida::LanguageModel>("harbour.gibrievida", 1, 0, "LanguageModel");
    qmlRegisterType<Gibrievida::BackupModel>("harbour.gibrievida", 1, 0, "BackupModel");

    qmlRegisterUncreatableType<Gibrievida::DistanceMeasurement>("harbour.gibrievida", 1, 0, "DistanceMeasurement", QStringLiteral("DistanceMeasurement can not be created."));

#ifndef CLAZY
    std::unique_ptr<QQuickView> view(SailfishApp::createView());
#else
    auto view = std::make_unique<QQuickView>();
#endif

    auto hbnscIconProv = Hbnsc::HbnscIconProvider::createProvider(view->engine());

    Gibrievida::CategoriesController catsController;
    Gibrievida::ActivitiesController actsController;
    Gibrievida::RecordsController recsController(&config);
    Gibrievida::Helpers *helpers = new Gibrievida::Helpers(&config, QLocale(), app.get());

    view->rootContext()->setContextProperty(QStringLiteral("categories"), &catsController);
    view->rootContext()->setContextProperty(QStringLiteral("activities"), &actsController);
    view->rootContext()->setContextProperty(QStringLiteral("records"), &recsController);
    view->rootContext()->setContextProperty(QStringLiteral("helpers"), helpers);
    view->rootContext()->setContextProperty(QStringLiteral("config"), &config);

#ifndef CLAZY
    view->setSource(SailfishApp::pathToMainQml());
#endif

    view->show();

    return app->exec();
}

