import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import logic

ColumnLayout {
    default required property FormItemBase item
    property string label: "Field"
    property string messageOnInvalidate: "Invalid value"

    spacing: Theme.defaultPadding

    UIText {
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        Layout.leftMargin: Theme.defaultPadding
        color: Theme.activatedElementBackground
        text: label
    }
    Loader {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: Theme.defaultPadding
        sourceComponent: item
    }
    UIText {
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        color: Theme.invalidAccentBorderColor
        font.pointSize: 11
        text: item && item.invalidateMessage ? item.invalidateMessage : messageOnInvalidate
        visible: item && item.dataState === InputDataState.Invalid
    }
}
