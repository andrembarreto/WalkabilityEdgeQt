import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import WalkabilityEdgeQt

Page {
    id: root

    signal returned()
    required property list<journeyEventViewModel> events

    // efeito para evento selecionado --> esmaece para indicar que ainda não foi registrado
    property real highlightOpacity: buttonConfirm.pressed && !buttonConfirm.confirmed ? 0.75 : 1.0
    property color highlightColor: "lightblue"

    Behavior on highlightOpacity {
        NumberAnimation { duration: 150 }
    }

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
                color: isCurrentItem ? root.highlightColor : "white"
                width: parent.width
                wrapMode: Text.WordWrap
                font.pointSize: 10
                minimumPointSize: 7
                fontSizeMode: Text.Fit
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                opacity: (1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2))
                         * (isCurrentItem ? root.highlightOpacity : 1.0)
                horizontalAlignment: Text.AlignHCenter
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Button {
                id: buttonReturn
                icon.source: "qrc:/resources/icons/chevron-backward.svg"
                onClicked: {
                    root.returned();
                }
            }
            Button {
                id: buttonConfirm
                property bool confirmed: false
                icon.source: "qrc:/resources/icons/pin-drop.svg"
                onPressed: {
                    confirmed = false;
                }
                onPressAndHold: {
                    let highlightedEvent = root.events[eventsContainer.currentIndex];
                    JourneyViewModel.registerEvent(highlightedEvent.id);
                    confirmed = true;
                    highlightFlash.restart();
                }
            }
        }
    }

    // piscadinha no texto do evento selecionado para mostrar registro
    SequentialAnimation {
        id: highlightFlash
        ColorAnimation {
            target: root
            property: "highlightColor"
            to: "orange"
            duration: 80
        }
        PauseAnimation { duration: 200 }
        ColorAnimation {
            target: root
            property: "highlightColor"
            to: "lightblue"
            duration: 300
        }
    }
}
