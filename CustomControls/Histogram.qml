import QtQuick 2.15
import QtQuick.Layouts 1.3

Item {
    id: root
    RowLayout {
        anchors.fill: parent
        spacing: 10

        Repeater {
            model: 15
            Rectangle {
                Layout.alignment: Qt.AlignBottom
                height: 0 + (10 * index)
                width: root.width / 20
                color: "green"
            }
        }
    }
}
