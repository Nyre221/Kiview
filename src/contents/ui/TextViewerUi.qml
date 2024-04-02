//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
import QtQuick
import QtQuick.Controls
import CManager 1.0
import org.kde.kirigami as Kirigami

ScrollView {
    anchors.fill: parent
    property list<Kirigami.Action> actions
    //these actions are read by viewersLoader and are shown in the "header bar" of the application.
    actions: [OpenAppButton{}]

TextArea {

        id: textArea
        text: Manager.textViewer.viewerText
        placeholderText: qsTr("placeholder")
        readOnly: true

    }

}


