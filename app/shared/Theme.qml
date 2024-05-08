pragma Singleton

import QtQuick 6.2

QtObject {
    readonly property color activatedElementBackground: "#1589ff"
    readonly property color activatedElementText: "white"
    readonly property color controlBackground: "#ededed"
    readonly property font defaultFont: Fonts.font_FiraSans
    readonly property double defaultPadding: 25
    readonly property color disabledBackground: "#c6c6c6"
    readonly property color disabledText: "#8a8a8a"
    readonly property double fontSize: 14
    readonly property color highlightedElementBackground: "white"
    readonly property color invalidAccentBorderColor: "#f00000"
    readonly property color menuBackground: "#e5e5e5"
    readonly property double radius: 3
    readonly property color validAccentBorderColor: "#00e900"
    readonly property color windowBackground: "#fafafa"
    readonly property color windowText: "#101010"
}
