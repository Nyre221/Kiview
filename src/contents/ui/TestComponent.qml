import QtQuick
import QtQuick.Controls as Controls
import org.kde.kirigami  as Kirigami
import CManager 1.0

    ListView {
        id: contentList
        anchors.fill: parent
        model: Manager.containerViewer.viewerElement


        delegate: Controls.ItemDelegate {
            //from c++ the string "!isDir!" is added to know in qml whether it is a folder or not.
            // remove the attribute from the string
            text: modelData.replace(/(^!isDir!)/gi, "")
            //chooses the icon suitable for the file type.
            // icon.name: iconChooser(modelData)
            width: parent.width
            property int currentIndex :contentList.currentIndex
            onCurrentIndexChanged:highlighted = (contentList.currentIndex === index)
            onClicked:contentList.currentIndex = index
        }



    }
