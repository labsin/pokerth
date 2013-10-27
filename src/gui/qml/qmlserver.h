#ifndef QMLSERVER_H
#define QMLSERVER_H

#include <QObject>

class QmlServer : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectAction)
    Q_PROPERTY(ConnectAction connectAction READ connectAction WRITE setconnectAction NOTIFY connectActionChanged)
    Q_PROPERTY(float connectProgress READ connectProgress WRITE setconnectProgress NOTIFY connectProgressChanged)

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

    float connectProgress() const
    {
        return m_connectProgress;
    }

signals:
    void connectingToServer();
    void connectedToServer();
    void loginNeeded();
    void connectActionChanged(ConnectAction arg);

    void connectProgressChanged(float arg);

public slots:

    void setconnectAction(ConnectAction arg)
    {
        setconnectAction(static_cast<int>(arg));
    }

    void connectedToServerTimeout();

    void setconnectProgress(float arg)
    {
        if (m_connectProgress != arg) {
            m_connectProgress = arg;
            emit connectProgressChanged(arg);
        }
    }

private:

    int m_connectAction;
    float m_connectProgress;
};

#endif // QMLSERVER_H
