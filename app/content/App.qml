// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Shapes

WindowUIText {
    id: mainWindow

    color: Theme.windowBackground
    height: 600
    minimumHeight: content.implicitHeight
    minimumWidth: content.implicitWidth
    title: "Generator raportów PKUP"
    visible: true
    width: 800

    StepSelectorUIText {
        id: content

        anchors.fill: parent
        items: controller.items
    }
}
