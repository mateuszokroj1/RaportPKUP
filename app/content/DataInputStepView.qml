import QtQuick 6.2
import QtQuick.Layouts

import shared as UI

ColumnLayout {
    id: root

    antialiasing: true
    spacing: 0

    RowLayout {
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "gray"
            z: -1
        }
        UI.Text {
            text: "Zapisane presety:"
        }
        UI.InputField {
            id: textInput

            font.pixelSize: 12
            text: qsTr("Text Input")
        }
        UI.Button {
        }
        UI.Button {
        }
    }
}
