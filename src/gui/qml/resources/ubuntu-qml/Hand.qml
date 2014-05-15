import QtQuick 2.0
import PokerTH 1.0 as PokerTH

Item {
    property variant cards
    implicitWidth: card1.implicitWidth*1.5
    implicitHeight: card1.implicitHeight
    Card {
        id: card1
        cardObj: cards[0]
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: cards[0].dealt
    }
    Card {
        id: card2
        cardObj: cards[1]
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: cards[0].dealt
    }
    Component.onCompleted: {
        console.assert(cards, "Hand needs cards property set");
    }
}
