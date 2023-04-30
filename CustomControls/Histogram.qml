import QtQuick 2.15
import QtQuick.Layouts 1.3

import WordModelInstance 1.0
import SortFilterProxyInstance 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Repeater {
            id: repeater
            model: SortFilterProxyInstance

            Item {
                visible: index < SortFilterProxyInstance.maxRows

                Rectangle {
                    id: rect
                    readonly property real percentage: wordCount / (WordModelInstance.totalWordCount / 100)
                    width: (root.width / 100) * percentage
                    height: (root.height / SortFilterProxyInstance.maxRows) - 2
                    color: "lightgreen"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "["+ display + "] : " + wordCount + " ("+rect.percentage.toFixed(2)+" %)"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
