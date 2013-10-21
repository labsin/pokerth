#include "qmlserver.h"
#include <net/socket_msg.h>
#include <QTimer>
#include <QtQml>

QmlServer::QmlServer(QObject *parent) :
    QObject(parent), m_connectAction(ConnectActionNone)
{
}

void QmlServer::setconnectAction(int arg)
{
    if(arg>ConnectActionLast || arg<ConnectActionFirst)
        return;
    if (m_connectAction != arg) {
        m_connectAction = arg;
        emit connectActionChanged(connectAction());
        if(arg==ConnectActionLast) {
            QTimer::singleShot(1000,this,SLOT(connectedToServerTimeout()));
        }
    }
}

void QmlServer::registerType()
{
    qmlRegisterType<QmlServer>("PokerTH",1, 0, "Server");
}

void QmlServer::connectedToServerTimeout()
{
    setconnectAction(0);
    emit connectedToServer();
}
