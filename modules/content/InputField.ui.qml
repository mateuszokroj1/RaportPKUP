import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

FormItemBase {
    implicitHeight: textField.implicitHeight
    implicitWidth: textField.implicitWidth

    TextField {
        id: textField

        anchors.fill: parent
        font: Theme.defaultFont
        horizontalAlignment: "AlignLeft"
        padding: Theme.defaultPadding
        placeholderText: "Text Box"

        background: Rectangle {
            id: backRect

            anchors.fill: textField
            border.color: Theme.activatedElementBackground
            border.width: 1
            color: Theme.windowBackground
            radius: Theme.radius
        }
    }
}
