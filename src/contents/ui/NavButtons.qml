//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami


Item{
    anchors.fill: parent
    anchors.leftMargin: 5
    anchors.rightMargin: 5

    Button {
        id:buttonBack
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        flat: true
        action: Kirigami.Action {
            icon.name: "arrow-left"
            shortcut: "Left"
            onTriggered: Manager.goBack()
        }
    }

    Button {
        id:buttonForward
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        flat: true
        action: Kirigami.Action {
            icon.name: "arrow-right"
            shortcut: "Right"
            onTriggered: Manager.goForward()
        }
    }

}
