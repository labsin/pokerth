import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import PokerTH 1.0 as PokerTH

ApplicationWindow {
    id: window
    width: minimumWidth
    height: minimumHeight
    minimumWidth: 1024
    minimumHeight: 600
    maximumWidth: 1920
    maximumHeight: 1280

    Image {
        id: background
        anchors.fill: parent
        source: "image://pokerth/table/table.png"
    }
    Item {
        id: container
        anchors.fill: parent
        anchors.margins: 13

        PathView {
            id: pathVieuw
            model: CurrentGame.model
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: buttonContainer.top
            interactive: false

            path: Path {
                id: path
                startX: pathVieuw.width/2
                startY: pathVieuw.height*4/5
                PathLine {
                    id: path1
                    x: pathVieuw.width*1/5; y: path.startY}
                PathArc {
                    id: arc1
                    direction: PathArc.Clockwise;
                    y: pathVieuw.height-path.startY; x: path1.x
                    radiusX: x - pathVieuw.width/10; radiusY: (path1.y-y)/2
                }
                PathLine {
                    id: path2
                    x: pathVieuw.width-path1.x; y: arc1.y}
                PathArc {
                    id: arc2
                    direction: PathArc.Clockwise;
                    y: path.startY; x: path2.x
                    radiusX: pathVieuw.width-x - pathVieuw.width/10
                    radiusY: (path2.y-y)/2
                }
                PathLine {
                    id: path3
                    x: path.startX; y: path.startY}
            }
            delegate: Player {
                player: model.player
            }
        }
        Row {
            anchors.centerIn: pathVieuw
            height: image_flop1.height
            spacing: 10
            Column {
                width: 130
                spacing: 10
                Label {
                    id: label_textPot
                    text: "Pot"
                    font.pointSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    height: 30
                }
                Item {
                    width: parent.width
                    height: 10
                    Label {
                        id: label_textTotal
                        text: "Total: "
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        width: parent.width/2
                        height: 10
                    }
                    Label {
                        id: label_potTotal
                        text: CurrentGame.pot
                        anchors.left: label_textTotal.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        height: 10
                        horizontalAlignment: Text.AlignLeft
                        width: parent.width/2
                    }
                }
                Item {
                    width: parent.width
                    height: 10
                    Label {
                        id: label_textBets
                        text: "Bets: "
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        width: parent.width/2
                        height: 10
                    }
                    Label {
                        id: label_bets
                        text: CurrentGame.set
                        anchors.left: label_textBets.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignLeft
                        width: parent.width/2
                        height: 10
                    }
                }
            }
            Image {
                id: image_flop1
                source: "image://PokerTH/table/cardholder_flop.png"
                width: flopCard1.implicitWidth
                height: flopCard1.implicitHeight
                Card {
                    id: flopCard1
                    anchors.fill: parent
                    cardObj: CurrentGame.flopCard1
                    visible: CurrentGame.flopCard1.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_flop.png"
                width: flopCard2.implicitWidth
                height: flopCard2.implicitHeight
                Card {
                    id: flopCard2
                    anchors.fill: parent
                    cardObj: CurrentGame.flopCard2
                    visible: CurrentGame.flopCard2.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_flop.png"
                width: flopCard3.implicitWidth
                height: flopCard3.implicitHeight
                Card {
                    id: flopCard3
                    anchors.fill: parent
                    cardObj: CurrentGame.flopCard3
                    visible: CurrentGame.flopCard3.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_turn.png"
                width: turnCard.implicitWidth
                height: turnCard.implicitHeight
                Card {
                    id: turnCard
                    anchors.fill: parent
                    cardObj: CurrentGame.turnCard
                    visible: CurrentGame.turnCard.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_river.png"
                width: riverCard.implicitWidth
                height: riverCard.implicitHeight
                Card {
                    id: riverCard
                    anchors.fill: parent
                    cardObj: CurrentGame.riverCard
                    visible: CurrentGame.riverCard.dealt
                }
            }
            Column {
                width: 130
                spacing: 10
                Label {
                    id: label_textStatus
                    text: gameStateString(CurrentGame.gameState)
                    font.pointSize: 18
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    height: 30
                    function gameStateString(gameState) {
                        switch(gameState) {
                        case PokerTH.Game.QML_GAME_STATE_PREFLOP:
                            return "Preflop"
                        case PokerTH.Game.QML_GAME_STATE_FLOP:
                            return "Flop"
                        case PokerTH.Game.QML_GAME_STATE_TURN:
                            return "Turn"
                        case PokerTH.Game.QML_GAME_STATE_POST_RIVER:
                        case PokerTH.Game.QML_GAME_STATE_RIVER:
                            return "River"
                        case PokerTH.Game.QML_GAME_STATE_PREFLOP_SMALL_BLIND:
                            return "Preflop SB"
                        case PokerTH.Game.QML_GAME_STATE_PREFLOP_BIG_BLIND:
                            return "Preflop BB"
                        default:
                            return "--ERROR--"
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: 10
                    Label {
                        id: label_textGame
                        text: "Game: "
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        width: parent.width/2
                        height: 10
                    }
                    Label {
                        id: label_game
                        text: CurrentGame.gameNr
                        anchors.left: label_textGame.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        height: 10
                        horizontalAlignment: Text.AlignLeft
                        width: parent.width/2
                    }
                }
                Item {
                    width: parent.width
                    height: 10
                    Label {
                        id: label_textHand
                        text: "Hand: "
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignRight
                        width: parent.width/2
                        height: 10
                    }
                    Label {
                        id: label_hand
                        text: CurrentGame.handNr
                        anchors.left: label_textHand.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignLeft
                        width: parent.width/2
                        height: 10
                    }
                }
            }
        }

        Item {
            id: buttonContainer
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height
            width: 200
            property int cash: 5000
            property bool betting: CurrentGame.highestSet ==0;
            property bool raising: !betting
            property bool checking: betting
            property bool calling: !checking
            property bool actionButtonsEnabled: CurrentGame.buttonsCheckable || CurrentGame.myTurn
            property int stepSize: 50

            RowLayout {
                id: row_raiseSpinBox
                anchors.left: parent.left
                anchors.right: parent.right
                SpinBox {
                    id: spinBox_raise
                    value: CurrentGame.bets
                    Layout.fillWidth: true
                    maximumValue: CurrentGame.myPlayer?CurrentGame.myPlayer.cash:minimumValue
                    minimumValue: CurrentGame.myPlayer?(buttonContainer.raising?
                                                         CurrentGame.highestSet-CurrentGame.myPlayer.set + CurrentGame.smallBlind
                                                       : CurrentGame.smallBlind*2
                                                     ):0
                    stepSize: buttonContainer.stepSize
                    anchors.left: parent.left
                    anchors.top: parent.top
                    prefix: "$"
                    onValueChanged: {
                        buttonContainer.setFromSpin(value)
                    }
                }
                Button {
                    id: button_allIn
                    anchors.left: spinBox_raise.right
                    anchors.right: parent.right
                    anchors.top: parent.top
                    enabled: buttonContainer.actionButtonsEnabled
                    onClicked: {
                        if(!CurrentGame.myTurn) {
                            if(CurrentGame.checkedButton==PokerTH.Game.AllInButton) {
                                CurrentGame.checkedButton=PokerTH.Game.NoButton
                            }
                            else {
                                CurrentGame.checkedButton=PokerTH.Game.AllInButton
                            }
                        }
                        else {
                            CurrentGame.allIn()
                        }
                    }

                    text: "All-In"
                }
            }
            Slider {
                id: slider_raise
                value: buttonContainer.fromSpinToSlider(CurrentGame.bets)
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: row_raiseSpinBox.bottom
                onValueChanged: {
                    buttonContainer.setFromSlider(value)
                }
            }
            Button {
                id: button_betRaise
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: slider_raise.bottom
                enabled: buttonContainer.actionButtonsEnabled
                onClicked: {
                    if(!CurrentGame.myTurn) {
                        if(CurrentGame.checkedButton==PokerTH.Game.BetRaiseButton) {
                            CurrentGame.checkedButton=PokerTH.Game.NoButton
                        }
                        else {
                            CurrentGame.checkedButton=PokerTH.Game.BetRaiseButton
                        }
                    }
                    else {
                        CurrentGame.betRaise()
                    }
                }
                text: buttonContainer.betting?"Bet":"Raise"
            }
            Button {
                id: button_call
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: button_betRaise.bottom
                enabled: buttonContainer.actionButtonsEnabled
                onClicked: {
                    if(!CurrentGame.myTurn) {
                        if(CurrentGame.checkedButton==PokerTH.Game.CallButton) {
                            CurrentGame.checkedButton=PokerTH.Game.NoButton
                        }
                        else {
                            CurrentGame.checkedButton=PokerTH.Game.CallButton
                        }
                    }
                    else {
                        if(buttonContainer.calling) {
                            CurrentGame.call()
                        }
                        else {
                            CurrentGame.check()
                        }
                    }
                }
                text: buttonContainer.calling?"Call":"Check"
            }
            Button {
                id: button_fold
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: button_call.bottom
                enabled: buttonContainer.actionButtonsEnabled
                onClicked: {
                    if(!CurrentGame.myTurn) {
                        if(CurrentGame.checkedButton==PokerTH.Game.FoldButton) {
                            CurrentGame.checkedButton=PokerTH.Game.NoButton
                        }
                        else {
                            CurrentGame.checkedButton=PokerTH.Game.FoldButton
                        }
                    }
                    else {
                        CurrentGame.fold()
                    }
                }
                text: "Fold"
            }
            Connections {
                target: CurrentGame
                onCheckedButtonChanged: {
                    switch(CurrentGame.checkedButton) {
                    case PokerTH.Game.NoButton:
                        button_betRaise.checked=false;
                        button_call.checked=false;
                        button_fold.checked=false;
                        button_allIn.checked=false;
                        break;
                    case PokerTH.Game.BetRaiseButton:
                        button_betRaise.checked=true;
                        break;
                    case PokerTH.Game.CallButton:
                        button_call.checked=true;
                        break;
                    case PokerTH.Game.FoldButton:
                        button_fold.checked=true;
                        break;
                    case PokerTH.Game.AllInButton:
                        button_allIn.checked=true
                        break;
                    }
                }
            }

            function setFromSpin(spin) {
                //var tmpSpin = roundOnStep(spin);
                var tmpSpin = spin;
                CurrentGame.bets = tmpSpin;
            }
            function setFromSlider(slider) {
                setFromSpin(fromSliderToSpin(slider));
            }

            function roundOnStep(value) {
                return Math.floor(value/stepSize)*stepSize
            }

            function fromSliderToSpin(slider) {
                var tmp = spinBox_raise.minimumValue
                        + slider*(spinBox_raise.maximumValue-spinBox_raise.minimumValue)
                //             tmp = roundOnStep(tmp)
                return tmp;
            }
            function fromSpinToSlider(spin) {
                return (spin - spinBox_raise.minimumValue)/(spinBox_raise.maximumValue-spinBox_raise.minimumValue)
            }
        }
        Slider {
            id: slider_speed
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            value: CurrentGame.gameSpeed
            onValueChanged: CurrentGame.gameSpeed = value
            maximumValue: 11
            minimumValue: 1
            stepSize: 1
        }
    }
}
