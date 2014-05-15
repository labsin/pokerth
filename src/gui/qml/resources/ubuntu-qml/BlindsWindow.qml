import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import PokerTH.GameData 1.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

Page {
    id: window
    title: "Change Blinds Settings"
    property real initialWidth: 460
    property real initialHeight: 400
    property GameData gameData
    signal saveBlinds
    signal keepBlinds
    signal cancelBlinds
    width: initialWidth
    height: initialHeight
    objectName: "newGameDialog"
    visible: false

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
                cancelBlinds()
            }
        }
        Button {
            id: button_ok_customBets
            height: button_cancel.height
            text: "OK"
            iconSource: "../gfx/dialog_ok_apply.png"
            onClicked: {
                keepBlinds()
            }
        }
    }
}

