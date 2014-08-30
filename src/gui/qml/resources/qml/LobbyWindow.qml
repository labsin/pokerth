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
    PokerTH.PlayerSortModel {
        id: players
        sourceModel: Manager.server.lobbyPlayers
        sortOrder: playersTable.sortIndicatorOrder
        sortRole: PokerTH.Enums.NickNameRole
        dynamicSortFilter: true
        onSortOrderChanged: {
            invalidate()
            print("sortOrder:"+sortOrder)
        }
    }

    PokerTH.GameSortModel {
        id: games
        sourceModel: Manager.server.games
        sortOrder: gamesTable.sortIndicatorOrder
        sortRole: PokerTH.Enums.GameNameRole
        dynamicSortFilter: true
        onSortOrderChanged: {
            invalidate()
            print("sortOrder:"+sortOrder)
        }
    }
    PokerTH.ChatSortModel {
        id: chat
        sourceModel: Manager.server.chat
    }
    SplitView {
        orientation: Qt.Horizontal
        anchors.fill: parent

        TableView {
            id: playersTable
            width: 150
            frameVisible: true
            sortIndicatorVisible: true
            model: players
            TableViewColumn {
                role: "name"
                title: "Name"
            }
            TableViewColumn {
                role: "country"
                title: "C"
            }
            TableViewColumn {
                role: "isGuest"
                title: "G"
            }
            onSortIndicatorColumnChanged: {
                switch(sortIndicatorColumn) {
                case 0:
                    players.sortRole = PokerTH.Enums.NickNameRole
                    break
                case 1:
                    players.sortRole = PokerTH.Enums.NickCountryRole
                    break
                case 2:
                    players.sortRole = PokerTH.Enums.NickIsGuestRole
                    break
                default:
                    players.sortRole = PokerTH.Enums.NickNameRole
                    break
                }
                players.invalidate()
            }
        }


        SplitView {
            id: sv
            orientation: Qt.Vertical
            Layout.fillWidth: true
            TableView {
                id: gamesTable
                Layout.fillHeight: true
                frameVisible: true
                sortIndicatorVisible: true
                model: games
                TableViewColumn {
                    role: "name"
                    title: "Name"
                }
                TableViewColumn {
                    role: "playersString"
                    title: "Players"
                }
                TableViewColumn {
                    role: "gameStatus"
                    title: "Status"
                }
                TableViewColumn {
                    role: "gameType"
                    title: "T"
                }
                TableViewColumn {
                    role: "pwProtect"
                    title: "P"
                }
                TableViewColumn {
                    role: "timeout"
                    title: "Time"
                }
                onSortIndicatorColumnChanged: {
                    switch(sortIndicatorColumn) {
                    case 0:
                        games.sortRole = PokerTH.Enums.GameNameRole
                        break
                    case 1:
                        games.sortRole = PokerTH.Enums.GamePlayersStringRole
                        break
                    case 2:
                        games.sortRole = PokerTH.Enums.GameStatusRole
                        break
                    case 3:
                        games.sortRole = PokerTH.Enums.GameTypeRole
                        break
                    case 4:
                        games.sortRole = PokerTH.Enums.GamePwProtectRole
                        break
                    case 5:
                        games.sortRole = PokerTH.Enums.GameTimeoutRole
                        break
                    default:
                        games.sortRole = PokerTH.Enums.GameNameRole
                        break
                    }
                    games.invalidate()
                }
            }
            ColumnLayout {
                id: cl
                height: window.height / 2
                Text {
                    id: chatLabel
                    text: "Chat"
                }
                ScrollView {
                    id: sv2
                    width: sv.width
                    Layout.fillHeight: true
                    frameVisible: true
                    ListView {
                        id: lv2
                        width: parent.width
                        model: chat
                        delegate: Text {
                            text: playerName+" message: " + message + " fromLobby:"+fromLobby+" fromGame:"+fromGame
                        }
                        onContentHeightChanged: {

                        }
                    }
                }
                Item {
                    id: ti
                    width: sv.width
                    height: 40
                    TextInput {
                        id: p_ti
                        anchors {top: parent.top; left: parent.left; bottom: parent.bottom; right: p_button.left}
                    }
                    Button {
                        id: p_button
                        anchors {top: parent.top; right: parent.right; bottom: parent.bottom}
                        width: 50
                        text: "send"
                        onClicked: {
                            Manager.server.sendMessage(p_ti.text)
                        }
                    }
                }
            }
        }
    }
}
