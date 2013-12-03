#ifndef QMLSERVER_H
#define QMLSERVER_H

#include <QObject>
#include "socket_msg.h"

class QmlServer : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectAction)
    Q_ENUMS(ServerError)
    Q_PROPERTY(int connectAction READ connectAction WRITE setconnectAction NOTIFY connectActionChanged)
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

    enum ServerError {
        ConnErrInternal = ERR_SOCK_INTERNAL,
        ConnErrServerAddrNotSet = ERR_SOCK_SERVERADDR_NOT_SET,
        ConnErrInvalidPort = ERR_SOCK_INVALID_PORT,
        ConnErrCreationFailed = ERR_SOCK_CREATION_FAILED,
        ConnErrSetAddrFailed = ERR_SOCK_SET_ADDR_FAILED,
        ConnErrSetPortFailed = ERR_SOCK_SET_PORT_FAILED,
        ConnErrResolveFailed = ERR_SOCK_RESOLVE_FAILED,
        ConnErrBindFailed = ERR_SOCK_BIND_FAILED,
        ConnErrListenFailed = ERR_SOCK_LISTEN_FAILED,
        ConnErrAcceptFailed = ERR_SOCK_ACCEPT_FAILED,
        ConnErrConnectFailed = ERR_SOCK_CONNECT_FAILED,
        ConnErrConnectTimeout = ERR_SOCK_CONNECT_TIMEOUT,
        ConnErrSelectFailed = ERR_SOCK_SELECT_FAILED,
        ConnErrRecvFailed = ERR_SOCK_RECV_FAILED,
        ConnErrSendFailed = ERR_SOCK_SEND_FAILED,
        ConnErrConnReset = ERR_SOCK_CONN_RESET,
        ConnErrConnExists = ERR_SOCK_CONN_EXISTS,
        ConnErrInvalidPacket = ERR_SOCK_INVALID_PACKET,
        ConnErrInvalidState = ERR_SOCK_INVALID_STATE,
        ConnErrInvalidType = ERR_SOCK_INVALID_TYPE,
        ConnErrInvalidServerListURL = ERR_SOCK_INVALID_SERVERLIST_URL,
        ConnErrInvalidServerListXML = ERR_SOCK_INVALID_SERVERLIST_XML,
        ConnErrUnzipFailed = ERR_SOCK_UNZIP_FAILED,
        ConnErrTransferInitFailed = ERR_SOCK_TRANSFER_INIT_FAILED,
        ConnErrTransferOpenFailed = ERR_SOCK_TRANSFER_OPEN_FAILED,
        ConnErrTransferInvalidURL = ERR_SOCK_TRANSFER_INVALID_URL,
        ConnErrTransferSelectFailed = ERR_SOCK_TRANSFER_SELECT_FAILED,
        ConnErrTransferFailed = ERR_SOCK_TRANSFER_FAILED,
        NetErrVersionNotSupported = ERR_NET_VERSION_NOT_SUPPORTED,
        NetErrServerMaintenance = ERR_NET_SERVER_MAINTENANCE,
        NetErrServerFull = ERR_NET_SERVER_FULL,
        NetErrInvalidPassword = ERR_NET_INVALID_PASSWORD,
        NetErrInvalidPasswordStr = ERR_NET_INVALID_PASSWORD_STR,
        NetErrPlayerNameInUse = ERR_NET_PLAYER_NAME_IN_USE,
        NetErrInvalidPlayerName = ERR_NET_INVALID_PLAYER_NAME,
        NetErrInvalidPlayerCards = ERR_NET_INVALID_PLAYER_CARDS,
        NetErrInvalidPlayerResults = ERR_NET_INVALID_PLAYER_RESULTS,
        NetErrInvalidGameName = ERR_NET_INVALID_GAME_NAME,
        NetErrInvalidGameRound = ERR_NET_INVALID_GAME_ROUND,
        NetErrInvalidSession = ERR_NET_INVALID_SESSION,
        NetErrUnknownGame = ERR_NET_UNKNOWN_GAME,
        NetErrInvalidChatText = ERR_NET_INVALID_CHAT_TEXT,
        NetErrUnknownPlayerId = ERR_NET_UNKNOWN_PLAYER_ID,
        NetErrNoCurrentPlayer = ERR_NET_NO_CURRENT_PLAYER,
        NetErrPlayerNotActive = ERR_NET_PLAYER_NOT_ACTIVE,
        NetErrPlayerKicked = ERR_NET_PLAYER_KICKED,
        NetErrPlayerBanned = ERR_NET_PLAYER_BANNED,
        NetErrPlayerBlocked = ERR_NET_PLAYER_BLOCKED,
        NetErrSessionTimedOut = ERR_NET_SESSION_TIMED_OUT,
        NetErrInvalidPlayerCount = ERR_NET_INVALID_PLAYER_COUNT,
        NetErrTooManyManualBlinds = ERR_NET_TOO_MANY_MANUAL_BLINDS,
        NetErrInvalidAvatarFile = ERR_NET_INVALID_AVATAR_FILE,
        NetErrAvatarTooLarge = ERR_NET_AVATAR_TOO_LARGE,
        NetErrBufInvalidSize = ERR_NET_BUF_INVALID_SIZE,
        NetErrInvalidRequestId = ERR_NET_INVALID_REQUEST_ID,
        NetErrWrongAvatarSize = ERR_NET_WRONG_AVATAR_SIZE,
        NetErrStartTimeout = ERR_NET_START_TIMEOUT,
        NetErrGameTerminationFailed = ERR_NET_GAME_TERMINATION_FAILED,
        NetErrInternalGameError = ERR_NET_INTERNAL_GAME_ERROR,
        NetErrDealerNotFound = ERR_NET_DEALER_NOT_FOUND,
        NetErrInitBlocked = ERR_NET_INIT_BLOCKED,
        NetErrGSASLInitFailed = ERR_NET_GSASL_INIT_FAILED,
        NetErrGSASLNoScram = ERR_NET_GSASL_NO_SCRAM,
        NetErrDBConnectFailed = ERR_NET_DB_CONNECT_FAILED

    };

    int connectAction() const
    {
        return m_connectAction;
    }

    Q_INVOKABLE void setLoginData(bool regUser, QString userName, QString password, bool rememberPassword);

    static void registerType();

    float connectProgress() const
    {
        return m_connectProgress;
    }

signals:
    void connectingToServer();
    void connectedToServer();
    void loginNeeded();
    void stopConnecting();
    void connectActionChanged(int arg);

    void connectProgressChanged(float arg);
    void serverError(int error);

public slots:

    void setconnectAction(int arg);

    void connectedToServerTimeout();

    void setconnectProgress(float arg)
    {
        if (m_connectProgress != arg) {
            m_connectProgress = arg;
            emit connectProgressChanged(arg);
        }
    }

    void processError(int error);

private:

    int m_connectAction;
    float m_connectProgress;
};

#endif // QMLSERVER_H
