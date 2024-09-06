import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    RowLayout {
        RowLayout {
            InputField {
                placeholderText: "Miejscowość"
                value: controller.city

                onValueChanged: {
                    controller.city = this.value;
                }
            }
        }
        UIButton {
            text: "Zapisz..."

            onClicked: controller.saveRaportToFile("s")
        }
    }
    TextEdit {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
