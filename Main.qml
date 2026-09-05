import QtQuick
import QtQuick.Controls

import WalkabilityEdgeQt

Window {
    id: window

    visible: true
    visibility: Window.FullScreen
    title: qsTr("Caminhabilidade")

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

    // Area util da interface. O conteudo da aplicacao entra aqui dentro.
    StackView {
        id: stack

        anchors.fill: parent
        anchors.margins: window.safeMargin
        initialItem: journeyStartScreen

        Component {
            id: journeyStartScreen

            JourneyStartScreen {
                onStarted: {
                    stack.push(journeyActiveScreen);
                }
            }
        }

        Component {
            id: journeyActiveScreen

            JourneyActiveScreen {

            }
        }
    }
}
