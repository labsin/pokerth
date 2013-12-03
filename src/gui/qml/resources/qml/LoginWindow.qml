import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import PokerTH 1.0 as PokerTH

Window {
    id: window
    title: "Login"
    width: 250
    height: 250

    modality: Qt.WindowModal

    onClosing: {
        if(CurrentServer.ConnectAction !== PokerTH.Server.ConnectSessionDone)
            CurrentServer.stopConnecting()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        GroupBox {
            id: groubBox_registeredUser
            title: "Registered user"
            checkable: true
            Layout.fillWidth: true

            onCheckedChanged: {
                if(checked)
                    checkBox_guest.checked = false
                else {
                    checkBox_guest.checked = true
                }
            }

            GridLayout {
                columns: 2
                columnSpacing: 5
                rowSpacing: 5

                Label {
                    text: "Username"
                }
                TextField {
                    id: textField_username
                    Layout.fillWidth: true
                }

                Label {
                    text: "Password"
                }
                TextField {
                    id: textField_password
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                }

                CheckBox {
                    id: checkBox_rememberPW
                    text: "Remember password (not recomended)"
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                }
            }
        }

        CheckBox {
            id: checkBox_guest
            text: "Login as guest"
            onCheckedChanged: {
                if(checked)
                    groubBox_registeredUser.checked = false
                else {
                    groubBox_registeredUser.checked = true
                }
            }
        }
        Row {
            Layout.alignment: Qt.AlignRight
            Button {
                id: button_login
                text: "Login"
                onClicked: {
                    CurrentServer.setLoginData(groubBox_registeredUser.checked, textField_username.text, textField_password.text, checkBox_rememberPW.checked)
                    window.hide()
                }
            }
            Button {
                id: button_cancel
                text: "Cancel"
                onClicked: {
                    CurrentServer.stopConnecting()
                }
            }
        }
        Label {
            id: label_createAccount
            textFormat: Text.RichText
            text: "<a href='http://create-gaming-account.pokerth.net'>Create new user account</a>."
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
