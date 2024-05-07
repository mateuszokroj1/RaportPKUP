import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import app
import Main

TextField {
    id: root

    property InputDataState dataState: InputDataState.Indeterminate

    font.family: Theme.fontFamily
    font.pointSize: Theme.fontSize
    horizontalAlignment: "AlignLeft"
    implicitHeight: contentHeight + topPadding + bottomPadding
    implicitWidth: 100
    padding: Theme.defaultPadding
    placeholderText: "Text Box"

    background: Rectangle {
        id: backRect

        anchors.fill: root
        border.color: Theme.activatedElementBackground
        border.width: 1
        color: Theme.windowBackground
    }
    states: [
        State {
            name: ""

            PropertyChanges {
                target: object
            }
        }
    ]
}
