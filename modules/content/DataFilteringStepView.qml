import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

import logic

ColumnLayout {
    SplitView {
        orientation: Qt.Horizontal

        Rectangle {
        }
    }
    ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 15
        model: controller.commits

        delegate: Item {
            required property CommitItem commit

            anchors.left: parent.left
            anchors.margins: 15
            anchors.right: parent.right

            ColumnLayout {
            }
        }
    }
}
