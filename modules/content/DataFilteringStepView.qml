import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls

import logic

ColumnLayout {
    ColumnLayout {
        UIText {
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: Theme.defaultPadding
            color: Theme.activatedElementBackground
            text: "Przelicznik pracy twórczej"
        }
        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }
            FormField {
                label: "Liczba dni roboczych"

                InputField {
                    id: workingDays

                    value: "0"
                }
            }
            Item {
                Layout.fillWidth: true
            }
            UIText {
                Layout.alignment: Qt.AlignVCenter
                text: "Suma godzin pracy twórczej: "
            }
            Item {
                Layout.fillWidth: true
            }
            UIText {
                Layout.alignment: Qt.AlignVCenter
                text: "Procentowy udział: 0%"
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }
    ColumnLayout {
        Layout.margins: 15

        SplitView {
            id: headers

            //Layout.fillWidth: true
            Layout.margins: 15
            Layout.preferredHeight: 50
            //implicitWidth: header1.implicitWidth + header2.implicitWidth + header3.implicitWidth + header4.implicitWidth
            orientation: Qt.Horizontal

            onWidthChanged: width => {
                console.log(width);
            }

            Rectangle {
                id: header1

                SplitView.fillHeight: true
                SplitView.minimumWidth: 160
                //SplitView.preferredWidth: 160
                color: "#eeeeee"

                //implicitWidth: headerText1.implicitWidth

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
                //SplitView.preferredWidth: 310
                color: "#eeeeee"

                //implicitWidth: headerText2.implicitWidth

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
                //SplitView.preferredWidth: 160
                color: "#eeeeee"

                //implicitWidth: headerText3.implicitWidth

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
                //SplitView.preferredWidth: 142
                color: "#eeeeee"

                //implicitWidth: headerText4.implicitWidth

                UIText {
                    id: headerText4

                    anchors.centerIn: parent
                    text: "Liczba godzin"
                }
            }
            Rectangle {
                id: header5

                SplitView.fillHeight: true
                SplitView.minimumWidth: 50
                //SplitView.preferredWidth: 50
                color: "#eeeeee"

                //implicitWidth: headerText5.implicitWidth

                UIText {
                    id: headerText5

                    anchors.centerIn: parent
                    text: "Usuń"
                }
            }
        }
        ListView {
            //Layout.fillHeight: true
            //Layout.fillWidth: true
            Layout.margins: 15
            model: controller.commits

            delegate: Item {
                required property CommitItem commit

                anchors.left: parent.left
                anchors.margins: 15
                anchors.right: parent.right

                ColumnLayout {
                    spacing: 15

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
                    Rectangle {
                        Layout.margins: 15
                        Layout.preferredWidth: header5.width

                        UIButton {
                            anchors.centerIn: parent
                            text: "Usuń"
                        }
                    }
                }
            }
        }
    }
}
