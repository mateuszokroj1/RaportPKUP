pragma Singleton

import QtQuick 6.2

QtObject {
    readonly property color activatedElementBackground: "#1589ff"
    readonly property color activatedElementText: "white"
    readonly property color controlBackground: "#ededed"
    readonly property color defaultBorderColor: "#2cafff"
    readonly property font defaultFont: Fonts.font_FiraSans
    readonly property double defaultPadding: 15
    readonly property color disabledBackground: "#c6c6c6"
    readonly property color disabledText: "#8a8a8a"
    readonly property double fontSize: 13
    readonly property color highlightedElementBackground: "white"
    readonly property color invalidAccentBorderColor: "#f00000"
    readonly property color menuBackground: "#eeeeee"
    readonly property double radius: 4
    readonly property color validAccentBorderColor: "#00e900"
    readonly property color windowBackground: "#fafafa"
    readonly property color windowText: "#101010"
}
