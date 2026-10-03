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

        Tumbler {
            id: eventsContainer
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            visibleItemCount: 3
            flickDeceleration: 3000

            model: root.events
            delegate: Label {
                readonly property bool isCurrentItem:
                    index === Tumbler.tumbler.currentIndex
                text: modelData.name
                color: isCurrentItem ? "lightblue" : "white"
                width: parent.width
                wrapMode: Text.WordWrap
                font.pointSize: 10
                minimumPointSize: 7
                fontSizeMode: Text.Fit
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Row {
            Layout.alignment: Qt.AlignHCenter
            spacing: 6

            Button {
                id: buttonReturn
                icon.source: "qrc:/resources/icons/chevron-backward.svg"
                icon.color: "white"
                icon.width: 20
                width: 40
                onClicked: {
                    root.returned();
                }
            }
            Button {
                id: button
                icon.source: "qrc:/resources/icons/pin-drop.svg"
                icon.color: "white"
                icon.width: 20
                width: 40
                onClicked: {
                    let highlightedEvent = root.events[eventsContainer.currentIndex];
                    JourneyViewModel.registerEvent(highlightedEvent.id);
                }
            }
        }
    }
}
