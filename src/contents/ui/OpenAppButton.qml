//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick 2.15
import QtQuick.Controls 2.0
import CManager 1.0
import org.kde.kirigami 2.20 as Kirigami



Kirigami.Action{
        icon.name: "document-open"
        onTriggered: Manager.openApp()
    }




