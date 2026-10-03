import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    signal finished()

    background: null
    padding: 12

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Hora de avaliar!")
                color: "white"
                font.pointSize: 10
            }

            Image {
                Layout.fillWidth: true
                Layout.fillHeight: true
                source: "qrc:/images/images/form-qr.png"
                fillMode: Image.PreserveAspectFit
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressAndHold: {
                root.finished();
            }
        }
    }
}
