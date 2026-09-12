import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import WalkabilityEdgeQt

Page {
    id: root
    required property journeyReportViewModel report
    signal finished()

    background: null

    Connections {
        target: JourneyViewModel.dispatcher

        function onSuccess() {
            root.finished();
        }

        function onFail(reason: string) {
            let messageItem = messageComponent.createObject(root, {
                message: reason,
                backgroundColor: Material.color(Material.Red)
            });
            if(messageItem !== null) {
                messageItem.open();
            }
            else {
                console.log("Error showing popup message for ", reason);
            }
        }
    }

    contentItem: StackLayout {
        currentIndex: JourneyViewModel.dispatcher.isExecuting ?
            frameSendingJourney.index : frameShowingJourney.index

        Item {
            id: frameShowingJourney
            readonly property int index: StackLayout.index

            Label {
                anchors {
                    bottom: reportColumn.top
                    bottomMargin: 12
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("Percurso concluído")
                color: "white"
                font.pointSize: 10
                font.bold: true
            }

            ColumnLayout {
                id: reportColumn
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 20
                }
                spacing: 4

                Repeater {
                    model: [
                        { field: qsTr("Início"), value: report.startTime },
                        { field: qsTr("Fim"), value: report.finishTime },
                        { field: qsTr("Duração"), value: report.duration },
                        { field: qsTr("Distância"), value: report.totalDistance },
                    ]
                    delegate: Label {
                        text: modelData.field + ": " + modelData.value
                        color: "white"
                        font.pointSize: 10
                    }
                }
            }

            Button {
                id: buttonSave
                anchors {
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("Salvar")
                font.pointSize: 12
                onClicked: {
                    JourneyViewModel.dispatch();
                }
            }
        }

        Item {
            id: frameSendingJourney
            readonly property int index: StackLayout.index

            ColumnLayout {
                anchors.centerIn: parent

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("Salvando percurso")
                    color: "white"
                    font.pointSize: 10
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }

    Component {
        id: messageComponent

        Popup {
            id: popupMessage
            required property string message
            property color backgroundColor: "transparent"

            x: parent.width/2 - width/2
            y: 8
            padding: 8

            background: Rectangle {
                radius: height / 4
                color: popupMessage.backgroundColor
            }
            contentItem: Label {
                text: popupMessage.message
                color: "white"
                font.pointSize: 8
                wrapMode: Text.WordWrap
            }
        }
    }
}
