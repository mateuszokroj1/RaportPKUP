import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    RowLayout {
        InputField {
            placeholderText: "Miejscowość"
            value: controller.city
        }
    }
    Button {
        text: "Zapisz"

        onClicked: controller.saveRaportToFile("s")
    }
}
