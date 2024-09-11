import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import QtCore

ColumnLayout {
    anchors.fill: parent

    Item {
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true
        implicitHeight: toolBar_layout.implicitHeight
        implicitWidth: toolBar_layout.implicitWidth

        Rectangle {
            anchors.fill: parent
            color: Theme.menuBackground
        }
        RowLayout {
            id: toolBar_layout

            anchors.left: parent.left
            anchors.right: parent.right

            Item {
                Layout.fillWidth: true
            }
            ColumnLayout {
                Layout.fillHeight: true

                InputField {
                    placeholderText: "Miejscowość"
                    value: controller.city

                    onValueChanged: {
                        controller.city = this.value;
                    }
                }
            }
            Item {
                Layout.fillWidth: true
            }
            ColumnLayout {
                UIText {
                    Layout.margins: Theme.defaultPadding
                    color: Theme.accentTitle
                    text: "Data przygotowania raportu"
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
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }
    TextEdit {
        id: preview

        Layout.fillHeight: true
        Layout.fillWidth: true
        readOnly: true
        text: controller.previewDocument
        textFormat: TextEdit.RichText
        wrapMode: TextEdit.Wrap
    }
    Item {
        Layout.alignment: Qt.AlignBottom
        Layout.fillWidth: true

        UIButton {
            anchors.centerIn: parent
            anchors.margins: Theme.defaultPadding
            highlighted: true
            text: "Zapisz"

            onClicked: saveDialog.open()
        }
    }
    FileDialog {
        id: saveDialog

        acceptLabel: "Zapisz"
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        defaultSuffix: ".pdf"
        fileMode: FileDialog.SaveFile
        modality: "WindowModal"
        nameFilters: ["Adobe PDF (*.pdf)"]
        rejectLabel: "Anuluj"

        onAccepted: {
            controller.saveRaportToFile(saveDialog.currentFile);
        }
    }
    Shortcut {
        context: Qt.WindowShortcut
        sequence: StandardKey.Save

        onActivated: saveDialog.open()
    }
}
