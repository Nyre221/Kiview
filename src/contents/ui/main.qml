//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import "contents/ui"
import CManager 1.0
import QtQuick.Window 2.0

Kirigami.ApplicationWindow {
    id: root
    title: Manager.windowTitle
    width: Screen.width*0.56
    height: Screen.height*0.6
    minimumHeight: Screen.height*0.4
    minimumWidth: Screen.width*0.35



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

    pageStack.initialPage:kiriPage


    Kirigami.Page{
    id:kiriPage
    padding: 0

    title: Manager.fileName
    contextualActions: viewersLoader.actions


            ViewersLoader{id: viewersLoader}
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

