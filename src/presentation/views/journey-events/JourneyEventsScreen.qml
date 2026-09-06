import QtQuick
import QtQuick.Controls

import WalkabilityEdgeQt

Page {
    id: root

    signal returned()
    signal eventSelected(event: JourneyEventViewModel)
    required property list<JourneyEventViewModel> events

    background: null

    contentItem: Item {

        ListView {
            anchors.fill: parent
            model: root.events
            delegate: Label {
                text: modelData.name
            }
        }
    }
}
