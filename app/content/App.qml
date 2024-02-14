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

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        rowSpacing: 0
        columnSpacing: 0
        rows: 1
        columns: 2

        ColumnLayout {
            id: columnLayout
            Layout.fillHeight: true
            anchors.top: parent.top
            Layout.maximumWidth: 250
            Layout.preferredWidth: 250

            Button {
                id: menuButton1
                text: "Wprowadzanie danych"
                Layout.fillWidth: true

                onClicked: {
                    content.index = 0
                }

                readonly property int targetIndex: 0

                contentItem: Text {
                    text: menuButton1.text
                    font: menuButton1.font
                    color: content.index === menuButton1.targetIndex ? "white" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    z: 2
                }

                background: Rectangle {
                    color: content.index === menuButton1.targetIndex ? "#0080f5" : "#eee"
                    anchors.fill: menuButton1
                    z: 1
                    radius: 30
                }
            }

            Button {
                id: menuButton2
                text: "Edycja godzin"
                Layout.fillWidth: true

                onClicked: {
                    content.index = 1
                }

                readonly property int targetIndex: 1

                contentItem: Text {
                    text: menuButton2.text
                    font: menuButton2.font
                    color: content.index === menuButton2.targetIndex ? "white" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight

                }

                background: Rectangle {
                    color: content.index === menuButton2.targetIndex ? "#0080f5" : "#eee"
                    anchors.fill: menuButton2

                    radius: 30
                }
            }
        }

        StackLayout {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            index: 0

            Rectangle {
                color: "white"
                anchors.fill: parent
            }
        }
    }
}

