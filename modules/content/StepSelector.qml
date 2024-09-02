import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import logic

RowLayout {
    id: root

    property double arrowsWidth: 35
    default required property list<MainViewItem> items

    function selectView(index) {
        if (index < 0 || index >= items.length)
            return;
        contentStack.currentIndex = index;
    }

    implicitHeight: menu.implicitHeight + 100
    implicitWidth: menu.width + contentStack.implicitWidth
    spacing: Theme.defaultPadding

    Item {
        Layout.fillHeight: true
        implicitHeight: menu.implicitHeight
        implicitWidth: menu.implicitWidth

        Rectangle {
            anchors.fill: parent
            anchors.rightMargin: arrowsWidth
            color: Theme.menuBackground
            z: -1
        }
        ColumnLayout {
            id: menu

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: Theme.defaultPadding

            Repeater {
                model: root.items

                StepSelectorButton {
                    required property int index
                    required property bool isEnabled
                    required property string name

                    Layout.preferredWidth: 300
                    arrowWidth: arrowsWidth
                    checked: contentStack.currentIndex === index
                    enabled: isEnabled
                    padding: 20
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
                implicitHeight: content.implicitHeight
                implicitWidth: content.implicitWidth
            }
        }
    }
}
