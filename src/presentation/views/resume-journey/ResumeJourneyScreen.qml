import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    signal accepted()
    signal rejected()

    background: null

    contentItem: Item {

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.85

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: qsTr("Percurso em andamento. Deseja retomar?")
                color: "white"
                font.pointSize: 10
                wrapMode: Text.WordWrap
            }

            RowLayout {
                Button {
                    text: qsTr("Não")
                    onClicked: {
                        root.rejected();
                    }
                }
                Button {
                    text: qsTr("Sim")
                    onClicked: {
                        root.accepted();
                    }
                }
            }
        }
    }
}
