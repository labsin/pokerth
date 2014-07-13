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

    modality: Qt.WindowModal

    ScrollView {
        anchors.fill: parent
        ListView {
            id: lv
            width: parent.width
            model: Manager.server.games
            delegate: Text {
                property GameInfo gameInfo: info
                text: name+" players:"+players.rowCount+"/"+info.data.maxNumberOfPlayers+" "+(gameInfo.mode==GameInfo.GAME_MODE_CREATED?"open":"closed")+" pwprotected "+gameInfo.isPasswordProtected
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Manager.joinGame(id)
                    }
                }
            }
        }
    }
}
