import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import WalkabilityEdgeQt

Page {
    id: root

    background: null

    contentItem: StackLayout {
        currentIndex: {
            switch(ScoreCalculatorViewModel.status) {
                case(ScoreCalculatorViewModel.Idle):
                    return 0;
                case(ScoreCalculatorViewModel.Calculating):
                    return 1;
                case(ScoreCalculatorViewModel.Ready):
                    return 2;
                case (ScoreCalculatorViewModel.Failed):
                    return 3;
            }
        }

        Item {
            id: calculateFrame

            Button {
                anchors.centerIn: parent
                text: qsTr("Calcular pontuação")
                onClicked: {
                    ScoreCalculatorViewModel.calculate();
                }
            }
        }

        Item {
            id: calculatingFrame

            BusyIndicator {
                anchors.centerIn: parent
            }
        }

        Item {
            id: resultFrame

            property int currentIndex: 0
            readonly property list<dimensionScoreViewModel> scores:
                ScoreCalculatorViewModel.score.dimensionScores
            property dimensionScoreViewModel currentScore:
                scores[currentIndex]

            Label {
                id: title
                anchors {
                    top: parent.top
                    topMargin: 12
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("Pontuação")
                color: "white"
                font.bold: true
                font.pointSize: 10
            }

            Button {
                id: buttonPrevious
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                icon.source: "qrc:/resources/icons/chevron-backward.svg"
                icon.color: "white"
                icon.width: 20
                width: 40
                flat: true
                enabled: resultFrame.scores.length > 1
                onClicked: {
                    resultFrame.currentIndex =
                            (resultFrame.currentIndex - 1 + resultFrame.scores.length)
                            % resultFrame.scores.length;
                }
            }

            ColumnLayout {
                anchors.centerIn: parent
                visible: resultFrame.currentScore !== undefined

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: resultFrame.currentScore.name
                    color: "white"
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: resultFrame.currentScore.value.toFixed(1)
                    color: "white"
                }
            }

            Button {
                id: buttonNext
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                icon.source: "qrc:/resources/icons/chevron-forward.svg"
                icon.color: "white"
                icon.width: 20
                width: 40
                flat: true
                enabled: resultFrame.scores.length > 1
                onClicked: {
                    resultFrame.currentIndex =
                            (resultFrame.currentIndex + 1)
                            % resultFrame.scores.length;
                }
            }

            Label {
                id: global
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 12
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("Pontuação global: %1")
                .arg(ScoreCalculatorViewModel.score.global.toFixed(1))

                color: "white"
                font.pointSize: 10
            }
        }

        Item {
            id: errorFrame

            ColumnLayout {
                anchors.centerIn: parent
                width: parent.width * 0.85
                spacing: 24

                Label {
                    Layout.fillWidth: true
                    text: qsTr("Ocorreu um erro ao calcular a pontuação.")
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                Button {
                    id: retryButton
                    Layout.alignment: Qt.AlignHCenter
                    icon.source: "qrc:/resources/icons/replay.svg"
                    onClicked: {
                        ScoreCalculatorViewModel.calculate();
                    }
                }
            }
        }
    }
}
