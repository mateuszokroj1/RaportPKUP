import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

Button {
    id: root

    property QtObject command

    checkable: false
    enabled: command != null ? command.canExecute : true
    font: Theme.defaultFont
    implicitHeight: implicitContentHeight + topPadding + bottomPadding + 2
    implicitWidth: implicitContentWidth + leftPadding + rightPadding + 2
    padding: Theme.defaultPadding
    text: "Button"

    background: Rectangle {
        id: rectangle

        anchors.fill: root
        border.color: Theme.windowText
        border.width: 1
        color: Theme.controlBackground
        radius: Theme.radius
    }
    contentItem: Text {
        id: content

        color: Theme.windowText
        font: root.font
        text: root.text
    }
    states: [
        State {
            name: "flat"
            when: root.flat

            PropertyChanges {
                color: "transparent"
                target: rectangle
            }
            PropertyChanges {
                border.width: 0
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
        },
        State {
            name: "disabledNonFlat"
            when: root.highlighted && !root.flat

            PropertyChanges {
                border.color: Theme.disabledText
                color: Theme.disabledBackground
                target: rectangle
            }
        }
    ]

    StateGroup {
        id: stateGroup

        states: [
            State {
                name: "disabled"
                when: !root.enabled

                PropertyChanges {
                    color: Theme.disabledText
                    target: content
                }
            }
        ]
    }
    Connections {
        function onClicked() {
            if (command != null && command.canExecute)
                command.execute();
        }

        target: root
    }
}
