import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

import logic

Button {
    id: root

    property Command command

    checkable: false
    enabled: command ? command.canExecute : true
    font: Theme.defaultFont
    implicitHeight: implicitContentHeight + topPadding + bottomPadding
    implicitWidth: implicitContentWidth + leftPadding + rightPadding
    padding: Theme.defaultPadding
    text: "Button"

    background: Rectangle {
        id: rectangle

        anchors.fill: root
        border.color: Theme.activatedElementBackground
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
            when: root.flat && !root.enabled

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
            when: root.enabled && root.highlighted && !root.flat

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
            when: !root.enabled && root.highlighted && !root.flat

            PropertyChanges {
                border.color: Theme.disabledText
                color: Theme.disabledBackground
                target: rectangle
            }
        },
        State {
            name: "enabledNonHighlightedHovered"
            when: root.hovered && root.enabled && !root.highlighted && !root.flat

            PropertyChanges {
                color: "white"
                target: rectangle
            }
        }
    ]

    onClicked: {
        if (root.command != null && root.command.canExecute)
            root.command.execute();
    }

    StateGroup {
        id: stateGroup

        states: [
            State {
                name: "disabled"
                when: !root.enabled && !root.highlighted

                PropertyChanges {
                    color: Theme.disabledText
                    target: content
                }
            }
        ]
    }
}
