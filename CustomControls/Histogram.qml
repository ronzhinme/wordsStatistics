import QtQuick 2.15
import QtQuick.Layouts 1.3

import WordModelInstance 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Repeater {
            id: repeater
            model: WordModelInstance

            Item {
                Rectangle {
                    width: (root.width / 100) * (wordCount / (WordModelInstance.totalWordCount / 100))
                    height: root.height / (repeater.count + 2)
                    color: "green"
                }

                Text {
                    text: display + " : " + wordCount
                }

                Behavior on y {SpringAnimation { spring: 2; damping: 0.2} }
            }
        }
    }
}
