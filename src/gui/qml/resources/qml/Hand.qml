import QtQuick 2.0
import PokerTH 1.0

Item {
    property list<Card> cards: [Card {} , Card {}]
    implicitWidth: card1.implicitWidth*1.5
    implicitHeight: card1.implicitHeight
    CardObj {
        id: card1
        cardObj: cards[0]
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: cards[0].dealt
    }
    CardObj {
        id: card2
        cardObj: cards[1]
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: cards[0].dealt
    }
}
