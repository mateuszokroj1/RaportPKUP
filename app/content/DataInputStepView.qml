import QtQuick 6.2
import QtQuick.Layouts

import content

ColumnLayout {
    id: root

    antialiasing: true
    spacing: 0

    RowLayout {
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true
        spacing: 0

        Rectangle {
            anchors.fill: parent
            color: "gray"
            z: -1
        }
        Text {
            text: "Zapisane presety:"
        }
        TextInput {
            id: textInput

            font.pixelSize: 12
            text: qsTr("Text Input")
        }
        UIButton {
        }
    }
}
