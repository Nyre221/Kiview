//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import org.kde.kirigami as Kirigami

Item{
    id:fallbackPage
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]

    Kirigami.Icon {
        id: messageIcon
        height: parent.height/3
        width:parent.width/3
        source: "unknown"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height/7
    }

    Text{
        id:headerText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: messageIcon.bottom
        text: i18n("File type not supported.")
        color: Kirigami.Theme.textColor
        font.bold: true
        font.pointSize: 13
    }
}
