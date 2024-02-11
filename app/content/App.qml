// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Window {
    id: mainWindow
    color: "white"
    visible: true
    title: "Generator raportów PKUP"

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        rows: 1
        columns: 2

        ColumnLayout {
            id: columnLayout
            width: 250
            height: 100

            Button {
                id: button
                text: qsTr("Button")
                Layout.fillWidth: true
            }
        }

        Rectangle {
            id: rectangle
            width: 200
            height: 200
            color: "#00abff"
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}

