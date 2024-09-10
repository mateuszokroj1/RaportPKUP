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
                        id: form1

                        property int val: 0

                        Layout.fillWidth: false
                        color: Theme.windowText
                        label: "Liczba dni roboczych"

                        InputField {
                            property int defaultValue: Math.round(Math.abs((controller.toDay - controller.fromDay) / (24 * 60 * 60 * 1000)))

                            Layout.fillHeight: false
                            value: defaultValue

                            validator: IntValidator {
                                bottom: 1
                                top: 1000000
                            }

                            Component.onCompleted: {
                                form1.val = defaultValue;
                            }
                            Keys.onDownPressed: {
                                if (value > 0)
                                    this.value--;
                            }
                            Keys.onUpPressed: {
                                this.value++;
                            }
                            onValueChanged: {
                                form1.val = this.value;
                            }
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    UIText {
                        Layout.alignment: Qt.AlignVCenter
                        text: "Suma godzin pracy twórczej: " + controller.sumOfHours
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    UIText {
                        Layout.alignment: Qt.AlignVCenter
                        text: "Procentowy udział: " + Math.trunc(controller.sumOfHours / (form1.val * 24)) + "%"
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

                    SplitView.minimumWidth: 100
                    color: "#eeeeee"

                    UIText {
                        id: header5_text

                        anchors.centerIn: parent
                        text: "Liczba godzin"
                    }
                }
                Rectangle {
                    id: header6

                    SplitView.minimumWidth: 120
                    color: "#eeeeee"

                    UIText {
                        id: header6_text

                        anchors.centerIn: parent
                        text: "Usuń"
                    }
                }
            }
            ListView {
                id: commits

                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                model: controller.commits
                snapMode: ListView.SnapOneItem
                spacing: 6

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    snapMode: ScrollBar.SnapAlways
                }
                delegate: Item {
                    id: row

                    property color background: "#efefef"
                    required property int index
                    required property CommitItem modelData
                    property color textColor: Theme.windowText

                    implicitHeight: row_layout.implicitHeight
                    implicitWidth: row_layout.implicitWidth

                    RowLayout {
                        id: row_layout

                        spacing: 6

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header1.width
                            color: background
                            implicitHeight: text1.implicitHeight + Theme.defaultPadding * 2

                            UIText {
                                id: text1

                                anchors.centerIn: parent
                                color: textColor
                                text: index + 1
                            }
                        }
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header2.width
                            color: background
                            implicitHeight: text2.implicitHeight + Theme.defaultPadding * 2

                            UIText {
                                id: text2

                                anchors.centerIn: parent
                                anchors.margins: Theme.defaultPadding
                                color: textColor
                                elide: Text.ElideRight
                                text: modelData.repositoryName
                            }
                        }
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header3.width
                            color: background
                            implicitHeight: text3.implicitHeight + Theme.defaultPadding * 2

                            UIText {
                                id: text3

                                anchors.centerIn: parent
                                anchors.margins: Theme.defaultPadding
                                color: textColor
                                text: modelData.time.getDate() + "-" + modelData.time.getMonth() + "-" + modelData.time.getFullYear()
                            }
                        }
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header4.width
                            color: background
                            implicitHeight: text4_wrapper.implicitHeight + Theme.defaultPadding * 2

                            Item {
                                id: text4_wrapper

                                anchors.centerIn: parent
                                implicitHeight: text4.implicitHeight
                                implicitWidth: text4.implicitWidth

                                UIText {
                                    id: text4

                                    anchors.fill: parent
                                    color: textColor
                                    elide: Text.ElideRight
                                    text: modelData.message
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header5.width
                            color: background
                            implicitHeight: textField1.implicitHeight + Theme.defaultPadding * 2

                            InputField {
                                id: textField1

                                anchors.centerIn: parent
                                value: modelData.duration
                                width: 80

                                validator: IntValidator {
                                    bottom: 0
                                    top: 1000000
                                }

                                Keys.onDownPressed: {
                                    if (modelData.duration > 0)
                                        modelData.duration--;
                                }
                                Keys.onUpPressed: {
                                    if (modelData.duration < 1000000)
                                        modelData.duration++;
                                }
                                onValueChanged: {
                                    modelData.duration = this.value;
                                }
                            }
                        }
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: header6.width
                            color: background
                            implicitHeight: button1.implicitHeight + Theme.defaultPadding * 2

                            UIButton {
                                id: button1

                                anchors.centerIn: parent
                                padding: 5
                                text: "Usuń"

                                onClicked: {
                                    controller.removeCommit(index);
                                }
                            }
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
