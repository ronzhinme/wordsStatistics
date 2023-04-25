import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Qt.labs.platform 1.1

ToolBar
{
    FileDialog {
        id: openFileDialog
        fileMode: FileDialog.OpenFile
    }

    ColumnLayout {
        RowLayout {
            ToolButton {
                text: qsTr("Open")
                onClicked: {
                    openFileDialog.open()
                }
            }

            Text {
                id: filePath
                text: openFileDialog.file
            }
        }

        RowLayout
        {
            property bool stateStarted : false
            property bool statePaused : false
            ToolButton {
                text: qsTr("Start")
                enabled: !parent.stateStarted && filePath.text !== ""
                onClicked: {
                    parent.stateStarted = !parent.stateStarted
                }
            }

            ToolButton {
                text: parent.statePaused ? qsTr("Resume") : qsTr("Pause")
                enabled: parent.stateStarted
                onClicked: {
                    parent.statePaused = !parent.statePaused
                }
            }

            ToolButton {
                text: qsTr("Cancel")
                enabled: parent.stateStarted
                onClicked: {
                    parent.statePaused = parent.stateStarted = false
                }
            }
        }
    }
}
