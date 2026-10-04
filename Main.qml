import QtQuick
import QtQuick.Controls
import QtCore

import WalkabilityEdgeQt

Window {
    id: window

    visible: true
    visibility: Window.FullScreen
    title: qsTr("Caminhabilidade")

    required property list<journeyDimensionViewModel> dimensions

    // Fundo preto: padrao em telas OLED de relogio e economiza bateria.
    color: "black"

    // Menor dimensao da tela
    readonly property real displaySize: Math.min(width, height)

    // Margem recomendada pelas diretrizes de layout do Wear OS para telas
    // redondas: 5,2% de cada lado. Adequada para conteudo comum
    readonly property real safeMargin: displaySize * 0.052

    // Margem do quadrado inscrito no circulo: (1 - 1/raiz(2)) / 2 ~= 14,64%.
    // E a unica regiao que nunca e cortada pela curvatura, em nenhum ponto.
    // Em 450 px resulta em ~66 px de margem e ~318x318 de area util.
    // Use para conteudo que precise dessa garantia estrita.
    readonly property real inscribedMargin: displaySize * (1.0 - 1.0 / Math.sqrt(2.0)) / 2.0

    StackView {
        id: stack

        anchors.fill: parent
        anchors.margins: window.safeMargin
        initialItem: JourneyViewModel.canResume()
                     ? resumeJourneyScreen
                     : journeyStartScreen

        Component {
            id: resumeJourneyScreen

            ResumeJourneyScreen {
                onAccepted: {
                    JourneyViewModel.resume();
                    stack.replace(journeyStartScreen);
                    stack.push(journeyActiveScreen);
                }
                onRejected: {
                    JourneyViewModel.discard();
                    stack.replace(journeyStartScreen);
                }
            }
        }

        Component {
            id: journeyStartScreen

            JourneyStartScreen {
                onStarted: {
                    stack.push(journeyActiveScreen);
                }
                onGoToSavedJourneys: {
                    stack.push(savedJourneysScreen);
                }
            }
        }

        Component {
            id: savedJourneysScreen

            SavedJourneysScreen {
                onFinished: {
                    stack.pop();
                }
                onDispatchRequested: function(journeyID) {
                    stack.push(journeyFinishedScreen, { journeyID: journeyID });
                }
                onEvalRequested: function(journeyID) {
                    stack.push(evaluateJourneyScreen, { journeyID: journeyID });
                }
                onDeleteRequested: function(journeyID) {
                    // TODO: handle delete request
                }
            }
        }

        Component {
            id: journeyActiveScreen

            JourneyActiveScreen {
                dimensions: window.dimensions
                onDimensionSelected: function(dimension) {
                    stack.push(journeyEventsScreen, { events: dimension.events });
                }
                onFinished: {
                    stack.replace(journeyFinishedScreen);
                }
            }
        }

        Component {
            id: journeyEventsScreen

            JourneyEventsScreen {
                onReturned: {
                    stack.pop();
                }
            }
        }

        Component {
            id: journeyFinishedScreen

            JourneyFinishedScreen {
                id: finishedScreen
                onFinished: {
                    stack.push(evaluateJourneyScreen, { journeyID: finishedScreen.journeyID });
                }
                onDiscarded: {
                    stack.replace(journeyStartScreen);
                }
                onReturned: {
                    stack.pop();
                }
            }
        }

        Component {
            id: evaluateJourneyScreen

            EvaluateJourneyScreen {
                id: evaluateScreen
                onFinished: {
                    ScoreCalculatorViewModel.calculate(evaluateScreen.journeyID);
                    stack.push(journeyResultScreen, { journeyID: evaluateScreen.journeyID });
                }
            }
        }

        Component {
            id: journeyResultScreen

            JourneyResultScreen {
                onFinished: {
                    stack.replace(journeyStartScreen);
                }
            }
        }
    }
}
