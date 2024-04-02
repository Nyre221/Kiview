import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami

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

