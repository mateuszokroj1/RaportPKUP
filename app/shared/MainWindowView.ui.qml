import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Main
import app

RowLayout {
    id: root

    default required property list<MainViewItem> items

    spacing: 25

    ColumnLayout {
        id: menu

        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillHeight: true
        width: 280

        Repeater {
            model: root.items
            width: 280

            MainWindowSideButton {
                required property int index
                required property bool isEnabled
                required property string name

                anchors.left: menu.left
                anchors.right: menu.right
                checked: contentStack.currentIndex === index
                enabled: isEnabled
                text: name

                onClicked: contentStack.currentIndex = enabled ? index : contentStack.currentIndex
            }
        }
    }
    StackLayout {
        id: contentStack

        Layout.fillHeight: true
        Layout.fillWidth: true

        Repeater {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: root.items

            Item {
                required property Item content

                Layout.fillHeight: true
                Layout.fillWidth: true
                children: [content]
            }
        }
    }
}
