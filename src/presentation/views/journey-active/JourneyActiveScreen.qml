import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import WalkabilityEdgeQt

Page {
    id: root

    signal stopped()
    signal dimensionSelected(dimension: JourneyDimensionViewModel)
    required property list<JourneyDimensionViewModel> dimensions

    background: null

    contentItem: Item {

        OptionSelector {
            anchors.centerIn: parent
            anchors.bottomMargin: 24
            options: root.dimensions
            onSelected: function(dimensionID) {
                const dimension = root.dimensions.find((d) => d.id === dimensionID);
                if(dimension)
                    root.dimensionSelected(dimension);
            }
        }

        Column {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: buttonStop
                padding: 2

                contentItem: Label {
                    anchors.centerIn: parent
                    text: qsTr("Finalizar")
                    font.pointSize: 10
                    color: "red"
                }

                background: Rectangle {
                    color: "red"
                    opacity: buttonStop.pressed ? 0.15 : 0
                    radius: height / 2
                }

                onClicked: {
                    root.stopped();
                }
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: formatTime(JourneyViewModel.elapsedTime)
                color: "white"
                font.pointSize: 12
            }
        }
    }

    function formatTime(timeInSeconds: int): string {
        const hours = Math.floor(timeInSeconds / 3600).toString().padStart(2, '0');
        const minutes = Math.floor(timeInSeconds % 3600 / 60).toString().padStart(2, '0');
        const seconds = Math.floor(timeInSeconds % 3600 % 60).toString().padStart(2, '0');
        return `${hours}:${minutes}:${seconds}`
    }
}
