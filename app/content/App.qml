// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.2

Window {
    id: mainWindow
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"
    visible: true
    title: "Generator raportów PKUP"

    property int borderSize: 3
    property int headerHeight: 40

    MouseArea {
        id: moveArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        height: 20
        
        property int startX;
        property int startY;

        onPressed: {
            startX = mouseX
            startY = mouseY
        }
        onPositionChanged: {
            mainWindow.setX(mainWindow.x + (mouseX - startX))
            mainWindow.setY(mainWindow.y + (mouseY - startY))
        }

        Rectangle
        {
            color: "blue"
            anchors.fill: parent
        }
    }

    MouseArea {
        id: leftArea
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        cursorShape: Qt.SizeHorCursor
        width: 5

        property int startX;

        onPressed: {
            startX = mouseX
        }
        onMouseXChanged: {
            var dx = mouseX - startX

            mainWindow.setX(mainWindow.x + dx)
            mainWindow.setWidth(Math.max(Math.min(mainWindow.maximumWidth, mainWindow.width - dx), mainWindow.minimumWidth))
        }

        Rectangle {
            anchors.fill: parent
            color: "red"
        }
    }

    MouseArea {
        id: rightArea
        width: 5
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        cursorShape: Qt.SizeHorCursor

        property int startX;

        onPressed: {
            startX = mouseX
        }
        onMouseXChanged: {
            mainWindow.setWidth(Math.max(Math.min(mainWindow.maximumWidth, mainWindow.width + (mouseX - startX)), mainWindow.minimumWidth))
        }

        Rectangle {
            anchors.fill: parent
            color: "red"
        }
    }

    MouseArea {
        id: bottomArea
        y: parent.height - bottomArea.height
        height: 5
        anchors.leftMargin: 5
        anchors.left: parent.left
        anchors.rightMargin: 5
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        cursorShape: Qt.SizeVerCursor

        property int startY;

        onPressed: {
            startY = mouseY
        }
        onMouseYChanged: {
            mainWindow.setHeight(Math.max(Math.min(mainWindow.maximumHeight, mainWindow.height + (mouseY - startY)), mainWindow.minimumHeight))
        }

        Rectangle {
            anchors.fill: parent
            color: "red"
        }
    }

    MouseArea {
        id: leftBottomArea
        y: parent.height - bottomArea.height
        height: 5
        anchors.leftMargin: 5
        anchors.left: parent.left
        anchors.rightMargin: 5
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        cursorShape: Qt.SizeVerCursor

        property int startY;

        onPressed: {
            startY = mouseY
        }
        onMouseYChanged: {
            mainWindow.setHeight(Math.max(Math.min(mainWindow.maximumHeight, mainWindow.height + (mouseY - startY)), mainWindow.minimumHeight))
        }

        Rectangle {
            anchors.fill: parent
            color: "red"
        }
    }
}

