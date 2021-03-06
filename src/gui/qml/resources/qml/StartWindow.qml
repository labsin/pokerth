import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import PokerTH 1.0 as PokerTH
import "createComponent.js" as Create

ApplicationWindow {
    id: window
    objectName: "startWindow"
    title: "PokerTH"
    width: 450
    height: 350 //+ menuBar.height
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height

    property Window dialogWindow

    menuBar: MenuBar {
        id: menuBar
//        property real height: __contentItem.height
        Menu {
            id: menuPokerTH
            title: qsTr("PokerTH")
            MenuItem {
                text: qsTr("Start Local Game")
                shortcut: "Ctrl+N"
                iconSource: "../gfx/swbuttonlocalgame.png"
                onTriggered: {
                    startNewLocalGame()
                }
            }
            MenuItem {
                text: qsTr("Start Internet Game")
                shortcut: "Ctrl+I"
                iconSource: "../gfx/swbuttoninternetgame.png"
            }
            Menu {
                id: menuNetwork_Game
                title: qsTr("Network Game")
                MenuItem {
                    text: qsTr("Create Network Game")
                    iconSource: "../gfx/swbuttoncreatenetworkgame.png"
                }
                MenuItem {
                    text: qsTr("Join Network Game")
                    iconSource: "../gfx/swbuttonjoinnetworkgame.png"
                }
            }
            MenuItem {
                text: qsTr("About")
            }
            MenuItem {
                text: qsTr("Quit")
                shortcut: "Ctrl+Q"
                iconSource: "../gfx/application_exit.png"
                onTriggered: Qt.quit()
            }
        }
        Menu {
            id: menuSettings
            title: qsTr("Settings")
            MenuItem {
                text: qsTr("Configure")
                iconSource: "../gfx/systemsettings.png"
            }
            MenuItem {
                text: qsTr("Logs")
                iconSource: "../gfx/registered.png"
            }
        }
    }

    Image {
        id: background
        anchors.fill: parent
        source: "image://pokerTH/misc/startwindowbg10_desktop.png"
    }

    Rectangle { // Shadow
        width: groupboxBackground.width
        height: groupboxBackground.height
        x: groupboxBackground.x + 7
        y: groupboxBackground.y + 7
        color: "#80000000"
    }

    Rectangle {
        id: groupboxBackground
        anchors.fill: groupbox
        anchors.margins: -15
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#d5d5d5";
            }
            GradientStop {
                position: 1.00;
                color: "#f7f7f7";
            }
        }
        border.color: "black"
        border.width: 1
        radius: 2
    }

//    DropShadow {
//        anchors.fill: groupboxBackground
//        horizontalOffset: 7
//        verticalOffset: 7
//        radius: 8.0
//        samples: 16
//        color: "#80000000"
//        source: groupboxBackground
//    }

    GroupBox {
        id: groupbox
        anchors.centerIn: background
        width: buttonColumn.width + 8
        height: buttonColumn.height + 8
        Component {
            id: buttonDelegate
            Button {
                id: thisButton
                iconSource: "../gfx/"+icon
                width: buttonColumn.biggestImplicitWidth
                text: (index+1) + " " + title
                style: ButtonStyle {
//                    background: Rectangle {
//                        implicitWidth: 100
//                        implicitHeight: 25
//                        border.width: control.activeFocus ? 2 : 1
//                        border.color: "#888"
//                        radius: 4
//                        gradient: Gradient {
//                            GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
//                            GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
//                        }
//                    }
                    label:Item {
                        implicitWidth: row.implicitWidth
                        implicitHeight: row.implicitHeight
                        Row {
                            id: row
                            anchors.left: parent.left
                            spacing: 2
                            Image {
                                source: "../gfx/"+icon
                                anchors.verticalCenter: parent.verticalCenter
                                fillMode: Image.PreserveAspectFit
                                height: text.height*1.5
                            }
                            Text {
                                id: text
                                renderType: Text.NativeRendering
                                anchors.verticalCenter: parent.verticalCenter
                                text: thisButton.text
                                color: __syspal.text
                            }
                        }
                    }
                }

                onImplicitWidthChanged: {
                    checkBiggestImplicitWidth()
                }

                function checkBiggestImplicitWidth() {
                    if(buttonColumn.biggestImplicitWidth<implicitWidth) {
                        buttonColumn.biggestImplicitWidth = implicitWidth
                    }
                }

                onClicked: {
                    print("Clicked "+title)
                    if(name) {
                        switch(name) {
                        case "srtLocGame":
                            startNewLocalGame()
                            return
                        case "srtIntGame":
                            startInternetGame()
                            return
                        }
                    }
                    if(dialog) {
                        Create.createAndShowWindow(dialog, window)
                    }
                    else {
                        print("No dialog specified")
                    }
                }
            }
        }

        ListModel {
            id: buttonModel
            ListElement {
                title: "Start Local Game"
                icon: "swbuttonlocalgame.png"
                name: "srtLocGame"
            }
            ListElement {
                title: "Internet Game"
                icon: "swbuttoninternetgame.png"
                name: "srtIntGame"
            }
            ListElement {
                title: "Create Network Game"
                icon: "swbuttoncreatenetworkgame.png"
            }
            ListElement {
                title: "Join Network Game"
                icon: "swbuttonjoinnetworkgame.png"
            }
            ListElement {
                title: "Logs"
                icon: "registered.png"
            }
        }

        Column {
            id: buttonColumn
            anchors.centerIn: parent
            spacing: 10
            property real biggestImplicitWidth: 100
            Repeater {
                delegate: buttonDelegate
                model: buttonModel
            }
        }
    }

    onVisibilityChanged: {
        if(visibility == Window.Hidden) {
            if(Create.dialogWindow)
                Create.dialogWindow.hide();
        }
    }

    function debug(string) {
        print(string)
    }

    function startNewLocalGame() {
        if(PokerTH.Config.readConfigInt("ShowGameSettingsDialogOnNewGame")===1) {
            Create.createAndShowWindow("NewGameDialog.qml", window)
        }
    }

    function startInternetGame() {
        Manager.joinGameLobby()
    }
}
