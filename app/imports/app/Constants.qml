pragma Singleton
import QtQuick 6.2

QtObject {
    readonly property color backgroundColor: "#EAEAEA"

    /* Edit this comment to add your custom font */
    readonly property font font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize
    })
    readonly property int height: 1080
    readonly property font largeFont: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.6
    })
    property string relativeFontDirectory: "fonts"
    readonly property int width: 1920
}
