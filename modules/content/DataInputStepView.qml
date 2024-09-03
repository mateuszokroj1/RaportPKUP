import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import logic

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
                id: presetSelector

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: Theme.defaultPadding
                editable: true
                model: controller.presets
                textRole: "name"

                onCurrentIndexChanged: controller.recallPreset(presetSelector.currentIndex)
                onEditTextChanged: controller.presetSelectorText = presetSelector.editText
            }
            UIButton {
                id: savePresetCmd

                enabled: controller.canSavePreset
                text: "Zapisz"

                onClicked: controller.savePreset(presetSelector.editText)
            }
            UIButton {
                id: deletePresetCmd

                Layout.rightMargin: Theme.defaultPadding
                enabled: presetSelector.currentIndex >= 0
                text: "Usuń"

                onClicked: controller.deletePreset(presetSelector.currentIndex)
            }
        }
    }
    ColumnLayout {
        Layout.bottomMargin: 15
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.leftMargin: 15
        Layout.rightMargin: 50
        Layout.topMargin: 15

        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            spacing: Theme.defaultPadding

            InputField {
                id: repositoryPath

                Layout.fillWidth: true
                value: controller.repositoryPath

                onValueChanged: {
                    controller.repositoryPath = repositoryPath.value;
                }
            }
            UIButton {
                text: "Wybierz folder"

                onClicked: directoryDialog.open()
            }
            UIButton {
                enabled: repositoryPath.value.length > 0
                text: "Dodaj"

                onClicked: controller.addRepository()
            }
        }
        FolderDialog {
            id: directoryDialog

            acceptLabel: "Wybierz"
            modality: Qt.WindowModal
            rejectLabel: "Anuluj"
            title: "Wybierz folder repozytorium"

            onAccepted: controller.setRepositoryPath(directoryDialog.selectedFolder)
        }
        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: Theme.defaultPadding
            Layout.minimumHeight: 1

            ListView {
                model: controller.repositories
                spacing: 15

                delegate: Item {
                    id: repoList_item

                    required property int index
                    required property string path

                    anchors.left: parent.left
                    anchors.margins: 15
                    anchors.right: parent.right
                    implicitHeight: repoList_itemLayout.implicitHeight
                    implicitWidth: repoList_itemLayout.implicitWidth

                    Rectangle {
                        anchors.fill: parent
                        color: "#eeeeee"
                        radius: 4
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

                            onClicked: controller.removeRepository(index)
                        }
                    }
                }
            }
        }
        GridLayout {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            columns: 2
            rows: 1

            FormField {
                Layout.column: 0
                Layout.fillHeight: false
                Layout.fillWidth: true
                Layout.row: 0
                label: "Imię i nazwisko"

                InputField {
                    value: controller.authorName
                }
            }
            FormField {
                Layout.column: 1
                Layout.fillHeight: false
                Layout.fillWidth: true
                Layout.row: 0
                label: "Adres e-mail"

                InputField {
                    value: controller.authorEmail
                }
            }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: Theme.defaultPadding

            RowLayout {
                Item {
                    Layout.fillWidth: true
                }
                UIText {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.bottomMargin: Theme.defaultPadding
                    color: Theme.activatedElementBackground
                    text: "Zakres dat przeszukiwania"
                }
                Item {
                    Layout.fillWidth: true
                }
            }
            RowLayout {
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }
                DatePicker {
                    id: fromDate

                    Component.onCompleted: {
                        fromDate.set(controller.fromDay);
                    }
                    onClicked: {
                        controller.fromDay = fromDate.selectedDate;
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                DatePicker {
                    id: toDate

                    Component.onCompleted: {
                        toDate.set(controller.toDay);
                    }
                    onClicked: {
                        controller.toDay = toDate.selectedDate;
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }
            UIButton {
                Layout.alignment: Qt.AlignCenter
                Layout.margins: Theme.defaultPadding
                enabled: controller.canStartSearch
                highlighted: true
                text: "Przeszukaj repozytoria"

                onClicked: controller.searchForCommits()
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }
    Connections {
        function onFromDayChanged() {
            fromDate.set(controller.fromDay);
        }
        function onToDayChanged() {
            toDate.set(controller.toDay);
        }

        target: controller
    }
}
