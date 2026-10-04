import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import WalkabilityEdgeQt

Page {
    id: root

    signal started()
    signal goToSavedJourneys()

    background: null

    contentItem: Item {

        Label {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }
            padding: 12
            text: qsTr("Bem vindo(a)")
            color: "white"
            font.pointSize: 12
        }

        Button {
            id: buttonStart
            anchors.centerIn: parent
            icon.source: "qrc:/resources/icons/play-circle.svg"
            icon.color: pressed ? "lightblue" : "white"
            icon.width: 50
            icon.height: 50
            flat: true
            onClicked: {
                JourneyViewModel.start();
                root.started();
            }
        }

        Button {
            id: buttonSavedJourneys
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            icon.source: "qrc:/resources/icons/folder-open.svg"
            icon.color: pressed ? "lightblue" : "white"
            flat: true
            onClicked: {
                root.goToSavedJourneys();
            }
        }
    }
}
