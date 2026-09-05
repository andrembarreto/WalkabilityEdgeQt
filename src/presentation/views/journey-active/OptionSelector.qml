import QtQuick
import QtQuick.Controls

Control {
    id: control

    signal selected(option: variant)
    required property list<variant> options
    property int displayedIndex: 0
    readonly property variant displayedOption: options ? options[displayedIndex] : null

    contentItem: Row {
        Button {
            id: buttonPrevious
            icon.source: "qrc:/resources/icons/chevron-backward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.displayedIndex = (control.displayedIndex - 1 + control.options.length) % control.options.length
            }
            enabled: control.options.length > 1
            flat: true
        }

        Button {
            id: buttonSelect
            text: control.displayedOption
        }

        Button {
            id: buttonNext
            icon.source: "qrc:/resources/icons/chevron-forward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.displayedIndex = (control.displayedIndex + 1) % control.options.length
            }
            enabled: control.options.length > 1
            flat: true
        }
    }
}
