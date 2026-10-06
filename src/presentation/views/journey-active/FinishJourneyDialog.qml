import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    modal: true

    background: Rectangle {
        color: "black"
        radius: height / 2
    }

    contentItem: Item {

        Label {
            anchors {
                bottom: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            text: qsTr("Deseja finalizar o percurso?")
            color: "white"
            font.pointSize: 12
        }

        Row {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            spacing: 4

            Button {
                id: buttonNo
                text: qsTr("Não")
                onClicked: {
                    root.reject();
                }
            }

            Button {
                id: buttonYes
                text: qsTr("Sim")
                onClicked: {
                    root.accept();
                }
            }
        }
    }
}
