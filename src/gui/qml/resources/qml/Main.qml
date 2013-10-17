import QtQuick 2.1

Item {
    StartWindow {
        id: startWindow
        visible: true
    }
    GameWindow {
        id: gameWindow
    }
    Connections {
        target: CurrentGame
        onGuiInitiated: {
            startWindow.close();
            gameWindow.show()
        }
    }
}
