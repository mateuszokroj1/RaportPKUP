import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

import logic

ColumnLayout {
    Layout.minimumWidth: implicitWidth
    anchors.fill: parent

    SplitView {
        Layout.fillWidth: true
        Layout.margins: 15
        Layout.preferredHeight: 50
        implicitWidth: header1.implicitWidth + header2.implicitWidth + header3.implicitWidth + header4.implicitWidth
        orientation: Qt.Horizontal

        Rectangle {
            id: header1

            SplitView.fillHeight: true
            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 160
            color: "#eeeeee"
            implicitWidth: headerText1.implicitWidth

            UIText {
                id: headerText1

                anchors.centerIn: parent
                text: "Nazwa repozytorium"
            }
        }
        Rectangle {
            id: header2

            SplitView.fillHeight: true
            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 310
            color: "#eeeeee"
            implicitWidth: headerText2.implicitWidth

            UIText {
                id: headerText2

                anchors.centerIn: parent
                text: "Tytuł"
            }
        }
        Rectangle {
            id: header3

            SplitView.fillHeight: true
            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 160
            color: "#eeeeee"
            implicitWidth: headerText3.implicitWidth

            UIText {
                id: headerText3

                anchors.centerIn: parent
                text: "Data wykonania"
            }
        }
        Rectangle {
            id: header4

            SplitView.fillHeight: true
            SplitView.minimumWidth: 142
            SplitView.preferredWidth: 142
            color: "#eeeeee"
            implicitWidth: headerText4.implicitWidth

            UIText {
                id: headerText4

                anchors.centerIn: parent
                text: "Liczba godzin"
            }
        }
    }
    ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 15
        model: controller.commits

        delegate: Item {
            required property CommitItem commit

            anchors.left: parent.left
            anchors.margins: 15
            anchors.right: parent.right

            ColumnLayout {
                Rectangle {
                }
                Rectangle {
                }
                Rectangle {
                }
                Rectangle {
                    Layout.margins: 15
                    Layout.preferredWidth: header4.width

                    SpinBox {
                        anchors.centerIn: parent
                        anchors.margins: 15
                        from: 0
                        stepSize: 1
                        to: 100000
                        value: commit.duration
                    }
                }
            }
        }
    }
}
