import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    Item {
        id: toolbar

        implicitHeight: toolbarLayout.implicitHeight
        implicitWidth: toolbarLayout.implicitWidth

        Rectangle {
            anchors.fill: parent
            color: Theme.menuBackground
            z: -1
        }
        RowLayout {
            id: toolbarLayout

            spacing: Theme.defaultPadding

            UIText {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
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

                onClicked: {
                    savedPresetCmd.text = "s";
                }
            }
            UIButton {
                id: renamePresetCmd

                text: "Zmień nazwę"
            }
            UIButton {
                id: deletePresetCmd

                text: "Usuń"
            }
        }
    }
    ColumnLayout {
        RowLayout {
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
        ListView {
            Layout.fillWidth: true
            Layout.minimumHeight: 100
        }
    }
}
