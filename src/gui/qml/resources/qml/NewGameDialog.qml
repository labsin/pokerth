import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import PokerTH.GameData 1.0

Window {
    id: window
    title: "Start Local Game"
    property real initialWidth: 300
    property real initialHeight: 270
    property GameData gameData: GameData { }
    width: initialWidth
    height: initialHeight
    minimumWidth: box.implicitWidth
    minimumHeight: 250
    maximumWidth: initialWidth>minimumHeight+150?initialWidth: minimumHeight+150
    maximumHeight: initialHeight>minimumHeight+30?initialHeight: minimumHeight+30
    flags: Qt.Window
    objectName: "newGameDialog"

    modality: Qt.WindowModal

    onVisibleChanged: {
        if(visible) {
            gameData.setToDefault()
        }
    }

    GroupBox {
        id: box
        title: qsTr("Local Game Settings")
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 8; anchors.rightMargin: 5; anchors.topMargin: 5; anchors.bottomMargin: 5
        anchors.bottom: buttonsRow.top
        GridLayout {
            id: grid
            rows: 4
            columns: 3
            anchors.fill: parent
            property real maxCol2Width: 1
            function checkCol2Width(width) {
                if(width>grid.maxCol2Width) {
                    grid.maxCol2Width = width
                }
            }

            Label {
                text: "Number of players:"
                Layout.column: 0
                Layout.row: 0
                Layout.minimumWidth: implicitWidth
            }
            SpinBox {
                id: spinBox_numbPlayers
                minimumValue: 2
                maximumValue: 10
                value: gameData.maxNumberOfPlayers
                onValueChanged: {
                    gameData.maxNumberOfPlayers = value
                }

                Layout.column: 1; Layout.columnSpan: 2
                Layout.row: 0
                Layout.fillWidth: true
                Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: grid.maxCol2Width
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onImplicitWidthChanged: grid.checkCol2Width((implicitWidth+5)*2)
            }
            Label {
                text: "Start Cash:"
                Layout.column: 0
                Layout.row: 1
                Layout.minimumWidth: implicitWidth
            }
            SpinBox {
                id: spinBox_startCash
                prefix: "$"
                minimumValue: 1000
                maximumValue: 1000000
                stepSize: 20
                value: gameData.startMoney
                onValueChanged: {
                    gameData.startMoney = value
                }
                Layout.column: 1; Layout.columnSpan: 2
                Layout.row: 1
                Layout.fillWidth: true
                Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: grid.maxCol2Width
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onImplicitWidthChanged: grid.checkCol2Width((implicitWidth+5)*2)
            }
            GroupBox {
                title: "Blinds"
                Layout.column: 0
                Layout.columnSpan: 3
                Layout.row: 2
                ColumnLayout {
                    ExclusiveGroup { id: group_blinds }
                    RadioButton {
                        id: radioButton_savedBlinds
                        text: "Use saved blinds settings"
                        exclusiveGroup: group_blinds
                        checked: gameData.defaultBlinds
                        onCheckedChanged: {
                            gameData.defaultBlinds = checked
                            if(checked) {
                                blindsWindow.close()
                            }
                        }
                    }
                    RadioButton {
                        id: radioButton_changedBlinds
                        text: "Change blinds settings..."
                        exclusiveGroup: group_blinds
                        checked: !gameData.defaultBlinds
                        onClicked: {
                                blindsWindow.show()
                        }
                    }
                }
            }
            Label {
                text: "Game Speed:"
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.row: 3
                Layout.minimumWidth: implicitWidth
            }
            SpinBox {
                id: spinBox_gameSpeed
                minimumValue: 1
                maximumValue: 11
                value: gameData.guiSpeed
                onValueChanged: {
                    gameData.guiSpeed=value
                }

                Layout.column: 2; Layout.row: 3
                Layout.fillWidth: true
                Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: (implicitWidth + 5)*2
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            }
        }
    }
    Row {
        id: buttonsRow
        anchors.bottom: parent.bottom
        anchors.right: box.right
        anchors.left: parent.left
        anchors.margins: box.anchors.rightMargin
        height: button_cancel.height
        spacing: 5
        layoutDirection: Qt.RightToLeft

        Button {
            id: button_cancel
            text: "Cancel"
            iconSource: "../gfx/dialog_close.png"
            onClicked: window.close()
        }
        Button {
            id: button_save
            height: button_cancel.height
            text: "Save"
            iconSource: "../gfx/filesave.png"
            onClicked: {

            }
        }
        Button {
            id: button_ok
            height: button_cancel.height
            text: "OK"
            iconSource: "../gfx/dialog_ok_apply.png"
            onClicked:{
                Manager.startGame(gameData)
            }
        }
    }

    BlindsWindow {
        id: blindsWindow
        gameData: window.gameData
        onKeepBlinds: {
            radioButton_changedBlinds.checked=true
            close()
        }
        onCancelBlinds: {
            radioButton_savedBlinds.checked=true
            close()
        }
    }
}
