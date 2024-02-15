

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

    width: 250
    height: 50

    text: "My Button"
    display: AbstractButton.TextOnly
    antialiasing: true
    bottomInset: 0
    topInset: 0
    rightPadding: 0
    bottomPadding: 0
    topPadding: 0
    leftPadding: 0

    background: Shape {
        anchors.fill: parent

        ShapePath {
            id: backgroundShape
            strokeWidth: 0
            fillRule: ShapePath.WindingFill
            fillColor: "#e6e6e6"

            startX: 0
            startY: 0
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

        color: "black"

        text: control.text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 20
        anchors.rightMargin: 0
        anchors.topMargin: 12

        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        font.family: "Calibri"
        font.pointSize: 12
    }

    states: [
        State {
            name: "hover"
            when: control.hovered

            PropertyChanges {
                target: backgroundShape
                fillColor: "white"
            }
            PropertyChanges {
                target: textItem
                font.bold: true
            }
        },
        State {
            name: "selected"
            when: control.selected

            PropertyChanges {
                target: backgroundShape
                fillColor: "#1589ff"
            }
            PropertyChanges {
                target: textItem
                color: "white"
                font.pointSize: 14
                font.bold: true
            }
        }
    ]
}
