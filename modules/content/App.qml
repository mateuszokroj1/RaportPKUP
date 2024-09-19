// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Window {
    id: mainWindow

    color: Theme.windowBackground
    minimumHeight: content.minimumHeight
    minimumWidth: content.minimumWidth
    title: "Generator raportów PKUP"
    visible: true

    StepSelector {
        id: content

        anchors.fill: parent

        items: [
            MainViewItem {
                name: "Przeszukiwanie repozytoriów"

                DataInputStepView {
                    readonly property int minimumHeight: 1000
                    readonly property int minimumWidth: 1090
                }
            },
            MainViewItem {
                name: "Edycja listy"

                DataFilteringStepView {
                    readonly property int minimumHeight: 1000
                    readonly property int minimumWidth: 1600
                }
            },
            MainViewItem {
                name: "Generowanie raportu"

                ReportingStepView {
                    readonly property int minimumHeight: 1000
                    readonly property int minimumWidth: 1090
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
                id: locked1_text

                Layout.alignment: Qt.AlignHCenter
                color: "white"
                text: "Skanowanie repozytoriów..."
            }
            UIText {
                id: locked2_text

                Layout.alignment: Qt.AlignHCenter
                color: "white"
                text: "Generowanie pliku PDF..."
            }
        }
    }
    MessageDialog {
        id: warningDialog

        buttons: MessageDialog.Ok
        modality: Qt.ApplicationModal
    }
    Connections {
        target: controller

        onLockScreen1: {
            locked.visible = true;
            locked1_text.visible = true;
            locked2_text.visible = false;
        }
        onLockScreen2: {
            locked.visible = true;
            locked1_text.visible = false;
            locked2_text.visible = true;
        }
        onShowFilteringView: {
            content.selectView(1);
        }
        onShowWarning: text => {
            warningDialog.text = text;
            warningDialog.open();
        }
        onUnlockScreen: {
            locked.visible = false;
        }
    }
}
