import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    Layout.alignment: Qt.AlignTop
    anchors.fill: parent
    spacing: 0

    Item {
        id: toolbar

        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
        implicitHeight: toolbarLayout.implicitHeight
        implicitWidth: toolbarLayout.implicitWidth

        Rectangle {
            anchors.fill: parent
            color: Theme.menuBackground
            z: -1
        }
        RowLayout {
            id: toolbarLayout

            anchors.fill: parent
            spacing: Theme.defaultPadding

            UIText {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.leftMargin: Theme.defaultPadding
                text: "Wstępnie zdefiniowane ustawienia:"
            }
            ComboBox {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Theme.defaultPadding
                editable: true
            }
            UIButton {
                id: savePresetCmd

                padding: Theme.defaultPadding
                text: "Zapisz jako"
            }
            UIButton {
                id: renamePresetCmd

                text: "Zmień nazwę"
            }
            UIButton {
                id: deletePresetCmd

                Layout.rightMargin: Theme.defaultPadding
                text: "Usuń"
            }
        }
    }
    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: Theme.defaultPadding

        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            spacing: Theme.defaultPadding

            InputField {
                Layout.fillWidth: true
            }
            UIButton {
                text: "Wybierz folder"
            }
            UIButton {
                text: "Dodaj"
            }
        }
        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: Theme.defaultPadding
            Layout.minimumHeight: 300

            background: Rectangle {
                border.color: "grey"
                border.width: 1
                color: "transparent"
                radius: 5
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.defaultPadding

                Item {
                    Rectangle {
                        anchors.fill: parent
                        z: -1
                    }
                    ColumnLayout {
                        Layout.alignment: Qt.AlignTop
                        anchors.fill: parent

                        RowLayout {
                            Layout.alignment: Qt.AlignTop
                            Layout.fillWidth: true
                            Layout.margins: 3

                            Text {
                                id: name

                                Layout.fillWidth: true
                                text: qsTr("text")
                            }
                            Button {
                                text: "a"
                            }
                        }
                    }
                }
            }
        }
    }
}
