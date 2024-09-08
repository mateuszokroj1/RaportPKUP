import QtQuick 6.2
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Shapes

import logic

Item {
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            implicitHeight: toolBar_layout.implicitHeight
            implicitWidth: toolBar_layout.implicitWidth

            Rectangle {
                anchors.fill: parent
                color: Theme.menuBackground
            }
            ColumnLayout {
                id: toolBar_layout

                anchors.fill: parent
                anchors.margins: Theme.defaultPadding

                UIText {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.margins: Theme.defaultPadding
                    color: Theme.accentTitle
                    font.bold: true
                    font.pointSize: 14
                    text: "Przelicznik pracy twórczej"
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: Theme.defaultPadding

                    Item {
                        Layout.fillWidth: true
                    }
                    FormField {
                        Layout.fillWidth: false
                        color: Theme.windowText
                        label: "Liczba dni roboczych"

                        InputField {
                            id: workingDays

                            Layout.fillHeight: false
                            value: "1"

                            validator: IntValidator {
                                bottom: 1
                                top: Math.round(Math.abs((controller.toDay - controller.fromDay) / (24 * 60 * 60 * 1000)))
                            }
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
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: Theme.defaultPageMargin

            SplitView {
                id: headers

                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                orientation: Qt.Horizontal

                Rectangle {
                    id: header1

                    SplitView.minimumWidth: 50
                    color: "#eeeeee"

                    UIText {
                        id: header1_text

                        anchors.centerIn: parent
                        text: "Lp."
                    }
                }
                Rectangle {
                    id: header2

                    SplitView.minimumWidth: 180
                    color: "#eeeeee"

                    UIText {
                        id: header2_text

                        anchors.centerIn: parent
                        text: "Nazwa repozytorium"
                    }
                }
                Rectangle {
                    id: header3

                    SplitView.minimumWidth: 140
                    color: "#eeeeee"

                    UIText {
                        id: header3_text

                        anchors.centerIn: parent
                        text: "Data wykonania"
                    }
                }
                Rectangle {
                    id: header4

                    SplitView.fillWidth: true
                    color: "#eeeeee"

                    UIText {
                        id: header4_text

                        anchors.centerIn: parent
                        text: "Tytuł"
                    }
                }
                Rectangle {
                    id: header5

                    SplitView.minimumWidth: 120
                    color: "#eeeeee"

                    UIText {
                        id: header5_text

                        anchors.centerIn: parent
                        text: "Liczba godzin"
                    }
                }
                Rectangle {
                    id: header6

                    SplitView.minimumWidth: 50
                    color: "#eeeeee"

                    UIText {
                        id: header6_text

                        anchors.centerIn: parent
                        text: "Usuń"
                    }
                }
            }
            ScrollView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: Theme.defaultPadding

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    Repeater {
                        model: controller.commits

                        RowLayout {
                            property color background: "#f0f0f0"
                            required property int index
                            required property QtObject modelData
                            property color textColor: Theme.windowText

                            spacing: 10

                            Rectangle {
                                Layout.preferredWidth: header1.width
                                color: background

                                UIText {
                                    anchors.centerIn: parent
                                    anchors.margins: Theme.defaultPadding
                                    color: textColor
                                    text: index + 1
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: header2.width
                                color: background

                                UIText {
                                    anchors.centerIn: parent
                                    anchors.margins: Theme.defaultPadding
                                    color: textColor
                                    elide: Text.ElideRight
                                    text: modelData.repositoryName
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: header3.width
                                color: background

                                UIText {
                                    anchors.centerIn: parent
                                    anchors.margins: Theme.defaultPadding
                                    color: textColor
                                    text: modelData.time.getDate() + "-" + modelData.time.getMonth() + "-" + modelData.time.getFullYear()
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: header4.width
                                color: background

                                UIText {
                                    anchors.left: parent.left
                                    anchors.margins: Theme.defaultPadding
                                    anchors.verticalCenter: parent
                                    color: textColor
                                    elide: Text.ElideRight
                                    text: modelData.message
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: header5.width
                                color: background

                                InputField {
                                    anchors.centerIn: parent
                                    anchors.margins: Theme.defaultPadding
                                    value: modelData.duration

                                    validator: IntValidator {
                                        bottom: 0
                                        top: 1000000
                                    }

                                    onValueChanged: {
                                        modelData.duration = this.value;
                                    }
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: header6.width
                                color: background

                                UIButton {
                                    anchors.centerIn: parent
                                    anchors.margins: Theme.defaultPadding
                                    text: "Usuń"

                                    onClicked: {
                                        controller.removeCommit(modelData);
                                    }
                                }
                            }
                        }
                    }
                }
                UIText {
                    anchors.centerIn: parent
                    text: "Lista jest pusta"
                    visible: controller.commits.length === 0
                }
            }
        }
    }
}

/*ColumnLayout {
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
}*/
