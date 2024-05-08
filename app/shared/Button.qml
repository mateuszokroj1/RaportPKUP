import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Templates as T

T.Button {
    id: root

    property QtObject command

    checkable: false
    enabled: command != null ? command.canExecute : true
    font: Theme.defaultFont
    implicitHeight: implicitContentHeight + topPadding + bottomPadding
    implicitWidth: implicitContentWidth + leftPadding + rightPadding
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
                    color: Theme.disabledText
                    target: content
                }
            }
        ]
    }
}
