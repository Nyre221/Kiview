//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami

Loader{
    id: viewersLoader
    anchors.fill: parent

    property list<Kirigami.Action> globalActions

    //these actions are shown in the headerbar of the application.
    globalActions: Kirigami.Action{
        icon.name: "kt-info-widget"
        onTriggered: shortcutsDialog.open()
    }


    InfoDialog{
    id:shortcutsDialog
    }


    //these actions depend on the current viewer and are shown in the application's headerbar.
    property list<Kirigami.Action> actions

    state: Manager.currentViewer
    property var currentItem
    onCurrentItemChanged: {
        fadeIn.stop(); fadeOut.start() }

    onLoaded: {fadeIn.start(); viewersLoader.actions = combineLists(item.actions,viewersLoader.globalActions)}

    function combineLists(list1,list2) {
       for (let i = 0; i < list2.length; i++) list1.push(list2[i]);
       return list1;
     }


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

}
