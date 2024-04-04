//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami
import QtWebEngine


Item{
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]
    property string htmlLink
    htmlLink: "<a href='https://github.com/Nyre221/Kiview/tree/master/extraStuff/Libreoffice%20troubleshooting'>" + i18n("Look Here!") +"</a>"


    WebEngineView {
        id:webView
        anchors.fill: parent
        focus: true
        url: Manager.documentViewer.viewerDocument
        settings.pluginsEnabled: true
        settings.pdfViewerEnabled: true
        settings.focusOnNavigationEnabled: true
        //hides right-click menu
        onContextMenuRequested: {
            request.accepted = true
        }
        visible: ! messagePage.visible



    }

    Item{
        id:messagePage
        anchors.fill: parent
        visible: Manager.documentViewer.messagePageVisible


        Kirigami.Icon {
            id: messageIcon
            height: parent.height/3
            width:parent.width/3
            source: "libreoffice-master-document"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height/7
        }

        Kirigami.LoadingPlaceholder{
            id: loadingIndicator
            visible: Manager.documentViewer.loadingBarVisible
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: messageIcon.bottom
        }

        Text{
            id:headerText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: messageIcon.bottom
            text: Manager.documentViewer.messageHeaderText
            color: Kirigami.Theme.textColor
            font.bold: true
            font.pointSize: 13
        }

        Text{
            id:descriptionText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: headerText.bottom
            text: (Manager.documentViewer.helpPageLinkVisible ? Manager.documentViewer.messageDescriptionText + " " + htmlLink : Manager.documentViewer.messageDescriptionText)
            color: Kirigami.Theme.textColor
            linkColor: Kirigami.Theme.linkColor
            font.pointSize: 12
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Button{
            id:relaodButton
            visible:  Manager.documentViewer.reloadButtonVisible
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: descriptionText.bottom
            text: i18n("Reload")
            onPressed: Manager.documentViewer.reloadDocument()

            contentItem: Text {
                font.pointSize: 10
                text: relaodButton.text
                color: Kirigami.Theme.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

    }
}
