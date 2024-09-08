import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import logic

ColumnLayout {
    property alias color: header.color
    default property alias item: loader.sourceComponent
    property string label: "Field"
    property string messageOnInvalidate: "Invalid value"

    spacing: Theme.defaultPadding

    UIText {
        id: header

        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        Layout.leftMargin: Theme.defaultPadding
        color: Theme.activatedElementBackground
        text: label
    }
    Loader {
        id: loader

        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: Theme.defaultPadding
    }
    UIText {
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        color: Theme.invalidAccentBorderColor
        font.pointSize: 11
        text: item && item.invalidateMessage ? item.invalidateMessage : messageOnInvalidate
        visible: item && item.dataState === InputDataState.Invalid
    }
}
