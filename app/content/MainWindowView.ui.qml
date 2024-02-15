import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

GridLayout {
    id: root
    rows: 1
    columns: 2

    required default property list<MainWindowItem> items
    columnSpacing: 25

    Rectangle {
        Layout.maximumWidth: 225
        Layout.preferredWidth: 225
        Layout.fillHeight: true
        color: "#f1f1f1"

        ColumnLayout {
            id: menu
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            width: parent.width + 25

            Repeater {
                model: root.items

                MainWindowSideButton {
                    required property string header
                    text: header

                    required property int index

                    hoverEnabled: !selected
                    selected: contentStack.currentIndex === index
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
                Layout.fillHeight: true
                Layout.fillWidth: true

                required property Item content

                children: [content]
            }
        }
    }
}
