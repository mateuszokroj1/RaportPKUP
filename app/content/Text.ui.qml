import QtQuick 6.2 as SYS

SYS.Item {
    property alias color: contentItem.color
    property alias font: contentItem.font
    property alias text: contentItem.text

    implicitHeight: contentItem.implicitHeight
    implicitWidth: contentItem.implicitWidth

    SYS.Text {
        id: contentItem

        color: Theme.windowText
        font: Theme.defaultFont
        text: "Text"
    }
}
