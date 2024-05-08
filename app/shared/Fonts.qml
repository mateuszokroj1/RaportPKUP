pragma Singleton

import QtQuick

Item {
    readonly property font font_FiraSans: exampleText.font

    FontLoader {
        id: fontLoader

        source: "fonts/FiraSans-Regular.ttf"
    }
    Text {
        id: exampleText

        font.family: fontLoader.font.family
        font.pointSize: Theme.fontSize
        font.styleName: fontLoader.font.styleName
        font.weight: fontLoader.font.weight
        text: "Przykład"
    }
}
