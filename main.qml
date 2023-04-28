import QtQuick
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import WordModelInstance 1.0

Window {
    width: 640
    height: 480
    minimumHeight: 400
    minimumWidth: 200
    visible: true
    title: qsTr("Words Statistics")

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10

        ToolPanel {
            Layout.fillWidth: true
        }

        Histogram {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ProgressBar {
            Layout.fillWidth: true
            from: 0
            to: 100
            value: WordModelInstance.percentage
        }
    }
}
