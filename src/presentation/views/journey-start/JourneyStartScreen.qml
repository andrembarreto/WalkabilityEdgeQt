import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import WalkabilityEdgeQt

Page {
    id: root
    signal started()

    background: null

    contentItem: ColumnLayout {
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Bem vindo(a)")
            color: "white"
            font.pointSize: 12
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Iniciar percurso")
            onClicked: {
                JourneyViewModel.start();
                root.started();
            }
        }
    }
}
