// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls

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
                name: "Wprowadzanie danych"

                DataInputStepView {
                }
            },
            MainViewItem {
                name: "Uzupełnianie godzin"

                //isEnabled: controller.commits.length

                DataFilteringStepView {
                }
            },
            MainViewItem {
                name: "Generowanie raportu"

                //isEnabled: controller.commits.length

                ReportingStepView {
                }
            }
        ]
    }
}
