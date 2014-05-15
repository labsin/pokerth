import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import PokerTH 1.0 as PokerTH

Window {
    id: window
    title: "Loading"
    width: 180
    height: 100

    modality: Qt.WindowModal

    onClosing: {
        if(CurrentServer.ConnectAction !== PokerTH.Server.ConnectSessionDone)
            CurrentServer.stopConnecting()
    }
    Item {
        anchors.fill: parent
        anchors.margins: 10
        Label {
            id: label_progress
            height: implicitHeight
            text: getLabelForConnectAction(CurrentServer.connectAction)
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 3
        }

        ProgressBar {
            value: CurrentServer.connectProgress
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: button_cancel.top
            anchors.bottomMargin: 5
        }
        Button {
            id: button_cancel
            text: "Cancel"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            onClicked: {
                Manager.cancelConnect();
            }
        }
    }

    function getLabelForConnectAction(connectAction) {
        switch(connectAction) {
        case PokerTH.Server.ConnectActionNone:
            return "Initializing..."
        case PokerTH.Server.ConnectInitDone:
            return "RetreivingServerList"
        case PokerTH.Server.ConnectServerListDone:
            return "Resolving"
        case PokerTH.Server.ConnectResolveDone:
            return "Connecting"
        case PokerTH.Server.ConnectConnectDone:
            return "Connected"
        case PokerTH.Server.ConnectSessionDone:
            return "Done"
        default:
            return ""
        }
    }

}
