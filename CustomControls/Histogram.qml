import QtQuick 2.15
import QtQuick.Layouts 1.3

import WordModelInstance 1.0
import SortFilterProxyInstance 1.0

ListView {
    spacing: 2
    model: SortFilterProxyInstance
    interactive: false
    delegate: Rectangle {
        visible: index < SortFilterProxyInstance.maxRows
        id: rect
        width: (ListView.view.width / 100) * wordPercentage
        height: (ListView.view.height / SortFilterProxyInstance.maxRows) - 2
        color: "lightgreen"

        Text {
            visible: index < SortFilterProxyInstance.maxRows
            text: wordStatText !== undefined ? wordStatText : ""
        }
    }
}
