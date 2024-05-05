import QtQuick 6.2
import QtQuick.Layouts

ColumnLayout {
    antialiasing: true
    spacing: 0

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
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
            height: 20
            renderType: Text.NativeRendering
            text: qsTr("Text Input")
            width: 80
        }
    }
}
