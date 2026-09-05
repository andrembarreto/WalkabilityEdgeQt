import QtQuick
import QtQuick.Controls

Control {
    id: control

    signal selected(id: variant)
    required property variant options
    property int currentIndex: 0
    readonly property variant currentOption: options ? options[currentIndex] : null

    contentItem: Row {
        Button {
            id: buttonPrevious
            icon.source: "qrc:/resources/icons/chevron-backward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.currentIndex = (control.currentIndex - 1 + control.options.length) % control.options.length
            }
            enabled: control.options.length > 1
            flat: true
        }

        Button {
            id: buttonSelect
            text: control.currentOption.name
        }

        Button {
            id: buttonNext
            icon.source: "qrc:/resources/icons/chevron-forward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.currentIndex = (control.currentIndex + 1) % control.options.length
            }
            enabled: control.options.length > 1
            flat: true
        }
    }
}
