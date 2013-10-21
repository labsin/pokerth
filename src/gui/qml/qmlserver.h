#ifndef QMLSERVER_H
#define QMLSERVER_H

#include <QObject>

class QmlServer : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectAction)
    Q_PROPERTY(ConnectAction connectAction READ connectAction WRITE setconnectAction NOTIFY connectActionChanged)

public:
    explicit QmlServer(QObject *parent = 0);

    enum ConnectAction {
        ConnectActionNone = 0,
        ConnectInitDone,
        ConnectServerListDone,
        ConnectResolveDone,
        ConnectConnectDone,
        ConnectSessionDone,
        LimitConnect = ConnectSessionDone,
        ConnectActionLast = ConnectSessionDone,
        ConnectActionFirst = ConnectActionNone
    };

    ConnectAction connectAction() const
    {
        return static_cast<ConnectAction>(m_connectAction);
    }

    void setconnectAction(int arg);

    static void registerType();

signals:
    void connectingToServer();
    void connectedToServer();
    void signalLoginNeeded();
    void connectActionChanged(ConnectAction arg);

public slots:

    void setconnectAction(ConnectAction arg)
    {
        setconnectAction(static_cast<int>(arg));
    }

    void connectedToServerTimeout();

private:

    int m_connectAction;
};

#endif // QMLSERVER_H
