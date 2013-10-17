import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import PokerTH 1.0
import PokerTH.GameData 1.0

Window {
    id: window
    title: "Start Local Game"
    property real initialWidth: 300
    property real initialHeight: 270
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

    Window {
        id: blindsWindow
        title: "Change Blinds Settings"
        property real initialWidth: 460
        property real initialHeight: 400
        width: initialWidth
        height: initialHeight
        minimumWidth: mainGrid.implicitWidth
        minimumHeight: 350
        maximumWidth: initialWidth>minimumHeight+150?initialWidth: minimumHeight+150
        maximumHeight: initialHeight>minimumHeight+30?initialHeight: minimumHeight+30
        flags: Qt.Window
        objectName: "newGameDialog"

        modality: Qt.WindowModal
        GridLayout {
            id: mainGrid
            rows: 3
            columns: 2
            anchors.left: parent.left; anchors.right: parent.right
            anchors.top: parent.top; anchors.bottom: buttonsRow_customBets.top
            anchors.leftMargin: 8; anchors.rightMargin: 5; anchors.topMargin: 5; anchors.bottomMargin: 5
            Label {
                text: "First small blind:"
            }
            SpinBox {
                id: spinBox_firstSmallBlind
                Layout.fillWidth: true
                minimumValue: 5
                maximumValue: 20000
                Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: (implicitWidth + 5)*2
                value: gameData.firstSmallBlind
                onValueChanged: {
                    gameData.firstSmallBlind = value
                }
            }
            GroupBox {
                title: "Raise blinds:"
                Layout.fillWidth: true
                ExclusiveGroup { id: group_blindsRaises }
                GridLayout {
                    rows: 2
                    columns: 3
                    RadioButton {
                        id: radioButton_raiseBlindsAtHands
                        text: "Every:"
                        exclusiveGroup: group_blindsRaises
                        checked: gameData.raiseIntervalMode === GameData.RAISE_ON_HANDNUMBER
                        onCheckedChanged: {
                            if(checked) {
                                gameData.raiseIntervalMode = GameData.RAISE_ON_HANDNUMBER
                            }
                        }
                    }
                    SpinBox {
                        id: spinBox_raiseSmallBlindsAtHands
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50; Layout.maximumWidth: (implicitWidth + 5)*2
                        value: gameData.raiseSmallBlindEveryHandsValue
                        onValueChanged: {
                            gameData.raiseSmallBlindEveryHandsValue = value
                        }
                        minimumValue: 1
                    }
                    Label {
                        text: "hands"
                    }

                    RadioButton {
                        id: radioButton_raiseBlindsAtMinutes
                        text: "Every:"
                        exclusiveGroup: group_blindsRaises
                        checked: gameData.raiseIntervalMode === GameData.RAISE_ON_MINUTES
                        onCheckedChanged: {
                            if(checked) {
                                gameData.raiseIntervalMode = GameData.RAISE_ON_MINUTES
                            }
                        }
                    }
                    SpinBox {
                        id: spinBox_raiseSmallBlindsAtMinutes
                        Layout.fillWidth: true
                        Layout.minimumWidth: 50; Layout.maximumWidth: (implicitWidth + 5)*2
                        value: gameData.raiseSmallBlindEveryMinutesValue
                        onValueChanged: {
                            gameData.raiseSmallBlindEveryMinutesValue = value
                        }
                        minimumValue: 1
                    }
                    Label {
                        text: "minutes"
                    }
                }
            }
            GroupBox {
                title: "Raise mode:"
                ExclusiveGroup { id: group_blindsRaisModes }
                ColumnLayout {
                    RadioButton {
                        id: radioButton_alwaysDoubleBlinds
                        text: "Always double blinds"
                        exclusiveGroup: group_blindsRaisModes
                        checked: gameData.raiseMode === GameData.DOUBLE_BLINDS
                        onCheckedChanged: {
                            if(checked) {
                                gameData.raiseMode = GameData.DOUBLE_BLINDS
                            }
                        }
                    }

                    RadioButton {
                        id: radioButton_manualBlindsOrder
                        text: "Manual blinds order:"
                        exclusiveGroup: group_blindsRaisModes
                        checked: gameData.raiseMode === GameData.MANUAL_BLINDS_ORDER
                        onCheckedChanged: {
                            if(checked) {
                                gameData.raiseMode = GameData.MANUAL_BLINDS_ORDER
                            }
                        }
                    }
                }
            }
            GroupBox {
                title: "Manual blinds order:"
                Layout.columnSpan: 2
                enabled: radioButton_manualBlindsOrder.checked
                RowLayout {
                    ColumnLayout {
                        TableView {
                            id: listView_customBlinds
                            Layout.fillWidth: true
                            model: gameData.manualBlindsList
                            TableViewColumn{ role: "modelData"  ; title: "Blind"; width: listView_customBlinds.width }
                            function sanityCheck() {
                                //TODO
                            }
                        }
                        SpinBox {
                            id: spinBox_input
                            Layout.fillWidth: true
                            Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: (implicitWidth + 5)*2
                            maximumValue: 20000
                            minimumValue: spinBox_firstSmallBlind.value+1
                            onMinimumValueChanged: {
                                listView_customBlinds.sanityCheck()
                            }
                        }
                    }
                    ColumnLayout {
                        Layout.alignment: Qt.AlignTop
                        Layout.maximumWidth: button_deleteBlind.width
                        Layout.minimumWidth: button_deleteBlind.width
                        Layout.fillHeight: true
                        Button {
                            id: button_deleteBlind
                            Layout.alignment: Qt.AlignTop
                            text: "Delete"
                        }
                        Button {
                            id: button_addBlind
                            Layout.alignment: Qt.AlignBottom
                            text: "Add"
                        }
                    }
                    GroupBox {
                        Layout.alignment: Qt.AlignBottom
                        Layout.fillWidth: true
                        title: "Afterwards:"
                        GridLayout {
                            rows: 3
                            columns: 2
                            ExclusiveGroup {
                                id: group_afterManualBlindsMode
                            }
                            RadioButton {
                                id: radioButton_afterAlwaysDoubleBlinds
                                property int afterMBMode: GameData.AFTERMB_DOUBLE_BLINDS
                                Layout.columnSpan: 2
                                checked: gameData.afterManualBlindsMode === afterMBMode
                                onCheckedChanged: {
                                    if(checked) {
                                        gameData.afterManualBlindsMode = afterMBMode
                                    }
                                }
                                text: "Always double blinds"
                                exclusiveGroup: group_afterManualBlindsMode
                            }
                            RadioButton {
                                id: radioButton_afterAlwaysRaiseAbout
                                property int afterMBMode: GameData.AFTERMB_RAISE_ABOUT
                                checked: gameData.afterManualBlindsMode === afterMBMode
                                onCheckedChanged: {
                                    if(checked) {
                                        gameData.afterManualBlindsMode = afterMBMode
                                    }
                                }
                                text: "Always raise by:"
                                exclusiveGroup: group_afterManualBlindsMode
                            }
                            SpinBox {
                                prefix: "$"
                                Layout.fillWidth: true
                                Layout.minimumWidth: implicitWidth + 5; Layout.maximumWidth: (implicitWidth + 5)*2
                                maximumValue: 20000
                                value: gameData.afterMBAlwaysRaiseValue
                                onValueChanged: {
                                    gameData.afterMBAlwaysRaiseValue = value
                                }
                            }
                            RadioButton {
                                id: radioButton_afterStayAtLastBlind
                                property int afterMBMode: GameData.AFTERMB_STAY_AT_LAST_BLIND
                                Layout.columnSpan: 2
                                checked: gameData.afterManualBlindsMode === afterMBMode
                                onCheckedChanged: {
                                    if(checked) {
                                        gameData.afterManualBlindsMode = afterMBMode
                                    }
                                }
                                text: "Keep last blind"
                                exclusiveGroup: group_afterManualBlindsMode
                            }
                        }
                    }
                }
            }
        }

        Row {
            id: buttonsRow_customBets
            anchors.bottom: parent.bottom
            anchors.right: mainGrid.right
            anchors.left: parent.left
            anchors.margins: mainGrid.anchors.rightMargin
            height: button_cancel_customBets.height
            spacing: 5
            layoutDirection: Qt.RightToLeft

            Button {
                id: button_cancel_customBets
                text: "Cancel"
                iconSource: "../gfx/dialog_close.png"
                onClicked: {
                    radioButton_savedBlinds.enabled = true
                    blindsWindow.close()
                }
            }
            Button {
                id: button_ok_customBets
                height: button_cancel.height
                text: "OK"
                iconSource: "../gfx/dialog_ok_apply.png"
                onClicked: {
                    radioButton_changedBlinds.enabled = true
                    blindsWindow.close()
                }
            }
        }
    }
    GameData {
        id: gameData
    }
}
