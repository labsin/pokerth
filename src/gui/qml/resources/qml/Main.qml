import QtQuick 2.1

Item {
    id: main
    property variant gameWindow
    StartWindow {
        id: startWindow
        visible: true
    }

    Connections {
        target: CurrentGame
        onGuiInitiated: {
            var component = Qt.createComponent("GameWindow.qml");
            if (component.status == Component.Ready) {
                gameWindow = component.createObject(main, {"thisGame": CurrentGame});
                if (gameWindow == null) {
                    // Error Handling
                    console.log("Error creating object");
                }
                else {
                    gameWindow.show()
                    startWindow.close();
                }
            }
        }
    }
}
