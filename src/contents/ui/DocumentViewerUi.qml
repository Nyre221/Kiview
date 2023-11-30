//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick 2.15
import QtQuick.Pdf 5.15
import QtQuick.Controls 2.0
import CManager 1.0
import org.kde.kirigami 2.20 as Kirigami


Item{
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]
    property string htmlLink
    htmlLink: "<a href='https://github.com/Nyre221/Kiview/tree/main/extraStuff/Libreoffice%20troubleshooting'>" + i18n("Look Here!") +"</a>"



    PdfMultiPageView {
        id:docView
        anchors.fill: parent
        //the margins are used to avoid covering the navigation arrows.
        anchors.rightMargin: 25
        anchors.leftMargin: 25
        //a workaround for a bug on this version of pdfpageview.
        //It allows to have the page re-centered if you resize the window.
        //scaleToWidth and scaleToPage do not work with some PDFs and tend to make the document disappear on qt5.
        onWidthChanged: if (currentPageRenderingStatus == 1)  (doc.source = "" , doc.source = Manager.documentViewer.viewerDocument)
        //go to the first page and sets the zoom level
        onCurrentPageRenderingStatusChanged:if (currentPageRenderingStatus == 1) ( docView.goToLocation(docView.currentPage,0,1.2))
        document: doc


        PdfDocument {
            id: doc
            source: Manager.documentViewer.viewerDocument
        }

        Shortcut {
            sequences: ["up",StandardKey.MoveToPreviousPage]
            onActivated: if (docView.currentPage > 0) docView.goToPage(docView.currentPage-1)

        }

        Shortcut {
            sequences: ["down",StandardKey.MoveToNextPage]
            onActivated: if (docView.currentPage < doc.pageCount-1) docView.goToPage(docView.currentPage+1)
        }
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
