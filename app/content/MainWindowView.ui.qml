import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

GridLayout {
    id: root

    default required property list<MainWindowItem> items

    columnSpacing: 25
    columns: 2
    rows: 1

    Rectangle {
        Layout.fillHeight: true
        Layout.maximumWidth: 225
        Layout.preferredWidth: 225
        color: "#f1f1f1"

        ColumnLayout {
            id: menu

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            width: parent.width + 25

            Repeater {
                model: root.items

                MainWindowSideButton {
                    required property string header
                    required property int index

                    hoverEnabled: !selected
                    selected: contentStack.currentIndex === index
                    text: header

                    onClicked: contentStack.currentIndex = index
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
