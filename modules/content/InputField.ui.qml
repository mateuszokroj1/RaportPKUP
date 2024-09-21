import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import logic

FormItemBase {
    property alias focusInside: textField.focus
    property alias inputMethodHints: textField.inputMethodHints
    property alias placeholderText: textField.placeholderText
    property alias validator: textField.validator
    property alias value: textField.text

    implicitHeight: textField.implicitHeight
    implicitWidth: textField.implicitWidth

    states: [
        State {
            name: "InvalidState"
            when: dataState === InputDataState.Invalid

            PropertyChanges {
                border.color: Theme.invalidAccentBorderColor
                target: backRect
            }
        },
        State {
            name: "ValidState"
            when: dataState === InputDataState.Invalid

            PropertyChanges {
                border.color: Theme.validAccentBorderColor
                target: backRect
            }
        }
    ]

    TextField {
        id: textField

        anchors.left: parent.left
        anchors.right: parent.right
        font: Theme.defaultFont
        horizontalAlignment: Qt.AlignLeft
        padding: Theme.defaultPadding

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
