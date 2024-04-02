//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtCore
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "contents/ui"
import CManager 1.0
import QtQuick.Window

Kirigami.ApplicationWindow {
    id: root
    title: Manager.windowTitle
    width: Screen.width*0.56
    height: Screen.height*0.6
    minimumHeight: Screen.height*0.4
    minimumWidth: Screen.width*0.35


    Settings {
        property alias width: root.width
        property alias height: root.height
    }



    Shortcut {
        sequences: [StandardKey.Cancel,"SPACE","q"]
        onActivated: Qt.quit()
    }
    Shortcut {
        sequences: ["RETURN"]
        onActivated: Manager.openApp()
    }
    Shortcut {
        sequences: ["a"]
        onActivated: Manager.goBack()
    }
    Shortcut {
        sequences: ["d"]
        onActivated: Manager.goForward()
    }

    pageStack.initialPage: Kirigami.Page{
    id:kiriPage
    padding: 6
    title: Manager.fileName
    actions: viewersLoader.actions



            ViewersLoader{id: viewersLoader}
            // TestComponent{}
            NavButtons{}
            Text{
                id:generalErrorMessage
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                visible: (Manager.dolphinBridgeErrorMessage === "" ? false : true)
                text: Manager.dolphinBridgeErrorMessage
                color: Kirigami.Theme.textColor
                width: parent.width
                wrapMode: Text.WrapAnywhere
                font.pointSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                linkColor: Kirigami.Theme.linkColor
                onLinkActivated: Qt.openUrlExternally(link)
            }
            Text{
                id:generalErrorMessageHtmlText
                anchors.top: generalErrorMessage.bottom
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                visible: (Manager.dolphinBridgeErrorMessage === "" ? false : true)
                text:  "<a href='https://github.com/Nyre221/Kiview'>" + i18n("How it works") +"</a>"
                color: Kirigami.Theme.textColor
                width: parent.width
                wrapMode: Text.WrapAnywhere
                font.pointSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                linkColor: Kirigami.Theme.linkColor
                onLinkActivated: Qt.openUrlExternally(link)
            }

    }

}

