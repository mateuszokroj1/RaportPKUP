import QtQuick 6.2
import QtQuick.Layouts

Item {
    id: root

    default property list<Item> items
    property int orientation: Qt.Horizontal

    Rectangle {
        anchors.fill: root
        color: Theme.menuBackground
        z: -1
    }
    Component {
        id: horizontalLayoutComponent

        RowLayout {
            id: horizontalLayout

            property alias items: root.items

            children: items
            spacing: Theme.defaultPadding
        }
    }
    Component {
        id: verticalLayoutComponent

        ColumnLayout {
            id: verticalLayout

            property alias items: root.items

            children: items
            spacing: Theme.defaultPadding
        }
    }
    Item {
        children: [orientation === Qt.Horizontal ? horizontalLayout : (orientation === Qt.Vertical) ? verticalLayout : null]
    }
}
