import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml

import QtQuick 2.0

Item {
    id: root

    property date selectedDate: new Date()

    signal clicked(date date)

    function set(date) {
        selectedDate = new Date(date);
        view.positionViewAtIndex((selectedDate.getFullYear()) * 12 + selectedDate.getMonth(), ListView.Center); // index from month year
    }

    height: 300
    implicitHeight: view.implicitHeight
    implicitWidth: view.implicitWidth
    width: 300

    ListView {
        id: view

        clip: true
        height: 300 // default size
        model: 3000 * 12 // index == months since January of the year 0

        orientation: Qt.Horizontal
        snapMode: ListView.SnapOneItem
        width: 300

        delegate: Item {
            property int firstDay: new Date(year, month, 1).getDay() // 0 Sunday to 6 Saturday

            property int month: index % 12 // 0 January
            property int year: Math.floor(index / 12)

            height: view.height
            width: view.width

            Column {
                Item {
                    height: view.height - grid.height
                    // month year header
                    width: view.width

                    Text {
                        // month year
                        anchors.centerIn: parent
                        text: ['styczeń', 'luty', 'marzec', 'kwiecień', 'maj', 'czerwiec', 'lipiec', 'sierpień', 'wrzesień', 'październik', 'listopad', 'grudzień'][month] + ' ' + year

                        font {
                            pixelSize: 0.5 * grid.cellHeight
                        }
                    }
                }
                Grid { // 1 month calender
                    id: grid

                    property real cellHeight: height / rows // width and height of each cell in the grid.

                    property real cellWidth: width / columns

                    columns: 7 // days
                    height: 0.875 * view.height
                    rows: 7
                    width: view.width

                    Repeater {
                        model: grid.columns * grid.rows // 49 cells per month

                        delegate: Rectangle {
                            property int date: day - firstDay + 1 // 1-31

                            // index is 0 to 48
                            property int day: index - 7 // 0 = top left below Sunday (-7 to 41)

                            border.color: new Date(year, month, date).toDateString() == selectedDate.toDateString() && text.text && day >= 0 ? Theme.activatedElementBackground : 'transparent' // selected
                            border.width: 0.3 * radius
                            height: grid.cellHeight
                            opacity: !mouseArea.pressed ? 1 : 0.3  //  pressed state

                            radius: 0.02 * view.height
                            width: grid.cellWidth

                            Text {
                                id: text

                                anchors.centerIn: parent
                                font.bold: new Date(year, month, date).toDateString() == new Date().toDateString() // today
                                font.pixelSize: 0.5 * parent.height
                                text: {
                                    if (day < 0)
                                        ['Nd', 'Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sob'][index];
                                    else
                                    // Su-Sa
                                    if (new Date(year, month, date).getMonth() == month)
                                        date;
                                    else
                                        // 1-31
                                        '';
                                }
                            }
                            MouseArea {
                                id: mouseArea

                                anchors.fill: parent
                                enabled: text.text && day >= 0

                                onClicked: {
                                    selectedDate = new Date(year, month, date);
                                    root.clicked(selectedDate);
                                }
                            }
                        }
                    }
                }
            }
        }

        Keys.onLeftPressed: {
            set(selectedDate.setMonth(selectedDate.getMonth() - 1));
            root.clicked(selectedDate);
        }
        Keys.onRightPressed: {
            set(selectedDate.setMonth(selectedDate.getMonth() + 1));
            root.clicked(selectedDate);
        }

        Button {
            anchors.left: parent.left
            anchors.top: parent.top
            flat: true
            padding: 5
            text: '←'

            onClicked: {
                set(selectedDate.setMonth(selectedDate.getMonth() - 1));
                root.clicked(selectedDate);
            }
        }
        Button {
            anchors.right: parent.right
            anchors.top: parent.top
            flat: true
            padding: 5
            text: '→'

            onClicked: {
                set(selectedDate.setMonth(selectedDate.getMonth() + 1));
                root.clicked(selectedDate);
            }
        }
    }
}
