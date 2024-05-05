import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Main
import app

GridLayout {
    id: root

    default required property list<MainViewItem> items

    columnSpacing: 25
    columns: 2
    rows: 1

    Rectangle {
        Layout.fillHeight: true
        Layout.preferredWidth: 200
        color: Theme.menuBackground

        ColumnLayout {
            id: menu

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            width: parent.width + 25

            Repeater {
                model: root.items

                MainWindowSideButton {
                    required property int index
                    required property bool isEnabled
                    required property string name

                    checked: contentStack.currentIndex === index
                    enabled: isEnabled
                    text: name

                    onClicked: contentStack.currentIndex = enabled ? index : contentStack.currentIndex
                }
            }
        }
    }
    StackLayout {
        id: contentStack

        Layout.fillHeight: true
        Layout.fillWidth: true

        Repeater {
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
