import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    background: null

    contentItem: Item {

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
                    { field: qsTr("Início"), value: "--" },
                    { field: qsTr("Fim"), value: "--" },
                    { field: qsTr("Duração"), value: "--" },
                    { field: qsTr("Distância"), value: "--" },
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
        }
    }
}
