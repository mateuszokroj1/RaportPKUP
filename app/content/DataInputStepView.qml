import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: root

    antialiasing: true
    spacing: 0

    //  Item {
    //      Layout.alignment: Qt.AlignTop
    //     Layout.fillWidth: true
    //
    //      implicitWidth: topMenu.implicitWidth
    //       implicitHeight: topMenu.implicitHeight

    //      Rectangle
    //      {
    //          anchors.fill: parent
    //          z: -1
    //           color: UI.Theme.menuBackground
    //      }

    RowLayout {
        id: topMenu

        spacing: Theme.defaultPadding

        Text {
            Layout.margins: Theme.defaultPadding
            color: Theme.windowText
            font: Theme.defaultFont
            text: "Ustawienia wstępne:"
        }
    }
    //  }
}
