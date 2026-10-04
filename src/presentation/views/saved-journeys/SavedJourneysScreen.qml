import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    signal evalRequested(journeyID: int)
    signal deleteRequested(journeyID: int)
    signal finished()

    readonly property int highlightedJourneyID: 1

    background: null

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent

            Button {
                id: buttonFinish
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                icon.source: "qrc:/resources/icons/home.svg"
                icon.color: pressed ? "lightblue" : "white"
                flat: true
                onClicked: {
                    root.finished();
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true

                Button {
                    id: buttonPrevious
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "qrc:/resources/icons/chevron-backward.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    icon.width: 20
                    flat: true
                }

                Label {
                    Layout.fillWidth: true
                    text: qsTr("Sábado, 03/10/26<br>Início às 21:30:55<br>15 minutos")
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    color: "white"
                }

                Button {
                    id: buttonNext
                    Layout.alignment: Qt.AlignRight
                    icon.source: "qrc:/resources/icons/chevron-forward.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    icon.width: 20
                    flat: true
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                Button {
                    id: buttonDelete
                    icon.source: "qrc:/resources/icons/delete.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    flat: true
                    onPressAndHold: {
                        deleteDialog.open();
                    }
                }

                Button {
                    id: buttonEval
                    icon.source: "qrc:/resources/icons/reviews.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    flat: true
                    onClicked: {
                        root.evalRequested(root.highlightedJourneyID);
                    }
                }
            }
        }
    }

    DeleteJourneyDialog {
        id: deleteDialog
        anchors.centerIn: parent
        height: parent.height
        width: parent.width

        onAccepted: {
            root.deleteRequested(root.highlightedJourneyID);
        }
    }
}
