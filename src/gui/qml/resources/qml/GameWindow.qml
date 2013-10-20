import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import PokerTH 1.0

ApplicationWindow {
    id: window
    width: minimumWidth
    height: minimumHeight
    minimumWidth: 1024
    minimumHeight: 600
    maximumWidth: 1920
    maximumHeight: 1280
    title: thisGame.title
    property Game thisGame: CurrentGame

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
            model: thisGame.model
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
            delegate: PlayerObj {
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
                        text: thisGame.pot
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
                        text: thisGame.highestSet
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
                CardObj {
                    id: flopCard1
                    anchors.fill: parent
                    cardObj: thisGame.flopCard1
                    visible: thisGame.flopCard1.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_flop.png"
                width: flopCard2.implicitWidth
                height: flopCard2.implicitHeight
                CardObj {
                    id: flopCard2
                    anchors.fill: parent
                    cardObj: thisGame.flopCard2
                    visible: thisGame.flopCard2.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_flop.png"
                width: flopCard3.implicitWidth
                height: flopCard3.implicitHeight
                CardObj {
                    id: flopCard3
                    anchors.fill: parent
                    cardObj: thisGame.flopCard3
                    visible: thisGame.flopCard3.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_turn.png"
                width: turnCard.implicitWidth
                height: turnCard.implicitHeight
                CardObj {
                    id: turnCard
                    anchors.fill: parent
                    cardObj: thisGame.turnCard
                    visible: thisGame.turnCard.dealt
                }
            }
            Image {
                source: "image://PokerTH/table/cardholder_river.png"
                width: riverCard.implicitWidth
                height: riverCard.implicitHeight
                CardObj {
                    id: riverCard
                    anchors.fill: parent
                    cardObj: thisGame.riverCard
                    visible: thisGame.riverCard.dealt
                }
            }
            Column {
                width: 130
                spacing: 10
                Label {
                    id: label_textStatus
                    text: ""
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
                        text: thisGame.gameNr
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
                        text: thisGame.handNr
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
            property int minimumRaiseDiff: 16
            property int minimumRaise: thisGame.highestSet+minimumRaiseDiff
            property int firstNotPossibleStep: Math.floor(minimumRaise/stepSize)*stepSize
            property bool betting: thisGame.highestSet ==0;
            property bool raising: !betting
            property bool checking: betting
            property bool calling: !checking
            property int stepSize: 50
            RowLayout {
                id: row_raiseSpinBox
                anchors.left: parent.left
                anchors.right: parent.right
                SpinBox {
                    id: spinBox_raise
                    value: thisGame.bets
                    Layout.fillWidth: true
                    maximumValue: thisGame.myPlayer?thisGame.myPlayer.cash:minimumValue
                    minimumValue: buttonContainer.raising?
                                      thisGame.highestSet-thisGame.myPlayer.set + thisGame.minimumRaise
                                    : thisGame.smallBlind*2
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
                    checkable: thisGame.buttonsCheckable
                    checked: thisGame.checkedButton==Game.AllInButton
                    onClicked: {
                        if(thisGame.buttonsCheckable) {
                            if(thisGame.checkedButton==Game.AllInButton) {
                                thisGame.checkedButton=Game.NoButton
                            }
                            else {
                                thisGame.checkedButton=Game.AllInButton
                            }
                        }
                        else {
                            thisGame.allIn()
                        }
                    }

                    text: "All-In"
                }
            }
            Slider {
                id: slider_raise
                value: buttonContainer.fromSpinToSlider(thisGame.bets)
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
                checkable: thisGame.buttonsCheckable
                checked: thisGame.checkedButton==Game.BetRaiseButton
                onClicked: {
                    if(thisGame.buttonsCheckable) {
                        if(thisGame.checkedButton==Game.BetRaiseButton) {
                            thisGame.checkedButton=Game.NoButton
                        }
                        else {
                            thisGame.checkedButton=Game.BetRaiseButton
                        }
                    }
                    else {
                        thisGame.set()
                    }
                }
                text: buttonContainer.betting?"Bet":"Raise"
            }
            Button {
                id: button_call
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: button_betRaise.bottom
                checkable: thisGame.buttonsCheckable
                checked: thisGame.checkedButton==Game.CallButton
                onClicked: {
                    if(thisGame.buttonsCheckable) {
                        if(thisGame.checkedButton==Game.CallButton) {
                            thisGame.checkedButton=Game.NoButton
                        }
                        else {
                            thisGame.checkedButton=Game.CallButton
                        }
                    }
                    else {
                        if(buttonContainer.calling) {
                            thisGame.call()
                        }
                        else {
                            thisGame.check()
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
                checkable: thisGame.buttonsCheckable
                checked: thisGame.checkedButton==Game.FoldButton
                onClicked: {
                    if(thisGame.buttonsCheckable) {
                        if(thisGame.checkedButton==Game.FoldButton) {
                            thisGame.checkedButton=Game.NoButton
                        }
                        else {
                            thisGame.checkedButton=Game.FoldButton
                        }
                    }
                    else {
                        thisGame.fold()
                    }
                }
                text: "Fold"
            }
            function setFromSpin(spin) {
                //var tmpSpin = roundOnStep(spin);
                var tmpSpin = spin;
                thisGame.bets = tmpSpin;
            }
            function setFromSlider(slider) {
                setFromSpin(fromSliderToSpin(slider));
            }

            function roundOnStep(value) {
                return Math.floor(value/stepSize)*stepSize
            }

            function fromSliderToSpin(slider) {
                var tmp = firstNotPossibleStep
                        + slider*(cash-firstNotPossibleStep)
                //             tmp = roundOnStep(tmp)
                if(tmp<minimumRaise)
                    return minimumRaise;
                return tmp;
            }
            function fromSpinToSlider(spin) {
                return (spin - firstNotPossibleStep)/(cash-firstNotPossibleStep)
            }
        }
    }

}
