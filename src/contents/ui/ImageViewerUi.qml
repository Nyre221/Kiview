//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami





Image {
    property bool smoothEnabled: false
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [
        Kirigami.Action{
            id:togleSmooth
            icon.name: "zoom-pixels"
            onTriggered: smoothEnabled = ! smoothEnabled
        },
        OpenAppButton{}]

    id: imageArea
    fillMode: Image.PreserveAspectFit
    source: Manager.imageViewer.viewerImage
    smooth: smoothEnabled


    Text{
        id:errorMessage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible: (Manager.imageViewer.errorMessageText === "" ? false : true)
        text: Manager.imageViewer.errorMessageText
        color: Kirigami.Theme.textColor
        font.pointSize: 15
    }
}
