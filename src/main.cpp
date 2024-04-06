//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <contentmanager.h>
#include <dolphinbridge.h>
#include <QIcon>
#include <iostream>
#include <QtWebEngineQuick>


int main(int argc, char *argv[])
{
    QtWebEngineQuick::initialize();
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kiview");
    QCoreApplication::setOrganizationName(QStringLiteral("nyre"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nyre221.github.io"));
    QCoreApplication::setApplicationName(QStringLiteral("Kiview"));



    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    ContentManager Manager;
    //to get information from dolphin.
    DolphinBridge*  bridge = new DolphinBridge;
    bridge->start(&Manager,argv,argc);

    //registers ContentManager as a singleton so it can be accessed from qml.
    qmlRegisterSingletonInstance<ContentManager>("CManager", 1, 0, "Manager", &Manager);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


//    qDebug() << QLocale::system();
//    qDebug() << KLocalizedString::isApplicationTranslatedInto(QString("it"));
//    qDebug() << KLocalizedString::availableApplicationTranslations();
//    qDebug() << KLocalizedString::languages();
//    qDebug() << KLocalizedString::availableDomainTranslations("kiview");


    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QObject::connect(&app, SIGNAL(aboutToQuit()), &Manager, SLOT(closing()));
    //sets the icon of the app
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("io.github.nyre221.kiview")));


    return app.exec();
}
