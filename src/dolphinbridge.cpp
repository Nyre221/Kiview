//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "dolphinbridge.h"
#include "qapplication.h"
#include <contentmanager.h>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtDBus/QtDBus>
#include <filesystem>
#include <thread>
#include <klocalizedstring.h>




DolphinBridge::DolphinBridge(QObject *parent)
    : QObject{parent}
{


}

void DolphinBridge::start(ContentManager *Manager, char *argv[], int argc)
{

    //used for testing
//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//    if (true){
//        std::string path = "/home/nyre";
//        //        std::string path = "/home/nyre";
//        Manager->setFiles(path,false);
//        return;
//    }
    //used for testing


    //Check if it is connected to the bus.
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << "Unable to connect to the session bus.";
        Manager->setDolphinBridgeErrorMessage(i18n("Unable to connect to the session bus."));
        return;
    }

    QDBusConnection bus = QDBusConnection::sessionBus();

    //used to determine whether the user wants to see a preview of a file or an overall view of the folder.
    bool fileWasSelected = false;
    QString filePath;

    QString errorGetClipboardContent;
    QString originalClipboardContent;
    //is used to restore the contents of the clipboard.
    auto r = getClipboardContent(bus);
    originalClipboardContent = std::get<0>(r);
    errorGetClipboardContent = std::get<1>(r);

    if ( errorGetClipboardContent != ""){
        //print error
        qDebug() << errorGetClipboardContent;
        Manager->setDolphinBridgeErrorMessage(errorGetClipboardContent);
        return;
    }

    //sets the contents of the clipboard to " " in order to clean it from other possible paths copied by the user.
    QString errorSetClipboardContent =  setClipboardContent(bus," ");

    if ( errorSetClipboardContent != ""){
        //print error
        qDebug() << errorSetClipboardContent;
        Manager->setDolphinBridgeErrorMessage(errorSetClipboardContent);
        return;
    }


    //search for the active dolphin window from which to take the file path.
    const auto[activeDolphinWindow, errorGetActiveDolphinInstance] =  getActiveDolphinWindow(bus,argv,argc);

    if ( errorGetActiveDolphinInstance != ""){
        //print error
        qDebug() << errorGetActiveDolphinInstance;
        Manager->setDolphinBridgeErrorMessage(errorGetActiveDolphinInstance);

        return;
    }

    //sends the signal to copy the file location to the clipboard.
    QString copyError = sendCopyFileLocationSignal(bus,activeDolphinWindow);
    if ( copyError != ""){
        //print error
        qDebug() << copyError;
        Manager->setDolphinBridgeErrorMessage(copyError);
        return;
    }

    //get the path
    auto r2 = getClipboardContent(bus);
    filePath = std::get<0>(r2);
    errorGetClipboardContent = std::get<1>(r2);


    if (std::filesystem::exists(filePath.toStdString())){
        //if path exist
        fileWasSelected = true;
    }
    // if there is no valid path in the clipboard, it means that no file was selected.
    else if (! (std::filesystem::exists(filePath.toStdString()))) {
        //if the path does not exist.
        //send the signal select all and try copying the path to the clipboard again.
        QString selectAllError = sendSelectAllSignal(bus,activeDolphinWindow);
        if ( selectAllError != ""){
            //print error
            qDebug() << selectAllError;
            Manager->setDolphinBridgeErrorMessage(selectAllError);
            return;
        }

        sendCopyFileLocationSignal(bus,activeDolphinWindow);
        //to deselect selected files (aesthetic)
        sendInvertSectionSignal(bus,activeDolphinWindow);

        auto r3 = getClipboardContent(bus);
        filePath = std::get<0>(r3);
        errorGetClipboardContent = std::get<1>(r3);
        //check one last time if the path is valid.
        if (! (std::filesystem::exists(filePath.toStdString()))){
            qDebug() << "invalid path";
            Manager->setDolphinBridgeErrorMessage(i18n("Error: Invalid Path"));
            return;
        }

        //gets the parent folder.
        //this is necessary because the path points to a file or folder in the parent folder and not to the parent folder.
        std::filesystem::path p = filePath.toStdString();
        filePath = QString().fromStdString(p.parent_path());
    }

    //restore clipboard contents.
    setClipboardContent(bus,originalClipboardContent);
    Manager->setFiles(filePath.toStdString(),fileWasSelected);




}

std::tuple<QString,QString> DolphinBridge::getClipboardContent(QDBusConnection bus)
{
    QDBusInterface dbus_iface("org.kde.klipper", "/klipper","org.kde.klipper.klipper", bus);
    auto result = dbus_iface.call("getClipboardContents");
    QString content = result.arguments().at(0).toString();
    QString error = result.errorMessage();
    return std::make_tuple(content,error);

}

QString DolphinBridge::setClipboardContent(QDBusConnection bus,QString content)
{
    QDBusInterface dbus_iface("org.kde.klipper", "/klipper","org.kde.klipper.klipper", bus);
    auto result = dbus_iface.call("setClipboardContents",content);
    QString error = result.errorMessage();
    return error;
}

QString DolphinBridge::sendCopyFileLocationSignal(QDBusConnection bus,QString dolphinWindow)
{
    QDBusInterface dbus_iface(dolphinWindow, "/dolphin/Dolphin_1/actions/copy_location","org.qtproject.Qt.QAction", bus);
    auto result = dbus_iface.call("trigger");
    QString error = result.errorMessage();
    return error;
}

QString DolphinBridge::sendSelectAllSignal(QDBusConnection bus, QString dolphinWindow)
{
    QDBusInterface dbus_iface(dolphinWindow, "/dolphin/Dolphin_1/actions/edit_select_all","org.qtproject.Qt.QAction", bus);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto result = dbus_iface.call("trigger");
    QString error = result.errorMessage();
    return error;
}

QString DolphinBridge::sendInvertSectionSignal(QDBusConnection bus, QString dolphinWindow)
{
    QDBusInterface dbus_iface(dolphinWindow, "/dolphin/Dolphin_1/actions/invert_selection","org.qtproject.Qt.QAction", bus);
    auto result = dbus_iface.call("trigger");
    QString error = result.errorMessage();
    return error;
}



std::tuple<QString,QString> DolphinBridge::getActiveDolphinWindow(QDBusConnection bus, char *argv[], int argc)
{
    QString activeWindow;
    QString errorMessage;

    QDBusReply<QStringList> reply = bus.interface()->registeredServiceNames();
    const QStringList values = reply.value();
    for (const QString &name : values){
        if (name.contains("dolphin")){
            QDBusInterface dbus_iface(name, "/dolphin/Dolphin_1","org.freedesktop.DBus.Properties", bus);
            auto result = dbus_iface.call("Get","org.qtproject.Qt.QWidget","isActiveWindow");
            auto qVar =  result.arguments().at(0);
            bool isActive = qVar.value<QDBusVariant>().variant().toBool();
            if (isActive){
                activeWindow = name;
            }
            errorMessage = result.errorMessage();

        }
    }

    if (activeWindow.isEmpty()){
        qDebug() << "No active dolphin window found";
        errorMessage = i18n("No active dolphin window found");

        //closes the program if the second argument indicates that kview was started with the shortcut.
        if (argc > 1){
            std::string firstParamenter = argv[1];
            if (!firstParamenter.compare("-s") ){
                exit(1);
            }
        }


    }



    return std::make_tuple(activeWindow,errorMessage);
}



