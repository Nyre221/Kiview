//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import CManager 1.0
import org.kde.kirigami 2.20 as Kirigami

Loader{
    id: viewersLoader
    anchors.fill: parent

    property list<Kirigami.Action> globalActions

    //these actions are shown in the headerbar of the application.
    globalActions: Kirigami.Action{
        icon.name: "kt-info-widget"
        onTriggered: shortcutsDialog.open()
    }


    //these actions depend on the current viewer and are shown in the application's headerbar.
    property list<Kirigami.Action> actions

    state: Manager.currentViewer
    property var currentItem
    onCurrentItemChanged: {
        fadeIn.stop(); fadeOut.start() }

    onLoaded: { fadeOut.stop(); fadeIn.start(); viewersLoader.actions = combineLists(item.actions,viewersLoader.globalActions)}


    Component {
        id: text_viewer
        TextViewerUi{}
    }

    Component{
        id: image_viewer
        ImageViewerUi{}
    }

    Component{
        id: fallback_viewer
        FallbackViewerUi{}
    }

    Component{
        id: document_viewer
        DocumentViewerUi{}
    }
    Component{
        id: video_viewer
        VideoViewerUi{}
    }

    Component{
        id: container_viewer
        ContainerViewerUI{}
    }

    NumberAnimation on opacity {

        id: fadeOut
        to: 0.0
        duration: 100
        onStopped: { viewersLoader.sourceComponent = currentItem; }
    }
    NumberAnimation on opacity {
        id: fadeIn
        duration: 100
        to: 1.0
    }

    states: [
        State {
            name: "text_viewer"
            PropertyChanges { target: viewersLoader; currentItem: text_viewer}
        },
        State {
            name: "image_viewer"

            PropertyChanges { target: viewersLoader; currentItem: image_viewer}
        },
        State {
            name: "fallback_viewer"

            PropertyChanges { target: viewersLoader; currentItem: fallback_viewer}
        },
        State {
            name: "document_viewer"

            PropertyChanges { target: viewersLoader; currentItem: document_viewer}
        },
        State {
            name: "video_viewer"

            PropertyChanges { target: viewersLoader; currentItem: video_viewer}
        },
        State {
            name: "container_viewer"

            PropertyChanges { target: viewersLoader; currentItem: container_viewer}
        }
    ]

    Kirigami.Dialog {
        id:shortcutsDialog
        title: i18n("Info")
        width: parent.width*0.75
        height: parent.height*0.8
        padding: 10

        property int headerTextSize: 14
        property int shortcutTextSize: 12

        Column {

            Text {
                id: shortcutsText
                text: i18n("Shortcuts")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize +2
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                bottomPadding: 5
                font.bold: true
            }


            Text {
                id: generalText
                text: i18n("General")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                padding: 5
                font.underline: true
            }
            Text {
                id: generalShortcutText
                text: i18n("Arrow keys (Left, Right), A, D:") + " " + i18n("Scroll through files.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere

            }
            Text {
                id: generalShortcutText2
                text: i18n("Enter:") + " " + i18n("Open file in default application.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere
            }
            Text {
                id: documentText
                text: i18n("Documents")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                padding: 5
                font.underline: true
            }
            Text {
                id: documentShortcutText
                text: i18n("Arrow keys (Up, Down), pageUp, pageDown:") + " " + i18n("Scroll between pages.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere
            }
            Text {
                id: videoplayerText
                text: i18n("Video player")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                padding: 5
                font.underline: true
            }
            Text {
                id: videoplayerShortcutText
                text: i18n("Arrow keys (Up, Down):") + " " + i18n("Scroll through the track.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere
            }
            Text {
                id: videoplayerShortcutText2
                text: i18n("E:") +  " " + i18n("pause/play.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere
            }
            Text {
                id: archiveText
                text: i18n("Folders and archives")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                padding: 5
                font.underline: true
            }
            Text {
                id: archiveShortcutText2
                text: i18n("Arrow keys (Up, Down), W, S:") + " " + i18n("Scroll through the list.")
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.shortcutTextSize
                horizontalAlignment: Text.AlignHCenter
                width: shortcutsDialog.width
                wrapMode: Text.WrapAnywhere
            }


            RowLayout{
                spacing: 50
                anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: infoText
                topPadding: 10
                text:  "<a href='https://github.com/Nyre221/Kiview'>" + i18n("How it works") +"</a>"
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignRight

                font.underline: true
                linkColor: Kirigami.Theme.linkColor
                onLinkActivated: Qt.openUrlExternally(link)
            }


            Text {
                id: donateText
                topPadding: 10
                text:  "<a href='https://www.paypal.com/donate/?hosted_button_id=J7QU55MMUP4G4'>" + i18n("Support me") +"</a>"
                color: Kirigami.Theme.textColor
                font.pointSize: shortcutsDialog.headerTextSize
                horizontalAlignment: Text.AlignHCenter
                font.underline: true
                linkColor: Kirigami.Theme.linkColor
                onLinkActivated: Qt.openUrlExternally(link)
            }
            }

        }


        Shortcut {
            enabled: shortcutsDialog.visible
            sequences: [StandardKey.Cancel,"SPACE","q"]
            onActivated: shortcutsDialog.close()
        }
    }



    function combineLists(list1,list2) {
       for (let i = 0; i < list2.length; i++) list1.push(list2[i]);
       return list1;
     }


}
