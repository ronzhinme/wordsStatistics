import QtQuick 2.15
import QtQuick.Layouts 1.3

Item {
    property alias model: repeater.model
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Repeater {
            id: repeater

            Item {
                Rectangle {
                    width: 1 + (root.width / 100) * index
                    height: root.height / (repeater.count + 2)
                    color: "green"
                }

                Text {
                    text: "word : " + index
                }

                Behavior on y {SpringAnimation { spring: 2; damping: 0.2} }
            }
        }
    }
}
