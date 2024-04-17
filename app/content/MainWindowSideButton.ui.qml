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

Button {
    id: control

    property bool selected: false

    antialiasing: true
    bottomInset: 0
    bottomPadding: 0
    display: AbstractButton.TextOnly
    height: 50
    leftPadding: 0
    rightPadding: 0
    text: "My Button"
    topInset: 0
    topPadding: 0
    width: 250

    background: Shape {
        anchors.fill: parent

        ShapePath {
            id: backgroundShape

            fillColor: "#e6e6e6"
            fillRule: ShapePath.WindingFill
            startX: 0
            startY: 0
            strokeWidth: 0

            PathLine {
                x: 0
                y: 0
            }
            PathLine {
                x: 225
                y: 0
            }
            PathLine {
                x: 250
                y: 25
            }
            PathLine {
                x: 225
                y: 50
            }
            PathLine {
                x: 0
                y: 50
            }
            PathLine {
                x: 0
                y: 0
            }
        }
    }
    contentItem: Text {
        id: textItem

        Layout.fillWidth: true
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 12
        color: "black"
        font.family: "Calibri"
        font.pointSize: 12
        horizontalAlignment: Text.AlignLeft
        text: control.text
        verticalAlignment: Text.AlignTop
    }
    states: [
        State {
            name: "hover"
            when: control.hovered

            PropertyChanges {
                fillColor: "white"
                target: backgroundShape
            }
            PropertyChanges {
                font.bold: true
                target: textItem
            }
        },
        State {
            name: "selected"
            when: control.selected

            PropertyChanges {
                fillColor: "#1589ff"
                target: backgroundShape
            }
            PropertyChanges {
                color: "white"
                font.bold: true
                font.pointSize: 14
                target: textItem
            }
        }
    ]
}
