#ifndef QMLSERVER_H
#define QMLSERVER_H

#include <QObject>
#include "socket_msg.h"
#include "gamedata.h"
#include "serverdata.h"
#include <QStandardItem>

class QTimer;
class RoleItemModel;

class QmlServer : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectAction)
    Q_ENUMS(NetworkError)
    Q_ENUMS(NetworkNotification)
    Q_ENUMS(IdleRole)
    Q_PROPERTY(int connectAction READ connectAction WRITE setconnectAction NOTIFY connectActionChanged)
    Q_PROPERTY(float connectProgress READ connectProgress WRITE setconnectProgress NOTIFY connectProgressChanged)
    Q_PROPERTY(bool inGame READ inGame NOTIFY inGameChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY isAdminChanged)
    Q_PROPERTY(QObject* lobbyPlayers READ getLobbyPlayers NOTIFY lobbyPlayersChanged)
    Q_PROPERTY(QObject* connectedPlayers READ getConnectedPlayers NOTIFY connectedPlayersChanged)
    Q_PROPERTY(QObject* connectedSpectators READ getConnectedSpectators NOTIFY connectedSpectatorsChanged)
    Q_PROPERTY(QObject* games READ getGames NOTIFY gamesChanged)

public:
    explicit QmlServer(QObject *parent = 0);
    ~QmlServer();

    enum IdleRole {
        Idle = 0,
        Active
    };

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

    enum NetworkError {
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
        ConnErrIPv6Failed = ERR_SOCK_CONNECT_IPV6_FAILED,
        ConnErrConnectTimeout = ERR_SOCK_CONNECT_TIMEOUT,
        ConnErrIPv6Timeout = ERR_SOCK_CONNECT_IPV6_TIMEOUT,
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

    enum NetworkNotification {
        NetNotInternal = NTF_NET_INTERNAL,
        NetNotRemovedOnRequest = NTF_NET_REMOVED_ON_REQUEST,
        NetNotRemovedGameFull = NTF_NET_REMOVED_GAME_FULL,
        NetNotRemovedAlreadyRunning = NTF_NET_REMOVED_ALREADY_RUNNING,
        NetNotRemovedKicked = NTF_NET_REMOVED_KICKED,
        NetNotRemovedTimeout = NTF_NET_REMOVED_TIMEOUT,
        NetNotRemovedStartFailed = NTF_NET_REMOVED_START_FAILED,
        NetNotRemovedGameClosed = NTF_NET_REMOVED_GAME_CLOSED,

        NetNotJoinGameInvalid = NTF_NET_JOIN_GAME_INVALID,
        NetNotJoinGameFull = NTF_NET_JOIN_GAME_FULL,
        NetNotJoinAlreadyRunning = NTF_NET_JOIN_ALREADY_RUNNING,
        NetNotJoinInvalidPassword = NTF_NET_JOIN_INVALID_PASSWORD,
        NetNotJoinGuestForbidden = NTF_NET_JOIN_GUEST_FORBIDDEN,
        NetNotJoinNotInvited = NTF_NET_JOIN_NOT_INVITED,
        NetNotJoinGameNameInUse = NTF_NET_JOIN_GAME_NAME_IN_USE,
        NetNotJoinGameBadName = NTF_NET_JOIN_GAME_BAD_NAME,
        NetNotJoinInvalidSettings = NTF_NET_JOIN_INVALID_SETTINGS,
        NetNotJoinIpBlocked = NTF_NET_JOIN_IP_BLOCKED,
        NetNotJoinRejoinFailed = NTF_NET_JOIN_REJOIN_FAILED,
        NetNotJoinNoSpectators = NTF_NET_JOIN_NO_SPECTATORS,

        NetNotNewReleaseAvailable = NTF_NET_NEW_RELEASE_AVAILABLE,
        NetNotOutdatedBeta = NTF_NET_OUTDATED_BETA
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

    bool inGame() const
    {
        return m_inGame;
    }

    bool isAdmin() const
    {
        return m_isAdmin;
    }

signals:
    void connectingToServer();
    void connectedToServer();
    void loginNeeded();
    void serverChooseNeeded();
    void stopConnecting();
    void connectActionChanged(int arg);

    void connectProgressChanged(float arg);
    void networkError(int errorID, int osErrorID);
    void networkNotification(int notificationID);

    void inGameChanged(bool inGame);
    void isAdminChanged(bool isAdmin);
    void lobbyPlayersChanged(QObject* model);
    void connectedPlayersChanged(QObject* model);
    void connectedSpectatorsChanged(QObject* model);
    void gamesChanged(QObject* model);

    //Fix thread affinity
    void SignalNetClientConnect(int actionID);
    void SignalNetClientServerListShow();
    void SignalNetClientStatsUpdate(ServerStats stats);
    void SignalNetClientRemovedFromGame(int notificationId);
    void SignalNetClientSelfJoined(unsigned playerId, QString playerName, bool isGameAdmin);
    void SignalNetClientPlayerJoined(unsigned playerId, QString playerName, bool isGameAdmin);
    void SignalNetClientPlayerChanged(unsigned playerId, QString newPlayerName);
    void SignalNetClientPlayerLeft(unsigned playerId, QString playerName, int removeReason);
    void SignalNetClientSpectatorJoined(unsigned playerId, QString playerName);
    void SignalNetClientSpectatorLeft(unsigned playerId, QString playerName, int removeReason);
    void SignalNetClientNewGameAdmin(unsigned playerId, QString playerName);
    void SignalNetClientGameListNew(unsigned gameId);
    void SignalNetClientGameListRemove(unsigned gameId);
    void SignalNetClientGameListUpdateMode(unsigned gameId/*, GameMode mode*/);
    void SignalNetClientGameListUpdateAdmin(unsigned gameId, unsigned adminPlayerId);
    void SignalNetClientGameListPlayerJoined(unsigned gameId, unsigned playerId);
    void SignalNetClientGameListPlayerLeft(unsigned gameId, unsigned playerId);
    void SignalNetClientGameListSpectatorJoined(unsigned gameId, unsigned playerId);
    void SignalNetClientGameListSpectatorLeft(unsigned gameId, unsigned playerId);
    void SignalLobbyPlayerJoined(unsigned playerId, QString nickName);
    void SignalLobbyPlayerLeft(unsigned playerId);


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

    void refresh(int actionID);
    void joinedNetworkGame(unsigned playerID, QString playerName, bool isGameAdmin);
    void addConnectedPlayer(unsigned playerId, QString playerName, bool isAdmin);
    void updatePlayer(unsigned playerId, QString playerName);
    void removeConnectedPlayer(unsigned playerId, QString playerName);
    void addConnectedSpectator(unsigned playerId, QString playerName);
    void removeConnectedSpectator(unsigned playerId, QString playerName);
    void newGameAdmin(unsigned playerId, QString playerName);

    void addGame(unsigned gameId);
    void removeGame(unsigned gameId);
    void updateGameMode(unsigned gameId /*GameMode gameMode*/);
    void updateGameAdmin(unsigned gameId, unsigned adminPlayerId);
    void gameAddPlayer(unsigned gameId, unsigned playerId, bool spectator = false);
    void gameRemovePlayer(unsigned gameId, unsigned playerId, bool spectator = false);
    void gameAddSpectator(unsigned gameId, unsigned playerId);
    void gameRemoveSpectator(unsigned gameId, unsigned playerId);
    void removedFromGame(int notificationId);
    void updateStats(ServerStats serverStats);

    void lobbyAddPlayer(unsigned playerId, QString playerName);
    void lobbyRemovePlayer(unsigned playerId);

    QObject *getLobbyPlayers() const;
    QObject *getConnectedPlayers() const;
    QObject *getConnectedSpectators() const;
    QObject *getGames() const;

    void processError(int error);
    void processNotification(int notification);

private:
    
    void setInGame(bool arg)
    {
        if(m_inGame != arg) {
            m_inGame = arg;
            emit inGameChanged(arg);
        }
    }
    
    void setIsAdmin(bool arg)
    {
        if(m_isAdmin != arg) {
            m_isAdmin = arg;
            emit isAdminChanged(arg);
        }
    }

    void updateGameItemData(QStandardItem *item, unsigned gameId);
    void updateGameItem(QStandardItem *item, unsigned gameId);
    void updatePlayerItem(QStandardItem *item, unsigned playerId);

    int m_connectAction;
    float m_connectProgress;
    QTimer* m_connectToServerTimeout;
    
    unsigned m_playerId;
    bool m_isAdmin;
    bool m_inGame;

    RoleItemModel* m_gameModel;
    RoleItemModel* m_lobbyPlayerModel;
    RoleItemModel* m_connectedPlayerModel;
    RoleItemModel* m_connectedSpectatorModel;
    QHash<int, QByteArray> playerRoleNames;
    QHash<int, QByteArray> gameRoleNames;
};

#endif // QMLSERVER_H
