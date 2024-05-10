import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Item {
    height: 100
    width: 100

    TextField {
        id: root

        property int dataState: InputDataState.Indeterminate

        font: Theme.defaultFont
        horizontalAlignment: "AlignLeft"
        padding: Theme.defaultPadding
        placeholderText: "Text Box"
        text: "t"

        background: Rectangle {
            id: backRect

            anchors.fill: root
            border.color: Theme.activatedElementBackground
            border.width: 1
            color: Theme.windowBackground
            radius: Theme.radius
        }
    }
}
