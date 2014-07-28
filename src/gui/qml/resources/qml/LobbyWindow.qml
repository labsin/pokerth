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
    PokerTH.PlayerSortModel {
        id: psm
        sortRole: sourceModel.role("country")
        sourceModel: Manager.server.lobbyPlayers
    }
    PokerTH.PlayerSortModel {
        id: psm2
        sourceModel: Manager.server.lobbyPlayers
        hideActive: true
        wildcardExpression: "labsin2"
    }

    ScrollView {
        id: sv1
        anchors {left: parent.left;
            top: parent.top; bottom: parent.bottom}
        width: parent.width/2
        ListView {
            id: lv
            width: parent.width
            model: psm
            delegate: Text {
//                property GameInfo gameInfo: info
                text: name+" country:"+country + " Idle:"+idle
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
        anchors {left: sv1.right; right: parent.right;
            top: parent.top; bottom: parent.bottom}
        ListView {
            id: lv2
            width: parent.width
            model: psm2
            delegate: Text {
//                property GameInfo gameInfo: info
                text: name+" country:"+country
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                    }
                }
            }
        }
    }
}
