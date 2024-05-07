// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Shapes
import Main
import app

Window {
    id: mainWindow

    color: Theme.menuBackground
    height: 480
    title: "Generator raportów PKUP"
    visible: true
    width: 640

    MainWindowView {
        anchors.fill: parent
        items: controller.items
    }
}
