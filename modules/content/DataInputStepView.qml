import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

ColumnLayout {
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
                enabled: false
            }
            UIButton {
                id: savePresetCmd

                enabled: false
                text: "Zapisz jako"
            }
            UIButton {
                id: renamePresetCmd

                enabled: false
                text: "Zmień nazwę"
            }
            UIButton {
                id: deletePresetCmd

                Layout.rightMargin: Theme.defaultPadding
                enabled: false
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
                value: parent.controller.repositoryPath
            }
            UIButton {
                text: "Wybierz folder"

                onClicked: directoryDialog.open()
            }
            UIButton {
                text: "Dodaj"
            }
        }
        FolderDialog {
            id: directoryDialog

            acceptLabel: "Wybierz"
            modality: Qt.WindowModal
            rejectLabel: "Anuluj"
            title: "Wybierz folder repozytorium"

            onAccepted: controller.setRepositoryPath(directoryDialog.folder)
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: Theme.defaultPadding
            model: controller.repositories

            delegate: Item {
                id: repoList_item

                required property string path

                anchors.left: parent.left
                anchors.margins: 15
                anchors.right: parent.right
                implicitHeight: repoList_itemLayout.implicitHeight
                implicitWidth: repoList_itemLayout.implicitWidth

                Rectangle {
                    anchors.fill: parent
                    color: "#eeeeee"
                }
                RowLayout {
                    id: repoList_itemLayout

                    anchors.fill: parent

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.margins: Theme.defaultPadding

                        UIText {
                            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                            elide: Text.ElideMiddle
                            text: path
                            wrapMode: Text.NoWrap
                        }
                    }
                    UIButton {
                        Layout.alignment: Qt.AlignRight
                        Layout.bottomMargin: Theme.defaultPadding
                        Layout.rightMargin: Theme.defaultPadding
                        Layout.topMargin: Theme.defaultPadding
                        text: "Usuń"

                        onClicked: controller.removeRepository(model)
                    }
                }
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            FormField {
                label: "Imię i nazwisko"

                InputField {
                    value: controller.authorName
                }
            }
            FormField {
                label: "Adres e-mail"

                InputField {
                    value: controller.authorEmail
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            RowLayout {
                InputField {
                    value: controller.fromDay
                }
                InputField {
                    value: controller.toDay
                }
            }
            RowLayout {
                InputField {
                    placeholderText: "Miejscowość"
                    value: controller.city
                }
                CheckBox {
                    checked: controller.canFetchBefore
                    text: "Pobierz informacje o zmianach przed przeszukaniem"
                }
            }
        }
        RowLayout {
            UIButton {
                Layout.alignment: Qt.AlignCenter
                highlighted: true
                text: "Przeszukaj repozytoria"
            }
        }
    }
}
