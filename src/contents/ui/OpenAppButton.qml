//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami



Kirigami.Action{
        icon.name: "document-open"
        onTriggered: Manager.openApp()
    }




