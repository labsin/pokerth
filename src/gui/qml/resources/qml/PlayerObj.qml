import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import PokerTH 1.0

Item {
    property bool alignBottom: false
//    property bool highlight: true
    property Player player: Player { }
    visible: player.activeStatus

    width: rowlayout.width+12
    height: rowlayout.height+12

    Item {
        id: backgroundContainer
        anchors.fill: parent
        anchors.margins: -30
        Rectangle {
            id: background
            anchors.fill: backgroundContainer
            anchors.margins: -backgroundContainer.anchors.margins
            color: "green"
            radius: 7
        }
    }

    //TODO: look at RectangularGlow
    Glow {
        id: glow
        anchors.fill: backgroundContainer
        visible: player.thisTurn
        radius: 5
        spread: 0.8
        transparentBorder: false
        fast: true
        color: "white"
        source: backgroundContainer
    }

    RowLayout {
        id: rowlayout
        spacing: 7
        anchors.centerIn: parent
        ColumnLayout {
            id: leftColumn
            spacing: 4
            Image {
                id: image_Avatar
                source: player.avatar==""?"image://PokerTH/table/genereticAvatar.png":player.avatar
                Layout.alignment: Qt.AlignLeft | (alignBottom?Qt.AlignBottom:Qt.AlignTop)
                Layout.row: alignBottom?8:0
                width: 50
                height: 50
            }
            Stars {
                id: stars
                stars: player.stars
                Layout.alignment: Qt.AlignLeft | (alignBottom?Qt.AlignBottom:Qt.AlignTop)
                Layout.row: alignBottom?7:1
            }
            Label {
                id: label_Timeout
                Layout.alignment: Qt.AlignLeft | (alignBottom?Qt.AlignBottom:Qt.AlignTop)
                Layout.row: alignBottom?6:2
            }
            Item {
                id: item_spacer
                Layout.row: alignBottom?5:3
                Layout.alignment: Qt.AlignLeft | (alignBottom?Qt.AlignTop:Qt.AlignBottom)
                Layout.fillHeight: true
            }

            Image {
                id: image_Button
                source: getLabelForButton(player.button)
                Layout.alignment: Qt.AlignLeft | (alignBottom?Qt.AlignTop:Qt.AlignBottom)
                Layout.row: 4
            }
        }
        GridLayout {
            id: rightColumn
            rowSpacing: 4
            columns:1
            Label {
                id: label_PlayerName
                text: player.name
                font.bold: true
                Layout.alignment: verticalAlignment | effectiveHorizontalAlignment
                Layout.row: alignBottom?7:0
                verticalAlignment: alignBottom?Qt.AlignBottom:Qt.AlignTop
            }
            Label {
                id: textLabel_Cash
                text: "$"+qsTr("%L1").arg(player.cash)
                color: "goldenrod"
                Layout.alignment: verticalAlignment | effectiveHorizontalAlignment
                Layout.row: alignBottom?6:1
                verticalAlignment: alignBottom?Qt.AlignBottom:Qt.AlignTop
            }
            Hand {
                id: item_cards
                Layout.alignment: Qt.AlignHCenter | (alignBottom?Qt.AlignBottom:Qt.AlignTop)
                Layout.row: alignBottom?5:2
                cards: player.cards
                Label {
                    id: textLabel_Status
                    text: getLabelForAction(player.action)
                    color: "white"
                    font.bold: true
                    style: Text.Outline; styleColor: "black"
                    anchors.bottom: item_cards.bottom
                    anchors.horizontalCenter: item_cards.horizontalCenter
                    anchors.bottomMargin: 14
                }
            }
            Label {
                id: textLabel_Set
                text: player.set>0?"$"+player.set:""
                color: "red"
                Layout.alignment: Qt.AlignHCenter | (alignBottom?Qt.AlignTop:Qt.AlignBottom)
                Layout.row: alignBottom?4:3
            }
        }
    }
    function getLabelForAction(action) {
        switch(action) {
        case Player.FoldAction:
            return "Folded"
        case Player.CheckAction:
            return "Checked"
        case Player.CallAction:
            return "Called"
        case Player.BetAction:
            return "Bet"
        case Player.RaiseAction:
            return "Raised"
        case Player.AllInAction:
            return "All-In"
        case Player.WonAction:
            return "Winner"
        case Player.NoAction:
        default:
            return ""
        }
    }
    function getLabelForButton(button) {
        switch(button) {
        case Player.NoButton:
            return ""
        case Player.DealerButton:
            return "image://PokerTH/table/dealerPuck.png"
        case Player.SmallBlindButton:
            return "image://PokerTH/table/smallblindPuck.png"
        case Player.BigBlindButton:
            return "image://PokerTH/table/bigblindPuck.png"
        default:
            return ""
        }
    }
}
