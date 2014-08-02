#include "qmlserver.h"
#include <net/socket_msg.h>
#include <QTimer>
#include <QtQml>
#include "manager.h"
#include "session.h"
#include "configfile.h"
#include "tools.h"
#include "roleitemmodel.h"
#include "qmlenums.h"
#include "gamedataclass.h"
#include <net/socket_msg.h>

QmlServer::QmlServer(QObject *parent) :
    QObject(parent), m_connectAction(ConnectActionNone), m_playerId(0), m_isAdmin(false) , m_inGame(false)
{
    connect(this,SIGNAL(networkError(int, int)),this,SLOT(processError(int)));
    connect(this,SIGNAL(networkNotification(int)),this,SLOT(processNotification(int)));
    m_connectToServerTimeout = new QTimer(this);
    m_connectToServerTimeout->setSingleShot(true);
    connect(m_connectToServerTimeout, SIGNAL(timeout()), this, SLOT(connectedToServerTimeout()));

    gameRoleNames[QmlEnums::GameNameRole] =  "name";
    gameRoleNames[QmlEnums::GameIdRole] = "gameId";
    gameRoleNames[QmlEnums::GameMeInThisGameRole] = "meInThisGame";
    gameRoleNames[QmlEnums::GameInfoRole] = "info";
    gameRoleNames[QmlEnums::GamePlayerModelRole] = "players";
    gameRoleNames[QmlEnums::GameSpectatorModelRole] = "spectators";

    m_gameModel = new RoleItemModel(gameRoleNames, this);
    gamesChanged(m_gameModel);

    playerRoleNames[QmlEnums::NickNameRole] =  "name";
    playerRoleNames[QmlEnums::NickIdRole] = "playerId";
    playerRoleNames[QmlEnums::NickCountryRole] = "country";
    playerRoleNames[QmlEnums::NickIsGuestRole] = "isGuest";
    playerRoleNames[QmlEnums::NickIdleRole] = "idle";
    playerRoleNames[QmlEnums::NickIsAdminRole] = "isAdmin";

    m_lobbyPlayerModel = new RoleItemModel(playerRoleNames, this);
    m_connectedPlayerModel = new RoleItemModel(playerRoleNames, this);
    m_connectedSpectatorModel = new RoleItemModel(playerRoleNames, this);
    lobbyPlayersChanged(m_lobbyPlayerModel);
    connectedPlayersChanged(m_connectedPlayerModel);
    connectedSpectatorsChanged(m_connectedSpectatorModel);

    QHash<int, QByteArray> chatRoleNames;
    chatRoleNames[QmlEnums::ChatPlayerName] = "playerName";
    chatRoleNames[QmlEnums::ChatMessage] = "message";
    chatRoleNames[QmlEnums::ChatPM] = "pm";
    chatRoleNames[QmlEnums::ChatLobby] = "fromLobby";
    chatRoleNames[QmlEnums::ChatGame] = "fromGame";
    chatRoleNames[QmlEnums::ChatBot] = "fromBot";
    chatRoleNames[QmlEnums::ChatMeReferred] = "meReferred";
    chatRoleNames[QmlEnums::ChatMyMessage] = "myMessage";
    chatRoleNames[QmlEnums::ChatOnIgnoreList] = "onIgnoreList";

    m_chatModel = new RoleItemModel(chatRoleNames, this);
    chatModelChanged(m_chatModel);
    refreshIgnoreList();
    refreshMynick();

    //Fix thread affinity
    connect(this, SIGNAL(SignalNetClientConnect(int)), this, SLOT(setconnectAction(int)));
    connect(this, SIGNAL(SignalNetClientServerListShow()), this, SIGNAL(serverChooseNeeded()));
    connect(this, SIGNAL(SignalNetClientStatsUpdate(ServerStats)), this, SLOT(updateStats(ServerStats)));
    connect(this, SIGNAL(SignalNetClientRemovedFromGame(int)),this, SLOT(removedFromGame(int)));
    connect(this, SIGNAL(SignalNetClientSelfJoined(uint,QString,bool)),this, SLOT(joinedNetworkGame(uint,QString,bool)));
    connect(this, SIGNAL(SignalNetClientPlayerJoined(uint,QString,bool)), this, SLOT(addConnectedPlayer(uint,QString,bool)));
    connect(this, SIGNAL(SignalNetClientPlayerChanged(uint,QString)), this, SLOT(updatePlayer(uint,QString)));
    connect(this, SIGNAL(SignalNetClientPlayerLeft(uint,QString,int)), this, SLOT(removeConnectedPlayer(uint,QString)));
    connect(this, SIGNAL(SignalNetClientSpectatorJoined(uint,QString)), this, SLOT(addConnectedSpectator(uint,QString)));
    connect(this, SIGNAL(SignalNetClientSpectatorLeft(uint,QString,int)), this, SLOT(removeConnectedSpectator(uint,QString)));
    connect(this, SIGNAL(SignalNetClientNewGameAdmin(uint,QString)), this, SLOT(newGameAdmin(uint,QString)));
    connect(this, SIGNAL(SignalNetClientGameListNew(uint)), this, SLOT(addGame(uint)));
    connect(this, SIGNAL(SignalNetClientGameListRemove(uint)), this, SLOT(removeGame(uint)));
    connect(this, SIGNAL(SignalNetClientGameListUpdateMode(uint)), this, SLOT(updateGameMode(uint)));
    connect(this, SIGNAL(SignalNetClientGameListUpdateAdmin(uint,uint)), this, SLOT(updateGameAdmin(uint,uint)));
    connect(this, SIGNAL(SignalNetClientGameListPlayerJoined(uint,uint)), this, SLOT(gameAddPlayer(uint,uint)));
    connect(this, SIGNAL(SignalNetClientGameListPlayerLeft(uint,uint)), this, SLOT(gameRemovePlayer(uint,uint)));
    connect(this, SIGNAL(SignalNetClientGameListSpectatorJoined(uint,uint)), this, SLOT(gameAddSpectator(uint,uint)));
    connect(this, SIGNAL(SignalNetClientGameListSpectatorLeft(uint,uint)), this, SLOT(gameRemoveSpectator(uint,uint)));
    connect(this, SIGNAL(SignalLobbyPlayerJoined(uint,QString)), this, SLOT(lobbyAddPlayer(uint,QString)));
    connect(this, SIGNAL(SignalLobbyPlayerLeft(uint)), this, SLOT(lobbyRemovePlayer(uint)));

    // Chat
    connect(this, SIGNAL(SignalNetClientGameChatMsg(QString,QString)), this, SLOT(gameChatMsg(QString,QString)));
    connect(this, SIGNAL(SignalNetClientLobbyChatMsg(QString,QString)), this, SLOT(lobbyChatMsg(QString,QString)));
    connect(this, SIGNAL(SignalNetClientPrivateChatMsg(QString,QString)), this, SLOT(pmChatMsg(QString,QString)));
}

QmlServer::~QmlServer()
{
    delete m_connectToServerTimeout;
    delete m_gameModel;
    delete m_connectedPlayerModel;
    delete m_connectedSpectatorModel;
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
            m_connectToServerTimeout->start(1000);
            refreshMynick();
        }
    }
}

void QmlServer::registerType()
{
    qmlRegisterType<QmlServer>("PokerTH",1, 0, "Server");
    qRegisterMetaType<QmlServer::ConnectAction>("ConnectAction");
    qRegisterMetaType<QmlServer::NetworkError>("NetworkError");
    qRegisterMetaType<QmlServer::NetworkNotification>("NetworkNotification");
}

QString QmlServer::checkForEmotes(QString msg)
{

    //prevent links from beeing broken by :/ emoticon
    if(!msg.contains("http://") && !msg.contains("https://")) {
        msg.replace(":/", "<img src=\"qrc:/emotes/emotes/face-uncertain.png\" />");
    }

    msg.replace("0:-)", "<img src=\"qrc:/emotes/emotes/face-angel.png\" />");
    msg.replace("X-(", "<img src=\"qrc:/emotes/emotes/face-angry.png\" />");
    msg.replace("B-)", "<img src=\"qrc:/emotes/emotes/face-cool.png\" />");
    msg.replace("8-)", "<img src=\"qrc:/emotes/emotes/face-cool.png\" />");
    msg.replace(":'(", "<img src=\"qrc:/emotes/emotes/face-crying.png\" />");
    msg.replace("&gt;:-)", "<img src=\"qrc:/emotes/emotes/face-devilish.png\" />");
    msg.replace(":-[", "<img src=\"qrc:/emotes/emotes/face-embarrassed.png\" />");
    msg.replace(":-*", "<img src=\"qrc:/emotes/emotes/face-kiss.png\" />");
    msg.replace(":-))", "<img src=\"qrc:/emotes/emotes/face-laugh.png\" />");
    msg.replace(":))", "<img src=\"qrc:/emotes/emotes/face-laugh.png\" />");
    msg.replace(":-|", "<img src=\"qrc:/emotes/emotes/face-plain.png\" />");
    msg.replace(":-P", "<img src=\"qrc:/emotes/emotes/face-raspberry.png\" />");
    msg.replace(":-p", "<img src=\"qrc:/emotes/emotes/face-raspberry.png\" />");
    msg.replace(":-(", "<img src=\"qrc:/emotes/emotes/face-sad.png\" />");
    msg.replace(":(", "<img src=\"qrc:/emotes/emotes/face-sad.png\" />");
    msg.replace(":-&", "<img src=\"qrc:/emotes/emotes/face-sick.png\" />");
    msg.replace(":-D", "<img src=\"qrc:/emotes/emotes/face-smile-big.png\" />");
    msg.replace(":D", "<img src=\"qrc:/emotes/emotes/face-smile-big.png\" />");
    msg.replace(":-!", "<img src=\"qrc:/emotes/emotes/face-smirk.png\" />");
    msg.replace(":-0", "<img src=\"qrc:/emotes/emotes/face-surprise.png\" />");
    msg.replace(":-O", "<img src=\"qrc:/emotes/emotes/face-surprise.png\" />");
    msg.replace(":-o", "<img src=\"qrc:/emotes/emotes/face-surprise.png\" />");
    msg.replace(":-/", "<img src=\"qrc:/emotes/emotes/face-uncertain.png\" />");

    msg.replace(";-)", "<img src=\"qrc:/emotes/emotes/face-wink.png\" />");
    msg.replace(";)", "<img src=\"qrc:/emotes/emotes/face-wink.png\" />");
    msg.replace(":-S", "<img src=\"qrc:/emotes/emotes/face-worried.png\" />");
    msg.replace(":-s", "<img src=\"qrc:/emotes/emotes/face-worried.png\" />");
    msg.replace(":-)", "<img src=\"qrc:/emotes/emotes/face-smile.png\" />");
    msg.replace(":)", "<img src=\"qrc:/emotes/emotes/face-smile.png\" />");

    return msg;
}

QObject *QmlServer::getLobbyPlayers() const
{
    return dynamic_cast<QObject *>(m_lobbyPlayerModel);
}

QObject *QmlServer::getConnectedPlayers() const
{
    return dynamic_cast<QObject *>(m_connectedPlayerModel);
}

QObject *QmlServer::getConnectedSpectators() const
{
    return dynamic_cast<QObject *>(m_connectedSpectatorModel);
}

QObject *QmlServer::getGames() const
{
    return dynamic_cast<QObject *>(m_gameModel);
}

QObject *QmlServer::getChatModel() const
{
    return dynamic_cast<QObject *>(m_chatModel);
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
                userName.toUtf8().constData(),
                password.toUtf8().constData(),
                !regUser);
    ManagerSingleton::Instance()->getSession()->terminateNetworkServer();
}

void QmlServer::connectedToServerTimeout()
{
    setconnectAction(0);
    emit connectedToServer();
}

void QmlServer::refresh(int actionID)
{
	if (actionID == MSG_NET_GAME_CLIENT_START) {
		QTimer::singleShot(500, this, SLOT(accept()));
	}
}

void QmlServer::joinedNetworkGame(unsigned playerId, QString playerName, bool isGameAdmin)
{
    setInGame(true);
    setIsAdmin(isGameAdmin);
    m_playerId = playerId;
    addConnectedPlayer(playerId, playerName, isGameAdmin);
}

void QmlServer::updatePlayer(unsigned playerId, QString playerName)
{
    QModelIndexList indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        updatePlayerItem(m_lobbyPlayerModel->itemFromIndex(*i), playerId);
    }
    indexes = m_connectedPlayerModel->match(m_connectedPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        updatePlayerItem(m_connectedPlayerModel->itemFromIndex(*i), playerId);
    }

    int it = 0;
    while(m_gameModel->item(it)) {
        QStandardItem* item = m_gameModel->item(it);
        int it1 = 0;
        bool found = false;
        QVariant var = item->data(QmlEnums::GamePlayerModelRole);
        if(var.canConvert<QObject*>()) {
            RoleItemModel* model = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
            indexes = model->match(model->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
            for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
                updatePlayerItem(model->itemFromIndex(*i), playerId);
                found = true;
            }
        }
        var = item->data(QmlEnums::GameSpectatorModelRole);
        if(var.canConvert<QObject*>()) {
            RoleItemModel* model = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
            indexes = model->match(model->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
            for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
                updatePlayerItem(model->itemFromIndex(*i), playerId);
                found = true;
            }
        }
        if(found) {
            break;
        }
        it++;
    }
}

void QmlServer::addConnectedPlayer(unsigned playerId, QString playerName, bool isAdmin)
{
    QStandardItem *item = new QStandardItem();
    m_connectedPlayerModel->appendRow(item);
    updatePlayerItem(item, playerId);
    item->setData(Idle, QmlEnums::NickIdleRole);
}

void QmlServer::removeConnectedPlayer(unsigned playerId, QString playerName)
{
    QModelIndexList indexes = m_connectedPlayerModel->match(m_connectedPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_connectedPlayerModel->removeRow((*i).row());;
    }
}

void QmlServer::addConnectedSpectator(unsigned playerId, QString playerName)
{
    QStandardItem *item = new QStandardItem();
    m_connectedSpectatorModel->appendRow(item);
    updatePlayerItem(item, playerId);
    item->setData(Idle, QmlEnums::NickIdleRole);
}

void QmlServer::removeConnectedSpectator(unsigned playerId, QString playerName)
{
    QModelIndexList indexes = m_connectedSpectatorModel->match(m_connectedSpectatorModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_connectedSpectatorModel->removeRow((*i).row());;
    }
}

void QmlServer::newGameAdmin(unsigned playerId, QString playerName)
{
    int it = 0;
    while (m_connectedPlayerModel->item(it)) {
        if (m_connectedPlayerModel->item(it)->data(QmlEnums::NickIdRole) == playerId) {
            updatePlayerItem(m_connectedPlayerModel->item(it),it);
        }
        else if(m_connectedPlayerModel->item(it)->data(QmlEnums::NickIsAdminRole).toBool()) {
            updatePlayerItem(m_connectedPlayerModel->item(it),it);
        }
        it++;
    }

    if (m_inGame && m_playerId == playerId) {
        setIsAdmin(true);
    }
}

void QmlServer::updatePlayerItem(QStandardItem* item, unsigned playerId)
{
    PlayerInfo playerInfo(ManagerSingleton::Instance()->getSession()->getClientPlayerInfo(playerId));

    item->setData(QString::fromUtf8(playerInfo.playerName.c_str()), QmlEnums::NickNameRole);
    item->setData(QString::fromUtf8(playerInfo.countryCode.c_str()), QmlEnums::NickCountryRole);
    item->setData(playerId, QmlEnums::NickIdRole);
    item->setData(playerInfo.isGuest, QmlEnums::NickIsGuestRole);
//    item->setData(Idle, QmlEnums::NickIdleRole);
    item->setData(playerInfo.isAdmin, QmlEnums::NickIsAdminRole);
}

void QmlServer::addChatMessage(int type, QString playerName, QString message)
{
    QStandardItem* item = new QStandardItem();

    item->setData(QVariant::fromValue(playerName), QmlEnums::ChatPlayerName);

    switch(type) {
    case 0: // PM
        item->setData(QVariant::fromValue(true),QmlEnums::ChatPM);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatGame);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatLobby);
        break;
    case 1: // Game
        item->setData(QVariant::fromValue(false),QmlEnums::ChatPM);
        item->setData(QVariant::fromValue(true),QmlEnums::ChatGame);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatLobby);
        break;
    case 2: // Lobby
        item->setData(QVariant::fromValue(false),QmlEnums::ChatPM);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatGame);
        item->setData(QVariant::fromValue(true),QmlEnums::ChatLobby);
        break;
    default:
        item->setData(QVariant::fromValue(false),QmlEnums::ChatPM);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatGame);
        item->setData(QVariant::fromValue(false),QmlEnums::ChatLobby);
        break;
    }

    message = message.replace("<","&lt;");
    message = message.replace(">","&gt;");
    //doing the links
    message = message.replace(QRegExp("((?:https?)://\\S+)"), "<a href=\"\\1\">\\1</a>");

    if(!ManagerSingleton::Instance()->getConfig()->readConfigInt("DisableChatEmoticons")) {
        message = checkForEmotes(message);
    }
    item->setData(QVariant::fromValue(message),QmlEnums::ChatMessage);

    if(message.contains(myNick, Qt::CaseInsensitive)) {
        item->setData(QVariant::fromValue(true),QmlEnums::ChatMeReferred);
    }
    else {
        item->setData(QVariant::fromValue(false),QmlEnums::ChatMeReferred);
    }

    if(playerName == myNick) {
        item->setData(QVariant::fromValue(true),QmlEnums::ChatMyMessage);
    }
    else {
        item->setData(QVariant::fromValue(false),QmlEnums::ChatMyMessage);
    }

    bool isBot = playerName == "(chat bot)";
    if(isBot) {
        item->setData(QVariant::fromValue(true),QmlEnums::ChatBot);
    }
    else {
        item->setData(QVariant::fromValue(false),QmlEnums::ChatBot);
    }

    bool found = false;
    std::list<std::string>::iterator it1;
    for(it1=ignoreList.begin(); it1 != ignoreList.end(); ++it1) {
        if(playerName == QString::fromUtf8(it1->c_str())) {
            found = true;
            break;
        }
        if(isBot && message.startsWith(QString::fromUtf8(it1->c_str()))) {
            found = true;
            break;
        }
    }
    if(found)
        item->setData(QVariant::fromValue(true),QmlEnums::ChatOnIgnoreList);
    else
        item->setData(QVariant::fromValue(false),QmlEnums::ChatOnIgnoreList);
    m_chatModel->appendRow(item);
}

void QmlServer::updateGameItemData(QStandardItem* item, unsigned gameId)
{
    GameInfo info(ManagerSingleton::Instance()->getSession()->getClientGameInfo(gameId));
    QVariant gi;
    QObject* giClass = new GameInfoClass(info, this);
    gi.setValue(dynamic_cast<QObject*>(giClass));
    item->setData(gi, QmlEnums::GameInfoRole);
}

void QmlServer::updateGameItem(QStandardItem* item, unsigned gameId)
{
    GameInfo info(ManagerSingleton::Instance()->getSession()->getClientGameInfo(gameId));
    item->setData(gameId, QmlEnums::GameIdRole);
    item->setData(QString::fromUtf8(info.name.c_str()), QmlEnums::GameNameRole);
    updateGameItemData(item,gameId);

    PlayerIdList::const_iterator i = info.players.begin();
    PlayerIdList::const_iterator end = info.players.end();

    QVariant var = item->data(QmlEnums::GamePlayerModelRole);
    if(var.canConvert<QObject*>()) {
        RoleItemModel* model = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
        model->clear();
        while (i != end) {
            QStandardItem *player = new QStandardItem();
            model->appendRow(player);
            updatePlayerItem(player, *i);
            item->setData(Active, QmlEnums::NickIdleRole);

            if(m_playerId == *i) {
                item->setData( true, QmlEnums::GameMeInThisGameRole);
            } else {
                item->setData( false, QmlEnums::GameMeInThisGameRole);
            }
            ++i;
        }
    }

    i = info.spectators.begin();
    end = info.spectators.end();

    var = item->data(QmlEnums::GameSpectatorModelRole);
    if(var.canConvert<QObject*>()) {
        RoleItemModel* model = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
        model->clear();
        while (i != end) {
            QStandardItem *player = new QStandardItem();
            model->appendRow(player);
            updatePlayerItem(player, *i);
            item->setData(Active, QmlEnums::NickIdleRole);

            if(m_playerId == *i) {
                item->setData( true, QmlEnums::GameMeInThisGameRole);
            } else {
                item->setData( false, QmlEnums::GameMeInThisGameRole);
            }
            ++i;
        }
    }

    //reset players iterator
    i = info.players.begin();
    end = info.players.end();
    while (i != end) {
        //mark players as active
        QModelIndexList indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,*i,1,Qt::MatchExactly);
        for (QModelIndexList::iterator i2 = indexes.begin(); i2 != indexes.end(); ++i2) {
            m_lobbyPlayerModel->itemFromIndex(*i2)->setData(Active, QmlEnums::NickIdleRole);
        }
        indexes = m_connectedPlayerModel->match(m_connectedPlayerModel->index(0,0),QmlEnums::NickIdRole,*i,1,Qt::MatchExactly);
        for (QModelIndexList::iterator i2 = indexes.begin(); i2 != indexes.end(); ++i2) {
            m_connectedPlayerModel->itemFromIndex(*i2)->setData(Active, QmlEnums::NickIdleRole);
        }
        ++i;
    }
    i = info.spectators.begin();
    end = info.spectators.end();
    while (i != end) {
        //mark spectators as active
        QModelIndexList indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,*i,1,Qt::MatchExactly);
        for (QModelIndexList::iterator i2 = indexes.begin(); i2 != indexes.end(); ++i2) {
            m_lobbyPlayerModel->itemFromIndex(*i2)->setData(Active, QmlEnums::NickIdleRole);
        }
        indexes = m_connectedSpectatorModel->match(m_connectedSpectatorModel->index(0,0),QmlEnums::NickIdRole,*i,1,Qt::MatchExactly);
        for (QModelIndexList::iterator i2 = indexes.begin(); i2 != indexes.end(); ++i2) {
            m_connectedSpectatorModel->itemFromIndex(*i2)->setData(Active, QmlEnums::NickIdleRole);
        }
        ++i;
    }
}

void QmlServer::addGame(unsigned gameId)
{
    QStandardItem* item = new QStandardItem();
    QVariant playerModel;
    RoleItemModel *playerModelObj = new RoleItemModel(playerRoleNames, this);
    playerModelObj->moveToThread(m_gameModel->thread());
    playerModelObj->setParent(m_gameModel);
    playerModel.setValue(dynamic_cast<QObject*>(playerModelObj));
    item->setData(playerModel, QmlEnums::GamePlayerModelRole);

    QVariant spectatorModel;
    RoleItemModel *spectatorModelObj = new RoleItemModel(playerRoleNames, this);
    spectatorModelObj->moveToThread(m_gameModel->thread());
    spectatorModelObj->setParent(m_gameModel);
    spectatorModel.setValue(dynamic_cast<QObject*>(spectatorModelObj));
    item->setData(spectatorModel, QmlEnums::GameSpectatorModelRole);

    updateGameItem(item, gameId);

    m_gameModel->appendRow(item);
}

void QmlServer::removeGame(unsigned gameId)
{
    QModelIndexList indexes = m_gameModel->match(m_gameModel->index(0,0),QmlEnums::GameIdRole,gameId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_gameModel->removeRow((*i).row());
    }
}

void QmlServer::updateGameMode(unsigned gameId /* GameMode gameMode*/)
{
    QModelIndexList indexes = m_gameModel->match(m_gameModel->index(0,0),QmlEnums::GameIdRole,gameId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        updateGameItemData(m_gameModel->itemFromIndex(*i),gameId);
    }
}

void QmlServer::updateGameAdmin(unsigned gameId, unsigned adminPlayerId)
{
    if (m_inGame && m_playerId == adminPlayerId) {
        setIsAdmin(true);
    }
    QModelIndexList indexes = m_gameModel->match(m_gameModel->index(0,0),QmlEnums::GameIdRole,gameId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        updateGameItemData(m_gameModel->itemFromIndex(*i),gameId);
    }
}

void QmlServer::gameAddPlayer(unsigned gameId, unsigned playerId, bool spectator)
{
    QModelIndexList indexes = m_gameModel->match(m_gameModel->index(0,0),QmlEnums::GameIdRole,gameId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        QStandardItem* item = m_gameModel->itemFromIndex(*i);
        QVariant var;
        if(!spectator) {
            var = item->data(QmlEnums::GamePlayerModelRole);
        }
        else {
            var = item->data(QmlEnums::GameSpectatorModelRole);
        }
        if(var.canConvert<QObject*>()) {
            RoleItemModel* playerModel = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
            QModelIndexList indexes2 = playerModel->match(playerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
            if(indexes2.count()==0) {
                QStandardItem *player = new QStandardItem();
                playerModel->appendRow(player);
                updatePlayerItem(player, playerId);
                item->setData(Active, QmlEnums::NickIdleRole);

                if(m_playerId == playerId) {
                    item->setData( true, QmlEnums::GameMeInThisGameRole);
                } else {
                    item->setData( false, QmlEnums::GameMeInThisGameRole);
                }
            }
            else {
                updatePlayerItem(playerModel->itemFromIndex(indexes2[0]), playerId);
                item->setData(Active, QmlEnums::NickIdleRole);
            }
        }
    }
    //mark player as active
    indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_lobbyPlayerModel->itemFromIndex(*i)->setData(Active, QmlEnums::NickIdleRole);
    }
    indexes = m_connectedPlayerModel->match(m_connectedPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_connectedPlayerModel->itemFromIndex(*i)->setData(Active, QmlEnums::NickIdleRole);
    }
}

void QmlServer::gameRemovePlayer(unsigned gameId, unsigned playerId, bool spectator)
{
    QModelIndexList indexes = m_gameModel->match(m_gameModel->index(0,0),QmlEnums::GameIdRole,gameId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        QStandardItem* item = m_gameModel->itemFromIndex(*i);
        QVariant var;
        if(!spectator) {
            var = item->data(QmlEnums::GamePlayerModelRole);
        }
        else {
            var = item->data(QmlEnums::GameSpectatorModelRole);
        }
        if(var.canConvert<QObject*>()) {
            RoleItemModel* playerModel = dynamic_cast<RoleItemModel*>(var.value<QObject*>());
            QModelIndexList indexes2 = playerModel->match(playerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
            for (QModelIndexList::iterator i2 = indexes2.begin(); i2 != indexes2.end(); ++i2) {
                playerModel->removeRow((*i2).row());
            }
        }
    }
    //mark player as idle
    indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_lobbyPlayerModel->itemFromIndex(*i)->setData(Idle, QmlEnums::NickIdleRole);
    }
    indexes = m_connectedPlayerModel->match(m_connectedPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_connectedPlayerModel->itemFromIndex(*i)->setData(Idle, QmlEnums::NickIdleRole);
    }
}

void QmlServer::gameAddSpectator(unsigned gameId, unsigned playerId)
{
    gameAddPlayer(gameId, playerId, true);
}

void QmlServer::gameRemoveSpectator(unsigned gameId, unsigned playerId)
{
    gameRemovePlayer(gameId, playerId, true);
}

void QmlServer::removedFromGame(int notificationId)
{
    setInGame(false);
    setIsAdmin(false);
    m_playerId = 0;
}

void QmlServer::updateStats(ServerStats serverStats)
{

}

void QmlServer::lobbyAddPlayer(unsigned playerId, QString playerName)
{
    QStandardItem *item = new QStandardItem();
    m_lobbyPlayerModel->appendRow(item);
    updatePlayerItem(item, playerId);
    item->setData(Idle, QmlEnums::NickIdleRole);
}

void QmlServer::lobbyRemovePlayer(unsigned playerId)
{
    int it1 = 0;
    QModelIndexList indexes = m_lobbyPlayerModel->match(m_lobbyPlayerModel->index(0,0),QmlEnums::NickIdRole,playerId,1,Qt::MatchExactly);
    for (QModelIndexList::iterator i = indexes.begin(); i != indexes.end(); ++i) {
        m_lobbyPlayerModel->removeRow((*i).row());
    }
}

void QmlServer::processError(int error)
{
    switch(static_cast<NetworkError>(error)) {
    case NetErrPlayerKicked:
    case NetErrPlayerBanned:
    case NetErrPlayerBlocked:
    case NetErrSessionTimedOut:
        ManagerSingleton::Instance()->getSession()->terminateNetworkClient();
        break;
    }
}

void QmlServer::processNotification(int notification)
{
    switch(static_cast<NetworkNotification>(notification)) {
    //TODO
    }
}

void QmlServer::refreshIgnoreList()
{
    ignoreList = ManagerSingleton::Instance()->getConfig()->readConfigStringList("PlayerIgnoreList");
}

void QmlServer::refreshMynick()
{
    //refresh myNick if it was changed during runtime
    myNick = QString::fromUtf8(ManagerSingleton::Instance()->getConfig()->readConfigString("MyName").c_str());
}
