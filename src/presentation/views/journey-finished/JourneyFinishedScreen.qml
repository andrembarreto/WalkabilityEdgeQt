import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import WalkabilityEdgeQt

Page {
    id: root

    signal finished()
    signal discarded()
    signal returned()

    // Id do percurso no cache de finalizados. Sem ele, trata o percurso recem finalizado
    property int journeyID: -1
    readonly property bool isSaved: journeyID >= 0

    background: null

    Component.onCompleted: {
        if(root.isSaved)
            JourneyViewModel.dispatchSaved(root.journeyID);
    }

    Connections {
        target: JourneyViewModel.dispatcher

        function onSuccess() {
            root.showMessage("Percurso salvo", Material.color(Material.LightBlue));
            root.finished();
        }

        function onFail(reason: string) {
            root.showMessage(reason, Material.color(Material.Red));
        }
    }

    contentItem: StackLayout {
        currentIndex: JourneyViewModel.dispatcher.isExecuting ?
            frameSending.index : frameDialog.index

        Item {
            id: frameDialog
            readonly property int index: StackLayout.index

            ColumnLayout {
                anchors.centerIn: parent

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("Percurso concluído")
                    color: "white"
                    font.pointSize: 10
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        id: buttonDiscard
                        visible: !root.isSaved
                        icon.source: "qrc:/resources/icons/delete.svg"
                        onPressAndHold: {
                            JourneyViewModel.discard();
                            root.discarded();
                        }
                    }

                    Button {
                        id: buttonReturn
                        visible: root.isSaved
                        icon.source: "qrc:/resources/icons/chevron-backward.svg"
                        onClicked: {
                            root.returned();
                        }
                    }

                    Button {
                        id: buttonSave
                        icon.source: "qrc:/resources/icons/save.svg"
                        onClicked: {
                            if(root.isSaved)
                                JourneyViewModel.dispatchSaved(root.journeyID);
                            else
                                JourneyViewModel.dispatch();
                        }
                    }
                }
            }
        }

        Item {
            id: frameSending
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

    function showMessage(message: string, backgroundColor: string) {
        let messageItem = messageComponent.createObject(Overlay.overlay, {
            message: message,
            backgroundColor: backgroundColor
        });
        if(messageItem !== null) {
            messageItem.open();
        }
        else {
            console.log("Error showing popup message for ", message);
        }
    }

    Component {
        id: messageComponent

        Popup {
            id: popupMessage
            required property string message
            property color backgroundColor: "transparent"

            x: parent.width/2 - width/2
            y: parent.height - height - 8
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

            Timer {
                interval: 3000
                running: popupMessage.opened
                onTriggered: popupMessage.close()
            }

            onClosed: popupMessage.destroy()
        }
    }
}
