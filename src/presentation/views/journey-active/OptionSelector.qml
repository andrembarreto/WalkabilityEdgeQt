import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Control {
    id: control

    signal selected(id: variant)
    required property variant options
    property int currentIndex: 0
    readonly property variant currentOption: options ? options[currentIndex] : null

    contentItem: Row {
        Button {
            id: buttonPrevious
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/resources/icons/chevron-backward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.currentIndex = (control.currentIndex - 1 + control.options.length) % control.options.length;
            }
            enabled: control.options.length > 1
            flat: true
        }

        Button {
            id: buttonSelect
            anchors.verticalCenter: parent.verticalCenter
            implicitWidth: 100

            contentItem: Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: parent.spacing

                Item {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: iconOption.height; width: iconOption.width

                    Image {
                        id: iconOption
                        fillMode: Image.PreserveAspectFit
                        source: control.currentOption.icon
                        sourceSize.height: 24
                    }
                    MultiEffect {
                        anchors.fill: iconOption
                        source: iconOption
                        colorization: 1.0
                        colorizationColor: "black"
                    }
                }
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: control.currentOption.name
                    font.pointSize: 8
                    color: "black"
                }
            }
            onClicked: {
                control.selected(control.currentOption.id);
            }
        }

        Button {
            id: buttonNext
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/resources/icons/chevron-forward.svg"
            icon.color: "white"
            icon.width: 20
            onClicked: {
                control.currentIndex = (control.currentIndex + 1) % control.options.length;
            }
            enabled: control.options.length > 1
            flat: true
        }
    }
}
