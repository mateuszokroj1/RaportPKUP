import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import logic

Item {
    id: root

    property double arrowsWidth: 35
    default required property list<MainViewItem> items
    readonly property int minimumHeight: items[contentStack.currentIndex].content.minimumHeight
    readonly property int minimumWidth: items[contentStack.currentIndex].content.minimumWidth

    function selectView(index) {
        if (index < 0 || index >= items.length)
            return;
        contentStack.currentIndex = index;
    }

    implicitHeight: menu.implicitHeight + 100
    implicitWidth: menu.width + contentStack.implicitWidth

    Item {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        width: menu.implicitWidth
        z: 1

        Rectangle {
            anchors.fill: parent
            anchors.rightMargin: arrowsWidth
            color: Theme.menuBackground
        }
        ColumnLayout {
            id: menu

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 120
            z: 0

            Repeater {
                model: root.items

                StepSelectorButton {
                    required property int index
                    required property bool isEnabled
                    required property string name

                    Layout.preferredWidth: 320
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

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 320 - arrowsWidth
        anchors.right: parent.right
        anchors.top: parent.top

        Repeater {
            model: root.items

            delegate: Item {
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
