import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import WalkabilityEdgeQt

Page {
    id: root

    signal returned()
    required property list<JourneyEventViewModel> events

    background: null

    contentItem: ColumnLayout {

        Button {
            id: buttonReturn
            Layout.alignment: Qt.AlignHCenter
            display: Button.TextBesideIcon
            Material.foreground: "white"
            Material.background: pressed ? "lightblue" : "transparent"
            text: "Voltar"
            font.pointSize: 10
            icon.source: "qrc:/resources/icons/chevron-backward.svg"
            icon.height: 12
            icon.width: 12
            onClicked: {
                root.returned();
            }
        }

        Tumbler {
            id: eventsContainer
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: root.events
            delegate: Label {
                text: modelData.name
                color: "white"
                font.pointSize: index === Tumbler.tumbler.currentIndex ? 12 : 8
                opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                padding: 8
            }
        }

        Button {
            id: button
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Registrar")
            font.pointSize: 10
            onClicked: {
                let highlightedEvent = root.events[eventsContainer.currentIndex];
                JourneyViewModel.registerEvent(highlightedEvent.id);
            }
        }
    }
}
