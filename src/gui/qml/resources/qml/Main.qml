import QtQuick 2.1
import "createComponent.js" as Create
import PokerTH 1.0 as PokerTH

Item {
    id: main
    property variant gameWindow
    property variant loadingWindow
    property variant loginWindow
    StartWindow {
        id: startWindow
        visible: true
    }

    Connections {
        target: CurrentGame
        onGuiInitiated: {
            print("Starting Game")
            gameWindow = Create.createObject("GameWindow.qml",startWindow)
            if(gameWindow) {
                gameWindow.show()
                startWindow.hide();
            }
        }
    }

    Connections {
        target: CurrentServer
        onConnectingToServer: {
            print("Connecting to Server")
            if(!loadingWindow)
                loadingWindow = Create.createObject("LoadingWindow.qml",startWindow)
            if(loginWindow)
                loginWindow.hide();
            loadingWindow.show()
        }
        onStopConnecting: {
            print("Stop Connecting")
            if(loadingWindow) {
                loadingWindow.hide();
            }
            if(loginWindow)
                loginWindow.hide();
            startWindow.show();
        }
        onLoginNeeded: {
            print("Login Needed")
            if(!loginWindow)
                if(loadingWindow)
                    loginWindow = Create.createObject("LoginWindow.qml",loadingWindow)
                else
                    loginWindow = Create.createObject("LoginWindow.qml",startWindow)

            loginWindow.show()
        }
        onServerError: {
            print("Connect Error")
            var errorString;

            switch (error) {
            case PokerTH.Server.ConnErrServerAddrNotSet:

                errorString = "Server address was not set."


                break;
            case PokerTH.Server.ConnErrInvalidPort:

                errorString = "An invalid port was set (ports 0-1023 are not allowed)."


                break;
            case PokerTH.Server.ConnErrCreationFailed:

                errorString = "Could not create a socket for TCP communication."


                break;
            case PokerTH.Server.ConnErrSetAddrFailed:

                errorString = "Could not set the IP address."


                break;
            case PokerTH.Server.ConnErrSetPortFailed:

                errorString = "Could not set the port for this type of address."


                break;
            case PokerTH.Server.ConnErrResolveFailed:

                errorString = "The server name could not be resolved."


                break;
            case PokerTH.Server.ConnErrBindFailed:

                errorString = "Bind failed - please choose a different port."


                break;
            case PokerTH.Server.ConnErrListenFailed:

                errorString = "Internal network error: \"listen\" failed."


                break;
            case PokerTH.Server.ConnErrAcceptFailed:

                errorString = "Server execution was terminated."


                break;
            case PokerTH.Server.ConnErrConnectFailed:

                errorString = "Could not connect to the server."


                break;
            case PokerTH.Server.ConnErrConnectTimeout:

                errorString = "Connection timed out.\nPlease check the server address.\n\nIf the server is behind a NAT-Router, make sure port forwarding has been set up on server side."


                break;
            case PokerTH.Server.ConnErrSelectFailed:

                errorString = "Internal network error: \"select\" failed."


                break;
            case PokerTH.Server.ConnErrSendFailed:

                errorString = "Internal network error: \"send\" failed."


                break;
            case PokerTH.Server.ConnErrRecvFailed: // Sometimes windows reports recv failed on close.
            case PokerTH.Server.ConnErrConnReset:

                errorString = "The connection to the server was lost."


                break;
            case PokerTH.Server.ConnErrConnExists:

                errorString = "Internal network error: Duplicate TCP connection."


                break;
            case PokerTH.Server.ConnErrInvalidPacket:

                errorString = "An invalid network packet was received.\nPlease make sure that all players use the same version of PokerTH."


                break;
            case PokerTH.Server.ConnErrInvalidState:

                errorString = "Internal state error.\nPlease make sure that all players use the same version of PokerTH."


                break;
            case PokerTH.Server.ConnErrInvalidServerListURL:
            case PokerTH.Server.ConnErrTransferInvalidURL:

                errorString = "Invalid server list URL.\nPlease correct the address in the settings."


                break;
            case PokerTH.Server.ConnErrInvalidServerListXML:

                errorString = "The PokerTH internet server list contains invalid data.\nIf you use a custom server list, please make sure its format is correct."


                break;
            case PokerTH.Server.ConnErrUnzipFailed:

                errorString = "Could not unzip the PokerTH internet server list."


                break;
            case PokerTH.Server.ConnErrTransferInitFailed:
            case PokerTH.Server.ConnErrTransferSelectFailed:
            case PokerTH.Server.ConnErrTransferFailed:

                errorString = "Could not download the PokerTH internet server list.\nPlease make sure you are directly connected to the internet."


                break;
            case PokerTH.Server.ConnErrTransferOpenFailed:

                errorString = "Could not open the target file when downloading the server list."


                break;
            case PokerTH.Server.NetErrVersionNotSupported:
                errorString = "The PokerTH server does not support this version of the game.<br>Please go to <a href=\"http://www.pokerth.net/\" target=\"_blank\">http://www.pokerth.net</a> and download the latest version."

                break;
            case PokerTH.Server.NetErrServerFull:

                errorString = "Sorry, this server is already full."


                break;
            case PokerTH.Server.NetErrInvalidPassword:

                errorString = "Invalid login.\nPlease check your username and password."


                break;
            case PokerTH.Server.NetErrInvalidPasswordStr:

                errorString = "The password is too long. Please choose another one."


                break;
            case PokerTH.Server.NetErrPlayerNameInUse:

                errorString = "The NickName is already in use. Please choose another one."


                break;
            case PokerTH.Server.NetErrInvalidPlayerName:

                errorString = "The NickName is invalid. Please choose another one."


                break;
            case PokerTH.Server.NetErrInvalidGameName:

                errorString = "The game name is either too short or too long. Please choose another one."


                break;
            case PokerTH.Server.NetErrUnknownGame:

                errorString = "The game could not be found."


                break;
            case PokerTH.Server.NetErrInvalidChatText:

                errorString = "The chat text is invalid."


                break;
            case PokerTH.Server.NetErrUnknownPlayerId:

                errorString = "The server referred to an unknown player. Aborting."


                break;
            case PokerTH.Server.NetErrNoCurrentPlayer:

                errorString = "Internal error: The current player could not be found."


                break;
            case PokerTH.Server.NetErrPlayerNotActive:

                errorString = "Internal error: The current player is not active."


                break;
            case PokerTH.Server.NetErrPlayerKicked:

                errorString = "You were kicked from the server."


                break;
            case PokerTH.Server.NetErrPlayerBanned:

                errorString = "You were temporarily banned from the server."


                break;
            case PokerTH.Server.NetErrPlayerBlocked:

                errorString = "Your account is blocked indefinitely."


                break;
            case PokerTH.Server.NetErrSessionTimedOut:

                errorString = "Your server connection timed out due to inactivity. You are very welcome to reconnect!"


                break;
            case PokerTH.Server.NetErrInvalidPlayerCount:

                errorString = "The client player count is invalid."


                break;
            case PokerTH.Server.NetErrTooManyManualBlinds:

                errorString = "Too many manual blinds were set. Please reconfigure the manual blinds."


                break;
            case PokerTH.Server.NetErrInvalidAvatarFile:
            case PokerTH.Server.NetErrWrongAvatarSize:

                errorString = "An invalid avatar file was configured. Please choose a different avatar."


                break;
            case PokerTH.Server.NetErrAvatarTooLarge:

                errorString = "The selected avatar file is too large. Please choose a different avatar."


                break;
            case PokerTH.Server.NetErrInitBlocked:

                errorString = "You cannot login at this time. Please try again in a few seconds."


                break;
            case PokerTH.Server.NetErrInvalidRequestId:

                errorString = "An internal avatar error occured. Please report this to an admin in the lobby chat."


                break;
            case PokerTH.Server.NetErrStartTimeout:

                errorString = "Could not start game: Synchronization failed."


                break;
            case PokerTH.Server.NetErrServerMaintenance:

                errorString = "The server is down for maintenance. Please try again later."


                break;
            default:

                errorString = "An internal error occured."


            }

            print(errorString)
        }
    }
}
