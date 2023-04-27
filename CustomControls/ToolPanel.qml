import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Dialogs

import ControllerInstance 1.0

ToolBar
{
    id: toolbar
    property bool stateStarted : false
    property bool statePaused : false

    FileDialog {
        id: openFileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            filePath.text = selectedFile
        }
    }

    ColumnLayout {
        RowLayout {
            ToolButton {
                text: qsTr("Open")
                enabled: !toolbar.stateStarted
                onClicked: {
                    openFileDialog.open()
                }
            }

           Text {
                id: filePath
                color: "gray"
                text: "Selected file path"
            }
        }

        RowLayout
        {
            ToolButton {
                text: qsTr("Start")
                enabled: !toolbar.stateStarted && filePath.text.startsWith("file:/")
                onClicked: {
                    toolbar.stateStarted = !toolbar.stateStarted
                    ControllerInstance.start(filePath.text)
                }
            }

            ToolButton {
                text: toolbar.statePaused ? qsTr("Resume") : qsTr("Pause")
                enabled: toolbar.stateStarted
                onClicked: {
                    toolbar.statePaused = !toolbar.statePaused
                }
            }

            ToolButton {
                text: qsTr("Cancel")
                enabled: toolbar.stateStarted
                onClicked: {
                    toolbar.statePaused = toolbar.stateStarted = false
                }
            }
        }
    }
}
