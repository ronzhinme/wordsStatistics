import QtQuick
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Words Statistics")

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        ToolPanel {
            Layout.fillWidth: true
        }

        Histogram {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
        }

        ProgressBar {
            Layout.fillWidth: true
            from: 0
            to: 100
            value: 50
        }
    }
}
