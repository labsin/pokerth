import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import PokerTH 1.0 as PokerTH
import PokerTH.GameData 1.0

Window {
    id: window
    title: "Lobby"
    width: 800
    height: 600
    Component.onCompleted: {
    }

    modality: Qt.WindowModal
    PokerTH.GameSortModel {
        id: psm
        sourceModel: Manager.server.games
        sortRole: PokerTH.Enums.GameSortStatus
    }
    PokerTH.ChatSortModel {
        id: psm2
        sourceModel: Manager.server.chat
    }

    ScrollView {
        id: sv1
        anchors {left: parent.left;
            top: parent.top; bottom: parent.bottom}
        width: parent.width/2
        frameVisible: true
        ListView {
            id: lv
            width: parent.width
            model: psm
            delegate: Text {
                property GameInfo gameInfo: info
                text: name+" id: " + gameId + " mode:"+gameInfo.mode+" gameType:"+gameInfo.data.gameType
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                    }
                }
            }
        }
    }
    ScrollView {
        id: sv2
        anchors {right: parent.right;
            top: parent.top; bottom: parent.bottom}
        width: parent.width/2
        frameVisible: true
        ListView {
            id: lv2
            width: parent.width
            model: psm2
            delegate: Text {
                text: playerName+" message: " + message + " fromLobby:"+fromLobby+" fromGame:"+fromGame
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        print(message)
//                    }
//                }
            }
        }
    }
}
