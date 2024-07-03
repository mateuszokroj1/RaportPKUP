import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    Button {
        text: "Zapisz"

        onClicked: controller.saveRaportToFile("s")
    }
}
