#include "qmlserver.h"
#include <net/socket_msg.h>
#include <QTimer>
#include <QtQml>
#include "manager.h"
#include "session.h"
#include "configfile.h"
#include "tools.h"

QmlServer::QmlServer(QObject *parent) :
    QObject(parent), m_connectAction(ConnectActionNone)
{
    connect(this,SIGNAL(serverError(int)),this,SLOT(processError(int)));
}

void QmlServer::setconnectAction(int arg)
{
    if(arg>static_cast<int>(ConnectActionLast) || arg<static_cast<int>(ConnectActionFirst))
        return;
    if (m_connectAction != arg) {
        m_connectAction = arg;
        emit connectActionChanged(arg);
        setconnectProgress(float(arg-static_cast<int>(ConnectActionFirst))/static_cast<int>(ConnectActionLast));

        if(arg==static_cast<int>(ConnectActionLast)) {
            QTimer::singleShot(1000,this,SLOT(connectedToServerTimeout()));
        }
    }
}

void QmlServer::registerType()
{
    qmlRegisterType<QmlServer>("PokerTH",1, 0, "Server");
    qRegisterMetaType<QmlServer::ConnectAction>("ConnectAction");
    qRegisterMetaType<QmlServer::ServerError>("ServerError");
}

void QmlServer::setLoginData(bool regUser, QString userName, QString password, bool rememberPassword)
{
    if(regUser) {
        ManagerSingleton::Instance()->getConfig()->writeConfigInt("InternetLoginMode", 0);
        ManagerSingleton::Instance()->getConfig()->writeConfigString("MyName", userName.toUtf8().constData());
        if(rememberPassword) {
            ManagerSingleton::Instance()->getConfig()->writeConfigInt("InternetSavePassword", 1);
            ManagerSingleton::Instance()->getConfig()->writeConfigString("InternetLoginPassword", password.toUtf8().toBase64().constData());
        } else {
            ManagerSingleton::Instance()->getConfig()->writeConfigInt("InternetSavePassword", 0);
        }

    } else {
        ManagerSingleton::Instance()->getConfig()->writeConfigInt("InternetLoginMode", 1);
        // Generate a valid guest name.
        QString guestName;
        int guestId;
        Tools::GetRand(1, 99999, 1, &guestId);
        guestName.sprintf("Guest%05d", guestId);
        ManagerSingleton::Instance()->getConfig()->writeConfigString("MyName", guestName.toUtf8().constData());
    }

    ManagerSingleton::Instance()->getConfig()->writeBuffer();

    ManagerSingleton::Instance()->getSession()->setLogin(
                ManagerSingleton::Instance()->getConfig()->readConfigString("MyName"),
                password.toUtf8().constData(),
                !regUser);
}

void QmlServer::connectedToServerTimeout()
{
    setconnectAction(0);
    emit connectedToServer();
}

void QmlServer::processError(int error)
{
    switch(static_cast<ServerError>(error)) {
    case NetErrPlayerKicked:
    case NetErrPlayerBanned:
    case NetErrPlayerBlocked:
    case NetErrSessionTimedOut:
        ManagerSingleton::Instance()->getSession()->terminateNetworkClient();
        break;
    }
}
