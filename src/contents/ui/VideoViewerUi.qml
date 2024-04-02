//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import CManager 1.0
import org.kde.kirigami as Kirigami
import QtWebEngine

// Videos currently fail to play in Qt6/Qml via "QtMultimedia" on some Linux distributions.
// The reason seems to be that you need gstreamer 1.22 with qml6glsink and they are currently not available on kde neon (ubuntu 22.04).
Item{
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]

    WebEngineView {
        id:webView
        anchors.fill: parent
        focus: true
        settings.fullScreenSupportEnabled: false
        url: Manager.videoViewer.viewerMedia
        // //hides right-click menu
        onContextMenuRequested: {
            request.accepted = true
        }
    }
}
