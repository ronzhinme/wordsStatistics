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
                    width: (root.width / 100) * wordPercentage
                    height: (root.height / SortFilterProxyInstance.maxRows) - 2
                    color: "lightgreen"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: wordStatText
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
