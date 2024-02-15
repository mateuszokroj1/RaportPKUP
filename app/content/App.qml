// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Shapes

Window {
    id: mainWindow
    width: 640
    height: 480
    color: "#fafafa"
    visible: true
    title: "Generator raportów PKUP"

    MainWindowView {
        anchors.fill: parent

        items: [
            MainWindowItem {
                header: "Wprowadzanie danych"
                content: ColumnLayout {
                    anchors.fill: parent
                    Text {text: "Text"}
                }
            }

        ]
    }
}

