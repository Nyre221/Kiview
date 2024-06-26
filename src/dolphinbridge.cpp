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
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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

    if ( errorGetClipboardContent != QStringLiteral("")){
        //print error
        qDebug() << errorGetClipboardContent;
        Manager->setDolphinBridgeErrorMessage(errorGetClipboardContent);
        return;
    }

    //search for the active dolphin window from which to take the file path.
    const auto[activeDolphinWindow, errorGetActiveDolphinInstance] =  getActiveDolphinWindow(bus);

    if ( errorGetActiveDolphinInstance != QStringLiteral("")){
        //print error
        qDebug() << errorGetActiveDolphinInstance;
        Manager->setDolphinBridgeErrorMessage(errorGetActiveDolphinInstance);
        //closes the program if the second argument indicates that kview was started with the shortcut.
        if (argc > 1){
            std::string firstParamenter = argv[1];
            if (!firstParamenter.compare("--shortcut") ){
                exit(1);
            }
        }

        return;
    }

    //sets the contents of the clipboard to " " in order to clean it from other possible paths copied by the user.
    QString errorSetClipboardContent =  setClipboardContent(bus,QStringLiteral(" "));

    if ( errorSetClipboardContent != QStringLiteral("")){
        //print error
        qDebug() << errorSetClipboardContent;
        Manager->setDolphinBridgeErrorMessage(errorSetClipboardContent);
        return;
    }


    //sends the signal to copy the file location to the clipboard.
    QString copyError = sendCopyFileLocationSignal(bus,activeDolphinWindow);

    if ( copyError != QStringLiteral("")){
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
        //makes X attempts to obtain the path.
        //This is to give slow PCs more opportunity to get the path.
        int times = 5;
        for (int i = 0; i < times; i++) {
            qDebug() << QStringLiteral("attempt:") + QString::number(i+1);
            //if the path does not exist.
            //send the signal select all and try copying the path to the clipboard again.
            QString selectAllError = sendSelectAllSignal(bus,activeDolphinWindow);
            if ( selectAllError != QStringLiteral("")){
                //print error
                qDebug() << selectAllError;
                Manager->setDolphinBridgeErrorMessage(selectAllError);
                //restore clipboard contents.
                restoreClipboardContent(bus,originalClipboardContent);
                return;
            }

            //on Plasma 6 the copy of the file path must be re-enabled if more than one file is selected.
            QDBusInterface dbus_iface(activeDolphinWindow, QStringLiteral("/dolphin/Dolphin_1/actions/copy_location"),QStringLiteral("org.qtproject.Qt.QAction"), bus);
            dbus_iface.call(QStringLiteral("resetEnabled"));
            //copy the path
            sendCopyFileLocationSignal(bus,activeDolphinWindow);

            //to deselect selected files
            sendInvertSectionSignal(bus,activeDolphinWindow);

            //get the path
            auto r3 = getClipboardContent(bus);
            filePath = std::get<0>(r3);
            errorGetClipboardContent = std::get<1>(r3);

            //check if the path is valid.
            if ((std::filesystem::exists(filePath.toStdString()))){
                break;
            }
            else if (i == times-1) {
                qDebug() << "invalid path";
                qDebug() << "path: " + filePath.toStdString();
                //restore clipboard contents.
                restoreClipboardContent(bus,originalClipboardContent);
                Manager->setDolphinBridgeErrorMessage(i18n("Error: Invalid Path"));
                return;
            }
            // increases the delay between functions for the next attempt
            // This is to give slow PCs more opportunity to get the path.
            waitingTime = waitingTime * 3;
        }

        //gets the parent folder.
        //this is necessary because the path points to a file or folder in the parent folder and not to the parent folder.
        std::filesystem::path p = filePath.toStdString();
        filePath = QString().fromStdString(p.parent_path());
    }

    //restore clipboard contents.
    restoreClipboardContent(bus,originalClipboardContent);
    Manager->setFiles(filePath.toStdString(),fileWasSelected);




}

std::tuple<QString,QString> DolphinBridge::getClipboardContent(QDBusConnection bus)
{
    qDebug() << "Sending Get Clipboard Content Signal";
    QDBusInterface dbus_iface(QStringLiteral("org.kde.klipper"), QStringLiteral("/klipper"),QStringLiteral("org.kde.klipper.klipper"), bus);
    auto result = dbus_iface.call(QStringLiteral("getClipboardContents"));
    QString content = result.arguments().at(0).toString();
    QString error = result.errorMessage();
    return std::make_tuple(content,error);

}

QString DolphinBridge::setClipboardContent(QDBusConnection bus,QString content)
{
    qDebug() << "Sending Set Clipboard Content Signal";
    QDBusInterface dbus_iface(QStringLiteral("org.kde.klipper"), QStringLiteral("/klipper"),QStringLiteral("org.kde.klipper.klipper"), bus);
    auto result = dbus_iface.call(QStringLiteral("setClipboardContents"),content);
    QString error = result.errorMessage();
    return error;
}

void DolphinBridge::restoreClipboardContent(QDBusConnection bus, QString content)
{
    // if the user had copied a file before opening the program, restoring just the text is not enough.
    // uses bash commands to restore the mimetype

    std::string s = content.toStdString();
    //the "file:///" prefix indicates that the last item copied to the clipboard by the user was a file or folder.
    //restores content via klipper if it finds blacklisted characters.
    if (! s.rfind("file:///", 0) == 0 || s.rfind(";") != std::string::npos || s.rfind("&") != std::string::npos || s.rfind("'") != std::string::npos || s.rfind("#") != std::string::npos)  {
    //it's plain text, klipper is enough.
        setClipboardContent(bus,content);
        return;
    }

    std::string exec;
    std::string copyCommand;
    //check if it is using wayland or X11
    if (! system("echo $XDG_SESSION_TYPE | grep -i wayland")){
        //wayland
        exec = "wl-copy";
        copyCommand = "wl-copy -t 'text/uri-list' '" + content.toStdString() + "'";
    }
    else if ( ! system("echo $XDG_SESSION_TYPE | grep -E -i 'x11|xorg'")){
        //X11
        exec = "xclip";
        copyCommand = "echo '" + content.toStdString() + "' | xclip -sel clip -t text/uri-list";
    }
    else{
        //error, use klipper
        qDebug() << "Error: Unable to identify session type. Switching to klipper...";
        setClipboardContent(bus,content);
        return;
    }

    //Check if the exec is installed.
    std::string command = "which " + exec;
    if (! system(command.c_str())){
        //run the copyCommand
        if (system(copyCommand.c_str())){
            //failed, use klipper
            qDebug() << QString::fromStdString("Error: Failed to restore clipboard content using: "+exec);
            setClipboardContent(bus,content);
            return;
        }
    }
    else{
        //failed, use klipper
        qDebug() << QString::fromStdString("Error: Failed to restore clipboard content: "+exec+" is not installed");
        setClipboardContent(bus,content);
        return;
    }
}

QString DolphinBridge::sendCopyFileLocationSignal(QDBusConnection bus,QString dolphinWindow)
{
    qDebug() << "Sending Copy File Location Signal";
    //This is to give slow PCs more opportunity to get the path.
    std::this_thread::sleep_for(std::chrono::milliseconds(waitingTime));
    //copy the path
    QDBusInterface dbus_iface2(dolphinWindow, QStringLiteral("/dolphin/Dolphin_1/actions/copy_location"),QStringLiteral("org.qtproject.Qt.QAction"), bus);
    auto result = dbus_iface2.call(QStringLiteral("trigger"));
    QString error = result.errorMessage();
    return error;

}

QString DolphinBridge::sendSelectAllSignal(QDBusConnection bus, QString dolphinWindow)
{
    qDebug() << "Sending Select All Signal";
    QDBusInterface dbus_iface(dolphinWindow, QStringLiteral("/dolphin/Dolphin_1/actions/edit_select_all"),QStringLiteral("org.qtproject.Qt.QAction"), bus);
    auto result = dbus_iface.call(QStringLiteral("trigger"));
    //This is to give slow PCs more opportunity to get the path.
    std::this_thread::sleep_for(std::chrono::milliseconds(waitingTime));
    QString error = result.errorMessage();
    return error;
}

QString DolphinBridge::sendInvertSectionSignal(QDBusConnection bus, QString dolphinWindow)
{
    qDebug() << "Sending Invert Selection Signal";
    QDBusInterface dbus_iface(dolphinWindow, QStringLiteral("/dolphin/Dolphin_1/actions/invert_selection"),QStringLiteral("org.qtproject.Qt.QAction"), bus);
    auto result = dbus_iface.call(QStringLiteral("trigger"));
    QString error = result.errorMessage();
    return error;
}



std::tuple<QString,QString> DolphinBridge::getActiveDolphinWindow(QDBusConnection bus)
{
    QString activeWindow;
    QString errorMessage;
    qDebug() << "Getting Active Window...";
    QDBusReply<QStringList> reply = bus.interface()->registeredServiceNames();
    const QStringList values = reply.value();
    // qDebug() << values;
    for (const QString &name : values){
        if (name.contains(QStringLiteral("dolphin"))){
            QDBusInterface dbus_iface(name, QStringLiteral("/dolphin/Dolphin_1"),QStringLiteral("org.freedesktop.DBus.Properties"), bus);
            auto result = dbus_iface.call(QStringLiteral("Get"),QStringLiteral("org.qtproject.Qt.QWidget"),QStringLiteral("isActiveWindow"));
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
    }

    return std::make_tuple(activeWindow,errorMessage);
}



