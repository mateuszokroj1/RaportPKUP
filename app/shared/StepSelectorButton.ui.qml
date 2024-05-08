/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Shapes
import QtQuick.Templates as T

T.Button {
    id: control

    property double arrowWidth: 25

    antialiasing: true
    implicitHeight: implicitContentHeight + topPadding + bottomPadding
    implicitWidth: implicitContentWidth + leftPadding + rightPadding + arrowWidth
    padding: Theme.defaultPadding
    text: "My Button"

    background: RowLayout {
        id: backgroundLayout

        property color backgroundColor: "transparent"

        anchors.margins: 0
        antialiasing: true
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            antialiasing: true
            color: backgroundLayout.backgroundColor
        }
        Shape {
            Layout.fillHeight: true
            antialiasing: true

            ShapePath {
                id: arrowShape

                fillColor: backgroundLayout.backgroundColor
                //                fillRule: ShapePath.
                startX: 0
                startY: 0
                strokeColor: "#00ffffff"
                strokeWidth: 0

                PathLine {
                    x: 0
                    y: 0
                }
                PathLine {
                    x: control.arrowWidth
                    y: control.height / 2
                }
                PathLine {
                    x: 0
                    y: control.height
                }
            }
        }
    }
    contentItem: RowLayout {
        Text {
            id: textItem

            color: Theme.windowText
            font: Theme.defaultFont
            //font.pointSize: Theme.fontSize - 1
            horizontalAlignment: Text.AlignLeft
            text: control.text
            verticalAlignment: Text.AlignVCenter
        }
    }
    states: [
        State {
            name: "hover"
            when: control.hovered && !control.checked && control.enabled

            PropertyChanges {
                backgroundColor: Theme.highlightedElementBackground
                target: backgroundLayout
            }
            PropertyChanges {
                font.bold: true
                target: textItem
            }
        },
        State {
            name: "checked"
            when: control.checked && control.enabled

            PropertyChanges {
                backgroundColor: Theme.activatedElementBackground
                target: backgroundLayout
            }
            PropertyChanges {
                color: Theme.highlightedElementBackground
                font.bold: true
                font.pointSize: Theme.fontSize + 1
                target: textItem
            }
        },
        State {
            name: "disabled"
            when: !control.enabled

            PropertyChanges {
                color: Theme.disabledText
                target: textItem
            }
        }
    ]
}
