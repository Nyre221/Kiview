//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef DOLPHINBRIDGE_H
#define DOLPHINBRIDGE_H
#include "qdbusconnection.h"
#include "qobject.h"
#include <contentmanager.h>
class DolphinBridge : public QObject
{
    Q_OBJECT

public:
    explicit DolphinBridge(QObject *parent = nullptr);
    void start(ContentManager* Manager, char *argv[], int argc);
    int waitingTime = 25;


private:
    std::tuple<QString,QString> getClipboardContent(QDBusConnection bus);
    std::tuple<QString,QString> getActiveDolphinWindow(QDBusConnection bus);
    QString setClipboardContent(QDBusConnection bus, QString content);
    void restoreClipboardContent(QDBusConnection bus, QString content);
    QString sendCopyFileLocationSignal(QDBusConnection bus, QString dolphinWindow);
    QString sendSelectAllSignal(QDBusConnection bus, QString dolphinWindow);
    QString sendInvertSectionSignal(QDBusConnection bus, QString dolphinWindow);

Q_SIGNALS:

};

#endif // DOLPHINBRIDGE_H
