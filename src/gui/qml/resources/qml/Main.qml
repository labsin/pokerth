import QtQuick 2.1
import "createComponent.js" as Create

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
            startWindow.show();
        }
    }
}
