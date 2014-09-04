import QtQuick 2.1
import "createComponent.js" as Create
import PokerTH 1.0 as PokerTH

StartWindow {
    id: startWindow

    property variant gameWindow
    property variant loadingWindow
    property variant loginWindow
    property variant lobbyWindow

    visible: true

    function showStart() {
        startWindow.show()
    }
    function hideStart() {
        startWindow.hide()
    }

    function showGame() {
        if(!gameWindow) {
            gameWindow = Create.createObject("GameWindow.qml",startWindow)
            if(gameWindow)
                gameWindow.closing.connect(
                            function(){
                                Manager.stopGame()
                            })
        }
        if(gameWindow){
            gameWindow.show()
            return true
        }
        return false
    }
    function hideGame() {
        if(gameWindow)
            gameWindow.hide()
    }

    function showLobby() {
        if(!lobbyWindow) {
            lobbyWindow = Create.createObject("LobbyWindow.qml",startWindow)
            if(lobbyWindow)
                lobbyWindow.closing.connect(
                            function(){
                                showStart()
                                Manager.cancelConnect()
                            })
        }
        if(lobbyWindow) {
            lobbyWindow.show()
            return true
        }
        return false
    }
    function hideLobby() {
        if(lobbyWindow) {
            lobbyWindow.hide()
        }
    }

    function showLoading() {
        if(!loadingWindow)
            loadingWindow = Create.createObject("LoadingWindow.qml",startWindow)
        if(loadingWindow){
            loadingWindow.show()
            return true
        }
        return false
    }
    function hideLoading() {
        if(loadingWindow)
            loadingWindow.hide()
    }

    function showLogin() {
        if(!loginWindow)
            if(loadingWindow)
                loginWindow = Create.createObject("LoginWindow.qml",loadingWindow)
            else
                loginWindow = Create.createObject("LoginWindow.qml",startWindow)

        if(loginWindow){
            loginWindow.show()
            return true
        }
        return false
    }
    function hideLogin() {
        if(loginWindow)
            loginWindow.hide()
    }

    Connections {
        target: Manager.game
        onGuiInitiated: {
            print("Starting Game")
            hideLoading()
            hideLogin()
            hideLobby()
            if(showGame()){
                hideStart()
            }
        }
        onGameStopped: {
            hideGame()
            showStart()
        }
    }

    Connections {
        target: Manager.server
        onConnectedToServer: {
            print("Connected to Server")
            hideLoading()
            hideLogin()
            if(showLobby()){
                hideStart()
            }
        }
        onConnectingToServer: {
            print("Connecting to Server")
            hideLogin()
            hideLobby()
            showLoading()
        }

        onStopConnecting: {
            print("Stop Connecting")
            hideLoading()
            hideLogin()
            hideLobby()
            showStart()
        }
        onLoginNeeded: {
            print("Login Needed")
            showLogin()
        }
        onWaitDialog: {
            hideGame()
            showStart()
            if(Manager.server.connectAction == PokerTH.Server.ConnectActionLast)
                showLobby()
        }
        onNetworkNotification: {
            print("Network Notificaion")
            var errorString = ""

            switch(notificationID) {

            case PokerTH.Server.NetNotJoinIpBlocked:
                errorString = "You cannot join this game, because another player in that game has your network address."
                break;
            case PokerTH.Server.NetNotRemovedStartFailed:
                errorString = "Your connection to the server is very slow, the game had to start without you."
                break;
            case PokerTH.Server.NetNotRemovedKicked:
                errorString = "You were kicked from the game."
                break;
            case PokerTH.Server.NetNotRemovedGameFull:
            case PokerTH.Server.NetNotJoinGameFull:
                errorString = "Sorry, this game is already full."
                break;
            case PokerTH.Server.NetNotRemovedAlreadyRunning:
            case PokerTH.Server.NetNotJoinAlreadyRunning:
                errorString = "Unable to join - the server has already started the game."
                break;
            case PokerTH.Server.NetNotJoinNotInvited:
                errorString = "This game is of type invite-only. You cannot join this game without being invited."
                break;
            case PokerTH.Server.NetNotJoinGameNameInUse:
            case PokerTH.Server.NetNotJoinGameBadName:
                errorString = "TODO"
                break;
            case PokerTH.Server.NetNotRemovedTimeout:
                errorString = "Your admin state timed out due to inactivity. Feel free to create a new game!"
                break;
            case PokerTH.Server.NetNotJoinInvalidPassword:
                errorString = "Invalid password when joining the game.\nPlease reenter the password and try again."
                break;
            case PokerTH.Server.NetNotJoinGuestForbidden:
                errorString = "You cannot join this type of game as guest."
                break;
            case PokerTH.Server.NetNotJoinInvalidSettings:
                errorString = "The settings are invalid for this type of game."
                break;
            case PokerTH.Server.NetNotNewReleaseAvailable:
                errorString = "A new release of PokerTH is available.<br>Please go to <a href=\"http://www.pokerth.net/\" target=\"_blank\">http://www.pokerth.net</a> and download the latest version."
                break;
            case PokerTH.Server.NetNotOutdatedBeta:
                errorString = "This beta release of PokerTH is outdated.<br>Please go to <a href=\"http://www.pokerth.net/\" target=\"_blank\">http://www.pokerth.net</a> and download the latest version."
            }

            print(errorString)
        }
        onNetworkMessage: {
            print("Network Message")
            var errorString = ""

            if(msgID == -1) {
                errorString = msg
            }
            else {
                switch(msgID) {

                case PokerTH.Server.NetMsgAvatarAccepted:
                    errorString = "The avatar report was accepted by the server. Thank you."
                    break;
                case PokerTH.Server.NetMsgAvatarDup:
                    errorString = "This avatar was already reported by another player."
                    break;
                case PokerTH.Server.NetMsgAvatarRejected:
                    errorString = "An error occurred while reporting the avatar."
                    break;
                case PokerTH.Server.NetMsgGameNameAccepted:
                    errorString = "The game name report was accepted by the server. Thank you."
                    break;
                case PokerTH.Server.NetMsgGameNameDup:
                    errorString = "This game name was already reported by another player."
                    break;
                case PokerTH.Server.NetMsgGameNameRejected:
                    errorString = "An error occurred while reporting the game name."
                    break;
                case PokerTH.Server.NetMsgAdminRemoveGameAccepted:
                    errorString = "The game was closed."
                    break;
                case PokerTH.Server.NetMsgAdminRemoveGameRejected:
                    errorString = "The game could not be closed."
                    break;
                case PokerTH.Server.NetMsgAdminBanPlayerAccepted:
                    errorString = "The player was kicked and banned permanently."
                    break;
                case PokerTH.Server.NetMsgAdminBanPlayerNoDB:
                    errorString = "The player was kicked, but could not be banned because it was a guest player."
                    break;
                case PokerTH.Server.NetMsgAdminBanPlayerDBError:
                    errorString = "The player was kicked, but could not be banned, \nbecause the nick could not be found in the database"
                    break;
                case PokerTH.Server.NetMsgAdminBanPlayerRejected:
                    errorString = "The player could not be found."
                    break;
                }
            }

            print(errorString)
        }
        onNetworkError: {
            print("Network Error")
            var errorString = ""

            switch (errorID) {
            case PokerTH.Server.ConnErrServerAddrNotSet:

                errorString = "Server address was not set."


                break
            case PokerTH.Server.ConnErrInvalidPort:

                errorString = "An invalid port was set (ports 0-1023 are not allowed)."


                break
            case PokerTH.Server.ConnErrCreationFailed:

                errorString = "Could not create a socket for TCP communication."


                break
            case PokerTH.Server.ConnErrSetAddrFailed:

                errorString = "Could not set the IP address."


                break
            case PokerTH.Server.ConnErrSetPortFailed:

                errorString = "Could not set the port for this type of address."


                break
            case PokerTH.Server.ConnErrResolveFailed:

                errorString = "The server name could not be resolved."


                break
            case PokerTH.Server.ConnErrBindFailed:

                errorString = "Bind failed - please choose a different port."


                break
            case PokerTH.Server.ConnErrListenFailed:

                errorString = "Internal network error: \"listen\" failed."


                break
            case PokerTH.Server.ConnErrAcceptFailed:

                errorString = "Server execution was terminated."


                break
            case PokerTH.Server.ConnErrConnectFailed:

                errorString = "Could not connect to the server."


                break
            case PokerTH.Server.ConnErrIPv6Failed:

                errorString = "Count not connect to the server.\n\nPlease note: IPv6 is enabled in the settings. The connection fails if your provider does not support IPv6.\nThis may be fixed by unchecking the \"Use IPv6\" checkbox in the settings."


                break
            case PokerTH.Server.ConnErrConnectTimeout:

                errorString = "Connection timed out.\nPlease check the server address.\n\nIf the server is behind a NAT-Router, make sure port forwarding has been set up on server side."


                break
            case PokerTH.Server.ConnErrIPv6Failed:

                errorString = "Connection timed out.\nPlease check the server address.\n\nPlease note: IPv6 is enabled in the settings. The connection fails if your provider does not support IPv6.\nThis may be fixed by unchecking the \"Use IPv6\" checkbox in the settings."


                break
            case PokerTH.Server.ConnErrSelectFailed:

                errorString = "Internal network error: \"select\" failed."


                break
            case PokerTH.Server.ConnErrSendFailed:

                errorString = "Internal network error: \"send\" failed."


                break
            case PokerTH.Server.ConnErrRecvFailed: // Sometimes windows reports recv failed on close.
            case PokerTH.Server.ConnErrConnReset:

                errorString = "The connection to the server was lost."


                break
            case PokerTH.Server.ConnErrConnExists:

                errorString = "Internal network error: Duplicate TCP connection."


                break
            case PokerTH.Server.ConnErrInvalidPacket:

                errorString = "An invalid network packet was received.\nPlease make sure that all players use the same version of PokerTH."


                break
            case PokerTH.Server.ConnErrInvalidState:

                errorString = "Internal state error.\nPlease make sure that all players use the same version of PokerTH."


                break
            case PokerTH.Server.ConnErrInvalidServerListURL:
            case PokerTH.Server.ConnErrTransferInvalidURL:

                errorString = "Invalid server list URL.\nPlease correct the address in the settings."


                break
            case PokerTH.Server.ConnErrInvalidServerListXML:

                errorString = "The PokerTH internet server list contains invalid data.\nIf you use a custom server list, please make sure its format is correct."


                break
            case PokerTH.Server.ConnErrUnzipFailed:

                errorString = "Could not unzip the PokerTH internet server list."


                break
            case PokerTH.Server.ConnErrTransferInitFailed:
            case PokerTH.Server.ConnErrTransferSelectFailed:
            case PokerTH.Server.ConnErrTransferFailed:

                errorString = "Could not download the PokerTH internet server list.\nPlease make sure you are directly connected to the internet."


                break
            case PokerTH.Server.ConnErrTransferOpenFailed:

                errorString = "Could not open the target file when downloading the server list."


                break
            case PokerTH.Server.NetErrVersionNotSupported:
                errorString = "The PokerTH server does not support this version of the game.<br>Please go to <a href=\"http://www.pokerth.net/\" target=\"_blank\">http://www.pokerth.net</a> and download the latest version."

                break
            case PokerTH.Server.NetErrServerFull:

                errorString = "Sorry, this server is already full."


                break
            case PokerTH.Server.NetErrInvalidPassword:

                errorString = "Invalid login.\nPlease check your username and password."


                break
            case PokerTH.Server.NetErrInvalidPasswordStr:

                errorString = "The password is too long. Please choose another one."


                break
            case PokerTH.Server.NetErrPlayerNameInUse:

                errorString = "The NickName is already in use. Please choose another one."


                break
            case PokerTH.Server.NetErrInvalidPlayerName:

                errorString = "The NickName is invalid. Please choose another one."


                break
            case PokerTH.Server.NetErrInvalidGameName:

                errorString = "The game name is either too short or too long. Please choose another one."


                break
            case PokerTH.Server.NetErrUnknownGame:

                errorString = "The game could not be found."


                break
            case PokerTH.Server.NetErrInvalidChatText:

                errorString = "The chat text is invalid."


                break
            case PokerTH.Server.NetErrUnknownPlayerId:

                errorString = "The server referred to an unknown player. Aborting."


                break
            case PokerTH.Server.NetErrNoCurrentPlayer:

                errorString = "Internal error: The current player could not be found."


                break
            case PokerTH.Server.NetErrPlayerNotActive:

                errorString = "Internal error: The current player is not active."


                break
            case PokerTH.Server.NetErrPlayerKicked:

                errorString = "You were kicked from the server."


                break
            case PokerTH.Server.NetErrPlayerBanned:

                errorString = "You were temporarily banned from the server."


                break
            case PokerTH.Server.NetErrPlayerBlocked:

                errorString = "Your account is blocked indefinitely."


                break
            case PokerTH.Server.NetErrSessionTimedOut:

                errorString = "Your server connection timed out due to inactivity. You are very welcome to reconnect!"


                break
            case PokerTH.Server.NetErrInvalidPlayerCount:

                errorString = "The client player count is invalid."


                break
            case PokerTH.Server.NetErrTooManyManualBlinds:

                errorString = "Too many manual blinds were set. Please reconfigure the manual blinds."


                break
            case PokerTH.Server.NetErrInvalidAvatarFile:
            case PokerTH.Server.NetErrWrongAvatarSize:

                errorString = "An invalid avatar file was configured. Please choose a different avatar."


                break
            case PokerTH.Server.NetErrAvatarTooLarge:

                errorString = "The selected avatar file is too large. Please choose a different avatar."


                break
            case PokerTH.Server.NetErrInitBlocked:

                errorString = "You cannot login at this time. Please try again in a few seconds."


                break
            case PokerTH.Server.NetErrInvalidRequestId:

                errorString = "An internal avatar error occured. Please report this to an admin in the lobby chat."


                break
            case PokerTH.Server.NetErrStartTimeout:

                errorString = "Could not start game: Synchronization failed."


                break
            case PokerTH.Server.NetErrServerMaintenance:

                errorString = "The server is down for maintenance. Please try again later."


                break
            default:

                errorString = "An internal error occured."


            }
            if(loadingWindow) {
                loadingWindow.error = errorString;
            }

            print(errorString)
        }
    }
}
