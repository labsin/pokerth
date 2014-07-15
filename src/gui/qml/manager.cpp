#include "manager.h"
#include "playerinterface.h"
#include <gamedata.h>
#include <generic/serverguiwrapper.h>
#include <net/socket_msg.h>
#include "tools.h"
#include "session.h"
#include "game.h"
#include "qmlgame.h"
#include "qmlcard.h"
#include "qmlplayer.h"
#include "storeplayers.h"
#include "playermodel.h"
#include "guiwrapper.h"
#include "configfile.h"
#include "configwrapper.h"
#include "qmlserver.h"
#include "roleitemmodel.h"
#include "qmlroles.h"
#include "QFile"

Manager::Manager() {
    connect(this,SIGNAL(afterInit()),this,SIGNAL(gameChanged()));
    connect(this,SIGNAL(afterInit()),this,SIGNAL(serverChanged()));
    connect(this,SIGNAL(afterInit()),this,SIGNAL(serversChanged()));
    connect(this, SIGNAL(SignalNetClientServerListAdd(uint)), this, SLOT(serverListAdd(uint)));
    connect(this, SIGNAL(SignalNetClientServerListClear()), this, SLOT(serverListClear()));
    connect(this, SIGNAL(signalSetPlayerAvatar(int,QString)), this, SLOT(setPlayerAvatar(int,QString)));
}

Manager::~Manager()
{
    delete myGame;
    delete myServer;
    delete myServerModel;
}

void Manager::Init(ConfigFile *c, Log *l)
{
    myGame = new QmlGame(this);
    myServer = new QmlServer(this);

    QHash<int, QByteArray> serverRoleNames;
    serverRoleNames[ServerEntry::NameRole] =  "name";
    serverRoleNames[ServerEntry::CountryRole] =  "country";
    serverRoleNames[ServerEntry::IdRole] =  "id";
    myServerModel = new RoleItemModel(serverRoleNames, this);
    myConfig = c;
    myLog = l;
    myGuiInterface.reset(new GuiWrapper());
    {
        mySession.reset(new Session(myGuiInterface.get(), myConfig, myLog));
        mySession->init(); // TODO handle error
        myLog->init();
        // 		myGuiInterface->setSession(session);
    }
    ConfigWrapper::registerType();
    GameDataClass::registerType();
    QmlPlayer::registerType();
    QmlCard::registerType();
    QmlGame::registerType();
    QmlServer::registerType();
}

void Manager::startGame(QObject* obj)
{
    if(GameDataClass* gameData = dynamic_cast<GameDataClass*>(obj)) {
        mySession->terminateNetworkClient();

        // Set dealer pos.
        if(gameData->defaultBlinds()) {
            gameData->resetBlinds();
        }
        GameData gd = gameData->getGameData();
        StartData startData;
        int tmpDealerPos = 0;
        startData.numberOfPlayers = gd.maxNumberOfPlayers;
        Tools::GetRand(0, startData.numberOfPlayers-1, 1, &tmpDealerPos);
        //if(DEBUG_MODE) {
        //    tmpDealerPos = 4;
        //}
        startData.startDealerPlayerId = static_cast<unsigned>(tmpDealerPos);

        //Start Game!!!
        mySession->startLocalGame(gd, startData);
    }
}

void Manager::stopGame()
{
    mySession->terminateNetworkClient();
    myGame->stopTimer();
    emit myGame->gameStopped();
    //myGame->saveGameTableGeometry();
}

void Manager::joinGame(unsigned gameId, QString password)
{
    qDebug() << "joinGame()";
    mySession->clientJoinGame(gameId, password.toUtf8().constData());
}

void Manager::joinGameLobby()
{
    cancelConnect();

    //start internet client with config values for user and pw
    mySession->startInternetClient();
    emit getServer()->connectingToServer();
}

void Manager::cancelConnect()
{
    myGame->stopTimer();
    mySession->terminateNetworkClient();
    if (myServerGuiInterface)
        myServerGuiInterface->getSession()->terminateNetworkServer();

    emit getServer()->stopConnecting();
}

bool Manager::isHosting()
{
    return (myServerGuiInterface && myServerGuiInterface->getSession()->isNetworkServerRunning());
}

QmlGame *Manager::getGame()
{
    return myGame;
}

QmlServer *Manager::getServer()
{
    return myServer;
}

QObject *Manager::getServerModel()
{
    return myServerModel;
}

void Manager::serverListAdd(unsigned serverId)
{
    ServerInfo info = getSession()->getClientServerInfo(serverId);
    QStandardItem* item = new QStandardItem();
    item->setData(info.id, ServerEntry::IdRole);
    item->setData(QString::fromStdString(info.name), ServerEntry::NameRole);
    item->setData(QString::fromStdString(info.country), ServerEntry::CountryRole);
    myServerModel->appendRow(item);
}

void Manager::serverListClear()
{
    myServerModel->clear();
}

void Manager::setPlayerAvatar(int myUniqueID, QString myAvatar)
{
    if(mySession->getCurrentGame()) {
        boost::shared_ptr<PlayerInterface> tmpPlayer = mySession->getCurrentGame()->getPlayerByUniqueId(myUniqueID);
        qDebug()<<"Avatar: "<<myAvatar;
        if (tmpPlayer.get()) {
            QString countryString(QString::fromLatin1(mySession->getClientPlayerInfo(myUniqueID).countryCode.c_str()).toLower());
            QmlPlayer* tmp = myGame->getPlayerModel()->at(tmpPlayer->getMyID());
            if(tmp) {
                tmp->setCountry(countryString);

                QFile myAvatarFile(myAvatar);
                if(myAvatarFile.exists()) {
                    tmp->setAvatar(myAvatar);
                } else {
                    tmp->setAvatar(QString(""));
                }
            }
        }
    }
}

GuiWrapper *Manager::getGui() {
    return dynamic_cast<GuiWrapper *>(myGuiInterface.get());
}
