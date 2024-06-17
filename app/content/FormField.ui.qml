import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

ColumnLayout {
    required property FormItemBase item
    property string label: "Field"
    property string messageOnInvalidate

    alignment: Qt.AlignTop | Qt.AlignLeft
    margins: Theme.defaultPadding
    spacing: Theme.defaultPadding

    UIText {
        color: Theme.activatedElementBackground
        text: label
    }
    Loader {
        Layout.fillWidth: true
        source: item
    }
    UIText {
        color: Theme.invalidAccentBorderColor
        font.pointSize: 11
        text: messageOnInvalidate
    }
}
