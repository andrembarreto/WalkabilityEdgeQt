import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import WalkabilityEdgeQt

Page {
    id: root

    signal dispatchRequested(journeyID: int)
    signal evalRequested(journeyID: int)
    signal deleteRequested(journeyID: int)
    signal finished()

    property int currentIndex: 0
    readonly property list<savedJourneyViewModel> journeys: SavedJourneysViewModel.journeys
    readonly property bool hasJourneys: journeys.length > 0
    property savedJourneyViewModel currentJourney: journeys[currentIndex]

    background: null

    StackView.onActivated: {
        SavedJourneysViewModel.fetch();
    }

    Connections {
        target: SavedJourneysViewModel

        function onJourneysChanged() {
            root.currentIndex = Math.max(0, Math.min(root.currentIndex, root.journeys.length - 1));
        }
    }

    contentItem: Item {

        ColumnLayout {
            anchors.fill: parent

            Button {
                id: buttonFinish
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                icon.source: "qrc:/resources/icons/home.svg"
                icon.color: pressed ? "lightblue" : "white"
                flat: true
                onClicked: {
                    root.finished();
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true

                Button {
                    id: buttonPrevious
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "qrc:/resources/icons/chevron-backward.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    icon.width: 20
                    flat: true
                    enabled: root.journeys.length > 1
                    onClicked: {
                        root.currentIndex =
                                (root.currentIndex - 1 + root.journeys.length)
                                % root.journeys.length;
                    }
                }

                Label {
                    id: labelInfo
                    Layout.fillWidth: true
                    text: root.hasJourneys
                          ? root.formatInfo(root.currentJourney)
                          : qsTr("Nenhuma jornada salva")
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    color: "white"
                }

                Button {
                    id: buttonNext
                    Layout.alignment: Qt.AlignRight
                    icon.source: "qrc:/resources/icons/chevron-forward.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    icon.width: 20
                    flat: true
                    enabled: root.journeys.length > 1
                    onClicked: {
                        root.currentIndex =
                                (root.currentIndex + 1)
                                % root.journeys.length;
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

                Button {
                    id: buttonDelete
                    icon.source: "qrc:/resources/icons/delete.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    flat: true
                    enabled: root.hasJourneys
                    onPressAndHold: {
                        deleteDialog.open();
                    }
                }

                Button {
                    id: buttonEval
                    icon.source: "qrc:/resources/icons/reviews.svg"
                    icon.color: pressed ? "lightblue" : "white"
                    flat: true
                    enabled: root.hasJourneys
                    onClicked: {
                        if(!root.currentJourney.dispatched)
                            root.dispatchRequested(root.currentJourney.id);
                        else
                            root.evalRequested(root.currentJourney.id);
                    }
                }
            }
        }
    }

    function formatDuration(seconds: int): string {
        if(seconds < 60)
            return qsTr("%1 segundos").arg(seconds);

        const minutes = Math.floor(seconds / 60);
        return minutes === 1 ? qsTr("1 minuto") : qsTr("%1 minutos").arg(minutes);
    }

    function formatInfo(journey: savedJourneyViewModel): string {
        const locale = Qt.locale("pt_BR");
        const day = journey.date.toLocaleDateString(locale, "dddd, dd/MM/yy");
        const time = journey.date.toLocaleTimeString(locale, "HH:mm:ss");

        return "%1<br>%2<br>%3"
            .arg(day.charAt(0).toUpperCase() + day.slice(1))
            .arg(qsTr("Início às %1").arg(time))
            .arg(formatDuration(journey.duration_s));
    }

    DeleteJourneyDialog {
        id: deleteDialog
        anchors.centerIn: parent
        height: parent.height
        width: parent.width

        onAccepted: {
            root.deleteRequested(root.currentJourney.id);
        }
    }
}
