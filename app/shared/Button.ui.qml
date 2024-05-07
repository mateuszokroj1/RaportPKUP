import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Templates as T

import app

T.Button {
    id: root

    property QtObject command
    property double radius: 10

    checkable: false
    enabled: command != null ? command.canExecute : true
    font.family: Theme.fontFamily
    font.pointSize: Theme.fontSize
    text: "Button"

    background: Rectangle {
        id: rectangle

        anchors.fill: root
        color: Theme.controlBackground
        radius: root.radius
    }
    contentItem: RowLayout {
        anchors.fill: root

        Text {
            id: content

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            color: Theme.windowText
            font: root.font
            text: root.text
        }
    }
    states: [
        State {
            name: "flat"
            when: root.flat

            PropertyChanges {
                color: "#00ffffff"
                target: rectangle
            }
        },
        State {
            name: "highlight"
            when: root.highlighted && !root.flat

            PropertyChanges {
                color: Theme.activatedElementBackground
                target: rectangle
            }
            PropertyChanges {
                color: Theme.activatedElementText
                target: content
            }
        }
    ]

    onClicked: {
        if (command != null && command.canExecute)
            command.execute();
    }

    StateGroup {
        id: stateGroup

        states: [
            State {
                name: "disabled"
                when: !root.enabled

                PropertyChanges {
                    color: "#757575"
                    target: content
                }
            }
        ]
    }
}
