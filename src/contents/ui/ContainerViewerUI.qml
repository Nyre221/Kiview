//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import CManager 1.0
Item{
    id:containerViewer
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]

    Item{
        id:infoPanel
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width:parent.width/4

        Kirigami.Icon {
            id: fileIcon
            height: 200
            width:parent.width
            source: Manager.containerViewer.fileIcon
            anchors.top: parent.top
            anchors.topMargin: parent.height/15
        }

        Column{
            id:infoArea
            anchors.top: fileIcon.bottom
            width: parent.width
            property int fontSizeText: 13
            property int fontSizeValue: 12
            property int separation: 15

            Column{
                id: sizeArea
                visible: (Manager.containerViewer.fileSize === "" ? false : true)
                width: parent.width
                topPadding: infoArea.separation

                Text{
                    id: sizeText
                    text: i18n("Size")
                    font.pointSize: infoArea.fontSizeText
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.bold: true
                }

                Text{
                    id: sizeValue
                    text: Manager.containerViewer.fileSize + " " +  Manager.containerViewer.fileSizeUnit
                    font.pointSize: infoArea.fontSizeValue
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.underline: true

                }

            }

            Column{
                id: lastModifiedArea
                visible: (Manager.containerViewer.fileLastModified === "" ? false : true)
                width: parent.width
                topPadding: infoArea.separation
                Text{
                    id: lastModifiedText
                    text: i18n("Last Modified")
                    font.pointSize: infoArea.fontSizeText
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.bold: true

                }

                Text{
                    id: lastModifiedValue
                    text:Manager.containerViewer.fileLastModified
                    font.pointSize: infoArea.fontSizeValue
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.underline: true

                }

            }
            Column{
                id: fileCountArea
                visible: (Manager.containerViewer.fileContentCount === "" ? false : true)
                width: parent.width
                topPadding: infoArea.separation
                Text{
                    id: countText
                    text: i18n("File Count")
                    font.pointSize: infoArea.fontSizeText
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.bold: true

                }

                Text{
                    id: countValue
                    text: Manager.containerViewer.fileContentCount
                    font.pointSize: infoArea.fontSizeValue
                    color: Kirigami.Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    font.underline: true


                }

            }

        }

    }


    Kirigami.Separator{
        id: lineSeparator
        anchors.left: infoPanel.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
    }

    Rectangle{
        id:listBackground
        anchors.fill: contentList
        color: Kirigami.Theme.alternateBackgroundColor
        anchors.topMargin: 0
    }




    ListView {
        id: contentList
        anchors.left: lineSeparator.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        model: Manager.containerViewer.viewerElement


        Shortcut {
            sequences: ["w","Up"]
            onActivated:  if (contentList.currentIndex > 0)  contentList.currentIndex = contentList.currentIndex -1
        }
        Shortcut {
            sequences: ["Down","s"]
            onActivated: if (contentList.count > contentList.currentIndex+1)  contentList.currentIndex = contentList.currentIndex +1
        }


        delegate: ItemDelegate {
            //from c++ the string "!isDir!" is added to know in qml whether it is a folder or not.
            // remove the attribute from the string
            text: modelData.replace(/(^!isDir!)/gi, "")
            //chooses the icon suitable for the file type.
            icon.name: iconChooser(modelData)
            width: parent.width
            property int currentIndex :contentList.currentIndex
            onCurrentIndexChanged:highlighted = (contentList.currentIndex === index)
            onClicked:contentList.currentIndex = index
        }


        Text{
            id:errorMessage
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode:Text.WrapAnywhere
            width: parent.width * 0.9
            visible: (Manager.containerViewer.errorMessageText === "" ? false : true)
            text: Manager.containerViewer.errorMessageText
            color: Kirigami.Theme.textColor
            font.pointSize: 15
            horizontalAlignment: Text.AlignHCenter
        }
    }


    function iconChooser(string){
        var icon = "unknown"
        var extension = string.split('.').pop()

        if (string.startsWith("!isDir!")){
            //from c++ the string "!isDir!" is added to know in qml whether it is a folder or not.
            icon = "folder-blue"
        }
        else if (["zip", "gz", "rar", "pyz"].includes(extension)){
            icon = "application-zip"
        }
        else if (["pdf"].includes(extension)){
            icon = "application-pdf"
        }
        else if (["png", "jpg", "jpeg", "webp", "svg", "svgz","jxl"].includes(extension)){
            icon = "image-png"
        }
        else if (["mp4"].includes(extension)){
            icon = "video-mp4"
        }
        else if (["mp3"].includes(extension)){
            icon = "audio-mp3"
        }
        else if (["sh"].includes(extension)){
            icon = "application-x-shellscript"
        }
        else if (["py"].includes(extension)){
            icon = "text-x-python"
        }
        else if (["ccp","h"].includes(extension)){
            icon = "text-x-c++src"
        }
        else if (["txt"].includes(extension)){
            icon = "text-x-log"
        }
        else if (["html", "htm"].includes(extension)){
            icon = "text-html"
        }
        else if (["doc", "docx", "odt", "ods", "xlsx", "xls", "csv"].includes(extension)){
            icon = "application-vnd.oasis.opendocument.text"
        }
        else if (["iso", "img"].includes(extension)){
            icon = "application-x-iso"
        }
        else if (["kra"].includes(extension)){
            icon = "application-x-krita"
        }

        return icon
    }
}

