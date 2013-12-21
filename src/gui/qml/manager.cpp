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
#include "guiwrapper.h"
#include "configfile.h"
#include "configwrapper.h"
#include "qmlserver.h"
#include "servermodel.h"

void Manager::Init(ConfigFile *c, Log *l)
{
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

void Manager::joinGameLobby()
{
    cancelConnect();

    //start internet client with config values for user and pw TODO
    mySession->startInternetClient();
    emit getServer()->connectingToServer();
}

void Manager::cancelConnect()
{
    dynamic_cast<GuiWrapper *>(myGuiInterface.get())->getGame()->stopTimer();
    mySession->terminateNetworkClient();
    if (myServerGuiInterface)
        myServerGuiInterface->getSession()->terminateNetworkServer();

    emit getServer()->stopConnecting();
}

QmlGame *Manager::getGame()
{
    return dynamic_cast<GuiWrapper*>(myGuiInterface.get())->getGame();
}

QmlServer *Manager::getServer()
{
    return dynamic_cast<GuiWrapper*>(myGuiInterface.get())->getServer();
}

QAbstractTableModel *Manager::getServerModel()
{
    return dynamic_cast<QAbstractTableModel*>(dynamic_cast<GuiWrapper*>(myGuiInterface.get())->getServerModel());
}

GuiWrapper *Manager::getGui() {
    return dynamic_cast<GuiWrapper *>(myGuiInterface.get());
}
