// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Window {
    id: mainWindow

    color: Theme.windowBackground
    minimumHeight: content.implicitHeight
    minimumWidth: content.implicitWidth
    title: "Generator raportów PKUP"
    visible: true

    StepSelector {
        id: content

        anchors.fill: parent

        items: [
            MainViewItem {
                name: "Przeszukiwanie repozytoriów"

                DataInputStepView {
                }
            },
            MainViewItem {
                name: "Edycja listy"

                DataFilteringStepView {
                }
            },
            MainViewItem {
                name: "Generowanie raportu"

                ReportingStepView {
                }
            }
        ]
    }
    Rectangle {
        id: locked

        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.8)
        visible: false

        ColumnLayout {
            anchors.centerIn: parent

            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
            }
            UIText {
                Layout.alignment: Qt.AlignHCenter
                color: "white"
                text: "Skanowanie repozytoriów..."
            }
        }
    }
    Connections {
        target: controller

        onLockScreen: {
            locked.visible = true;
        }
        onShowFilteringView: {
            content.selectView(1);
        }
        onUnlockScreen: {
            locked.visible = false;
        }
    }
}
