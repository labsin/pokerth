/*****************************************************************************
 * PokerTH - The open source texas holdem engine                             *
 * Copyright (C) 2006-2012 Felix Hammer, Florian Thauer, Lothar May          *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as            *
 * published by the Free Software Foundation, either version 3 of the        *
 * License, or (at your option) any later version.                           *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 *                                                                           *
 * Additional permission under GNU AGPL version 3 section 7                  *
 *                                                                           *
 * If you modify this program, or any covered work, by linking or            *
 * combining it with the OpenSSL project's OpenSSL library (or a             *
 * modified version of that library), containing parts covered by the        *
 * terms of the OpenSSL or SSLeay licenses, the authors of PokerTH           *
 * (Felix Hammer, Florian Thauer, Lothar May) grant you additional           *
 * permission to convey the resulting work.                                  *
 * Corresponding Source for a non-source form of such a combination          *
 * shall include the source code for the parts of OpenSSL used as well       *
 * as that of the covered work.                                              *
 *****************************************************************************/
#include "guiwrapper.h"
#include <QDebug>
#include <QtQml>
#include "session.h"
#include "game.h"
#include "playerinterface.h"
#include "log.h"
#include "configfile.h"
#include "manager.h"
#include "session.h"
#include "playermodel.h"
#include "game_defs.h"
#include "player.h"
#include "qmlgame.h"
#include "storeplayers.h"
#include <net/socket_msg.h>

using namespace std;


GuiWrapper::GuiWrapper()
{
    myManager = ManagerSingleton::Instance();
    myConfig = myManager->getConfig();
    myGame = new QmlGame();
}


GuiWrapper::~GuiWrapper()
{
    delete myGame;
}

void GuiWrapper::initGui(int speed)
{
    qDebug()<<"initGui()";
    assert(myManager->getSession());
    //set WindowTitle dynamically
    QString titleString = "";
    if(myManager->getSession()->getGameType() == Session::GAME_TYPE_INTERNET || myManager->getSession()->getGameType() == Session::GAME_TYPE_NETWORK) {
        GameInfo info(myManager->getSession()->getClientGameInfo(myManager->getSession()->getClientCurrentGameId()));
        titleString = QString::fromUtf8(info.name.c_str())+" - ";
    }
    myGame->settitle(QString(titleString + tr("PokerTH %1 - The Open-Source Texas Holdem Engine").arg(POKERTH_BETA_RELEASE_STRING)));

    //set speeds for local game and for first network game
    if( !myManager->getSession()->isNetworkClientRunning() || (myManager->getSession()->isNetworkClientRunning() && !myManager->getSession()->getCurrentGame()) ) {
        myGame->setgameSpeed(speed);
    }

    // TODO: is there an other way?
    //    myPlayerModel->addRows(myManager->getSession()->getCurrentGame()->getActivePlayerList()->size());
    emit myGame->guiInitiated(speed);
}

boost::shared_ptr<Session> GuiWrapper::getSession()
{
    return myManager->getSession();
}
void GuiWrapper::setSession(boost::shared_ptr<Session> /*session*/)
{
    /*myManager->setSession(session);*/
}

void GuiWrapper::refreshSet() const
{
    qDebug()<<"refreshSet()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        myGame->getPlayerModel()->at((*it_c)->getMyID())->setSet((*it_c)->getMySet());
    }
}
void GuiWrapper::refreshCash() const
{
    qDebug()<<"refreshCash()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        refreshStatus(*it_c);
        myGame->getPlayerModel()->at((*it_c)->getMyID())->setCash((*it_c)->getMyCash());
    }
}
void GuiWrapper::refreshAction(int playerID, int playerAction) const
{
    myGame->getPlayerModel()->at(playerID)->setAction(static_cast<PlayerAction>(playerAction));
}
void GuiWrapper::refreshActions() const
{
    qDebug()<<"refreshActions()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        refreshAction( (*it_c)->getMyID(), (*it_c)->getMyAction());
    }
}
void GuiWrapper::refreshPlayerAvatar() const
{
    qDebug()<<"refreshPlayerAvatar()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        setPlayerAvatar((*it_c)->getMyUniqueID(), (*it_c)->getMyAvatar());
    }
}
void GuiWrapper::refreshChangePlayer() const
{
    qDebug()<<"refreshChangePlayer()";
}
void GuiWrapper::refreshAll() const
{
    qDebug()<<"refreshAll()";
    refreshSet();
    refreshButton();
    refreshActions();
    refreshCash();
    refreshGroupbox();
    refreshPlayerName();
    refreshPlayerAvatar();
}
void GuiWrapper::refreshPot() const
{
    qDebug()<<"refreshPot()";
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    myGame->setbets(currentHand->getBoard()->getSets());
    myGame->setpot(currentHand->getBoard()->getPot());
}
void GuiWrapper::refreshGroupbox(int playerID, int status) const
{
    qDebug()<<"refreshGroupbox()";
    int j;

    if(playerID == -1 || status == -1) {

        boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();
        PlayerListConstIterator it_c;
        PlayerList seatsList = currentGame->getSeatsList();
        for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
            Player *tmp = myGame->getPlayerModel()->at((*it_c)->getMyID());

            tmp->setTurn((*it_c)->getMyTurn());
            tmp->setActiveStatus((*it_c)->getMyActiveStatus());
        }
    } else {
        Player *tmp = myGame->getPlayerModel()->at(playerID);
        switch(status) {

            //inactive
        case 0: {
            tmp->setActiveStatus(false);
        }
        break;
        //active but fold
        case 1: {
            tmp->setActiveStatus(true);
            tmp->setTurn(false);
//            tmp->setAction(PLAYER_ACTION_FOLD);
        }
        break;
        //active in action
        case 2:  {
            tmp->setActiveStatus(true);
            //Is below right?
            tmp->setTurn(true);
        }
        break;
        //active not in action
        case 3:  {
            tmp->setActiveStatus(true);
            //Is below right?
            tmp->setTurn(false);
        }
        break;
        default:
        {}
        }
    }
}
void GuiWrapper::refreshPlayerName() const
{
    qDebug()<<"refreshPlayerName()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        Player *tmp = myGame->getPlayerModel()->at((*it_c)->getMyID());
        QString name = QString::fromUtf8((*it_c)->getMyName().c_str());
        QString guid = QString::fromUtf8((*it_c)->getMyGuid().c_str());
        qDebug()<<"Name: "<<name<<" Guid: "<<guid;
        bool guest = myManager->getSession()->getClientPlayerInfo((*it_c)->getMyUniqueID()).isGuest;
        tmp->setButton((*it_c)->getMyButton());
        tmp->setGuest(guest);
        tmp->setPlayerType((*it_c)->getMyType());
        tmp->setName(name);
        tmp->setGuid(guid);
    }
}
void GuiWrapper::refreshButton() const
{
    qDebug()<<"refreshButton()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        myGame->getPlayerModel()->at((*it_c)->getMyID())->setButton((*it_c)->getMyButton());
    }
}
void GuiWrapper::refreshGameLabels(GameState state) const
{
    qDebug()<<"refreshGameLabels()";
}

void GuiWrapper::refreshStatus(boost::shared_ptr<PlayerInterface> player) const
{
    qDebug()<<"refreshStatus()";
    Player *tmp = myGame->getPlayerModel()->at(player->getMyID());
    tmp->setActiveStatus(player->getMyActiveStatus());
    tmp->setStayOnTableStatus(player->getMyStayOnTableStatus());
    tmp->setIsSessionActive(player->isSessionActive());
    tmp->setIsKicked(player->isKicked());
    tmp->setIsMuted(player->isMuted());
}

void GuiWrapper::setPlayerAvatar(int myUniqueID, const std::string &myAvatar) const
{
    qDebug()<<"setPlayerAvatar()";
    boost::shared_ptr<PlayerInterface> tmpPlayer = myManager->getSession()->getCurrentGame()->getPlayerByUniqueId(myUniqueID);
    QString myTmpAvatar = QString::fromUtf8(myAvatar.c_str());
    qDebug()<<"Avatar: "<<myTmpAvatar;
    if (tmpPlayer.get()) {
        QString countryString(QString(myManager->getSession()->getClientPlayerInfo(myUniqueID).countryCode.c_str()).toLower());
        Player* tmp = myGame->getPlayerModel()->at(tmpPlayer->getMyID());
        tmp->setCountry(countryString);

        QFile myAvatarFile(myTmpAvatar);
        if(myAvatarFile.exists()) {
            tmp->setAvatar(myTmpAvatar);
        } else {
            tmp->setAvatar(QString(""));
        }
    }
}


void GuiWrapper::waitForGuiUpdateDone() const
{
    qDebug()<<"waitForGuiUpdateDone()";
}

void GuiWrapper::dealBeRoCards(int myBeRoID)
{
    qDebug()<<"dealBeRoCards()";
    myGame->dealBeRoCards(myBeRoID);
}

void GuiWrapper::dealHoleCards()
{
    qDebug()<<"dealHoleCards()";
    myGame->dealHoleCards();
}
void GuiWrapper::dealFlopCards()
{
    qDebug()<<"dealFlopCards()";
    myGame->dealFlopCards0();
}
void GuiWrapper::dealTurnCard()
{
    qDebug()<<"dealTurnCard()";
    myGame->dealTurnCards0();
}
void GuiWrapper::dealRiverCard()
{
    qDebug()<<"dealRiverCard()";
    myGame->dealRiverCards0();
}

void GuiWrapper::nextPlayerAnimation()
{
    qDebug()<<"nextPlayerAnimation()";
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    //refresh Change Player
    refreshSet();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentHand->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        if((*it_c)->getMyID() == currentHand->getPreviousPlayerID()) break;
    }

    if(currentHand->getPreviousPlayerID() != -1) {
        refreshAction(currentHand->getPreviousPlayerID(), (*it_c)->getMyAction());
    }
    refreshCash();

    //refresh actions for human player
    updateMyButtonsState();

    emit myGame->nextPlayerAnimationSignal();
}

void GuiWrapper::beRoAnimation2(int myBeRoID)
{
    qDebug()<<"beRoAnimation2()";
    myGame->beRoAnimation2(myBeRoID);
}

void GuiWrapper::preflopAnimation1()
{
    qDebug()<<"preflopAnimation1()";
    myGame->preflopAnimation1();
}
void GuiWrapper::preflopAnimation2()
{
    qDebug()<<"preflopAnimation2()";
    myGame->preflopAnimation2();
}

void GuiWrapper::flopAnimation1()
{
    qDebug()<<"flopAnimation1()";
    myGame->flopAnimation1();
}
void GuiWrapper::flopAnimation2()
{
    qDebug()<<"flopAnimation2()";
    myGame->flopAnimation2();
}

void GuiWrapper::turnAnimation1()
{
    qDebug()<<"turnAnimation1()";
    myGame->turnAnimation1();
}
void GuiWrapper::turnAnimation2()
{
    qDebug()<<"turnAnimation2()";
    myGame->turnAnimation2();
}

void GuiWrapper::riverAnimation1()
{
    qDebug()<<"riverAnimation1()";
    myGame->riverAnimation1();
}
void GuiWrapper::riverAnimation2()
{
    qDebug()<<"riverAnimation2()";
    myGame->riverAnimation2();
}

void GuiWrapper::postRiverAnimation1()
{
    qDebug()<<"postRiverAnimation1()";
    myGame->postRiverAnimation1();
}
void GuiWrapper::postRiverRunAnimation1()
{
    qDebug()<<"postRiverRunAnimation1()";
    myGame->postRiverRunAnimation1();
}

void GuiWrapper::flipHolecardsAllIn()
{
    qDebug()<<"flipHolecardsAllIn()";
    myGame->flipHolecardsAllIn();
}

void GuiWrapper::handSwitchRounds()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->switchRounds();
}

void GuiWrapper::nextRoundCleanGui()
{
    qDebug()<<"nextRoundCleanGui()";
    refreshAll();
    myGame->nextRoundCleanGui();
}

void GuiWrapper::meInAction()
{
    qDebug()<<"meInAction()";
    myGame->meInAction();
}
void GuiWrapper::updateMyButtonsState()
{
    qDebug()<<"updateMyButtonsState()";
    myGame->updateMyButtonsState();
}
void GuiWrapper::disableMyButtons()
{
    qDebug()<<"disableMyButtons()";
    myGame->setbuttonsCheckable(false);
}
void GuiWrapper::startTimeoutAnimation(int playerNum, int timeoutSec)
{
    qDebug()<<"startTimeoutAnimation()";
}
void GuiWrapper::stopTimeoutAnimation(int playerNum)
{
    qDebug()<<"stopTimeoutAnimation()";
}

void GuiWrapper::startVoteOnKick(unsigned playerId, unsigned voteStarterPlayerId, int timeoutSec, int numVotesNeededToKick)
{
    qDebug()<<"startVoteOnKick()";
}
void GuiWrapper::changeVoteOnKickButtonsState(bool showHide)
{
    qDebug()<<"changeVoteOnKickButtonsState()";
}
void GuiWrapper::refreshVotesMonitor(int currentVotes, int numVotesNeededToKick)
{
    qDebug()<<"refreshVotesMonitor()";
}
void GuiWrapper::endVoteOnKick()
{
    qDebug()<<"endVoteOnKick()";
}

void GuiWrapper::logPlayerActionMsg(string playerName, int action, int setValue)
{
    qDebug()<<"logPlayerActionMsg()";
}
void GuiWrapper::logNewGameHandMsg(int gameID, int handID)
{
    qDebug()<<"logNewGameHandMsg()";
}
void GuiWrapper::logNewBlindsSetsMsg(int sbSet, int bbSet, std::string sbName, std::string bbName)
{
    qDebug()<<"logNewBlindsSetsMsg()";
}
void GuiWrapper::logPlayerWinsMsg(std::string playerName, int pot, bool main)
{
    qDebug()<<"logPlayerWinsMsg()";
}
void GuiWrapper::logPlayerSitsOut(std::string playerName)
{
    qDebug()<<"logPlayerSitsOut()";
}
void GuiWrapper::logDealBoardCardsMsg(int roundID, int card1, int card2, int card3, int card4, int card5)
{
    qDebug()<<"logDealBoardCardsMsg()";
}
void GuiWrapper::logFlipHoleCardsMsg(string playerName, int card1, int card2, int cardsValueInt, string showHas)
{
    qDebug()<<"logFlipHoleCardsMsg()";
}
void GuiWrapper::logPlayerWinGame(std::string playerName, int gameID)
{
    qDebug()<<"logPlayerWinGame()";
}
void GuiWrapper::flushLogAtGame(int gameID)
{
    qDebug()<<"flushLogAtGame()";
}
void GuiWrapper::flushLogAtHand()
{
    qDebug()<<"flushLogAtHand()";
}


void GuiWrapper::SignalNetClientConnect(int actionID)
{
    qDebug()<<"SignalNetClientConnect()";
}
void GuiWrapper::SignalNetClientServerListAdd(unsigned serverId)
{
    qDebug()<<"SignalNetClientServerListAdd()";
}
void GuiWrapper::SignalNetClientServerListShow()
{
    qDebug()<<"SignalNetClientServerListShow()";
}
void GuiWrapper::SignalNetClientLoginShow()
{
    qDebug()<<"SignalNetClientLoginShow()";
}
void GuiWrapper::SignalNetClientRejoinPossible(unsigned gameId)
{
    qDebug()<<"SignalNetClientRejoinPossible()";
}
void GuiWrapper::SignalNetClientPostRiverShowCards(unsigned playerId)
{
    qDebug()<<"SignalNetClientPostRiverShowCards()";
}
void GuiWrapper::SignalNetClientServerListClear()
{
    qDebug()<<"SignalNetClientServerListClear()";
}
void GuiWrapper::SignalNetClientGameInfo(int actionID)
{
    qDebug()<<"SignalNetClientGameInfo()";
}
void GuiWrapper::SignalNetClientError(int errorID, int osErrorID)
{
    qDebug()<<"SignalNetClientError()";
}
void GuiWrapper::SignalNetClientNotification(int notificationId)
{
    qDebug()<<"SignalNetClientNotification()";
}
void GuiWrapper::SignalNetClientStatsUpdate(const ServerStats &stats)
{
    qDebug()<<"SignalNetClientStatsUpdate()";
}
void GuiWrapper::SignalNetClientShowTimeoutDialog(NetTimeoutReason reason, unsigned remainingSec)
{
    qDebug()<<"SignalNetClientShowTimeoutDialog()";
}
void GuiWrapper::SignalNetClientRemovedFromGame(int notificationId)
{
    qDebug()<<"SignalNetClientRemovedFromGame()";
}
void GuiWrapper::SignalNetClientSelfJoined(unsigned playerId, const string &playerName, bool isGameAdmin)
{
    qDebug()<<"SignalNetClientSelfJoined()";
}
void GuiWrapper::SignalNetClientPlayerJoined(unsigned playerId, const string &playerName, bool isGameAdmin)
{
    qDebug()<<"SignalNetClientPlayerJoined()";
}
void GuiWrapper::SignalNetClientPlayerChanged(unsigned playerId, const string &newPlayerName)
{
    qDebug()<<"SignalNetClientPlayerChanged()";
}
void GuiWrapper::SignalNetClientPlayerLeft(unsigned playerId, const string &playerName, int removeReason)
{
    qDebug()<<"SignalNetClientPlayerLeft()";
}
void GuiWrapper::SignalNetClientSpectatorJoined(unsigned playerId, const string &playerName)
{
    qDebug()<<"SignalNetClientSpectatorJoined()";
}
void GuiWrapper::SignalNetClientSpectatorLeft(unsigned playerId, const string &playerName, int removeReason)
{
    qDebug()<<"SignalNetClientSpectatorLeft()";
}
void GuiWrapper::SignalNetClientNewGameAdmin(unsigned playerId, const string &playerName)
{
    qDebug()<<"SignalNetClientNewGameAdmin()";
}

void GuiWrapper::SignalNetClientGameListNew(unsigned gameId)
{
    qDebug()<<"SignalNetClientGameListNew()";
}
void GuiWrapper::SignalNetClientGameListRemove(unsigned gameId)
{
    qDebug()<<"SignalNetClientGameListRemove()";
}
void GuiWrapper::SignalNetClientGameListUpdateMode(unsigned gameId, GameMode mode)
{
    qDebug()<<"SignalNetClientGameListUpdateMode()";
}
void GuiWrapper::SignalNetClientGameListUpdateAdmin(unsigned gameId, unsigned adminPlayerId)
{
    qDebug()<<"SignalNetClientGameListUpdateAdmin()";
}
void GuiWrapper::SignalNetClientGameListPlayerJoined(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListPlayerJoined()";
}
void GuiWrapper::SignalNetClientGameListPlayerLeft(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListPlayerLeft()";
}
void GuiWrapper::SignalNetClientGameListSpectatorJoined(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListSpectatorJoined()";
}
void GuiWrapper::SignalNetClientGameListSpectatorLeft(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListSpectatorLeft()";
}
void GuiWrapper::SignalNetClientGameStart(boost::shared_ptr<Game> game)
{
    qDebug()<<"SignalNetClientGameStart()";
}

void GuiWrapper::SignalNetClientWaitDialog()
{
    qDebug()<<"SignalNetClientWaitDialog()";
}
void GuiWrapper::SignalNetClientGameChatMsg(const string &playerName, const string &msg)
{
    qDebug()<<"SignalNetClientGameChatMsg()";
}
void GuiWrapper::SignalNetClientLobbyChatMsg(const string &playerName, const string &msg)
{
    qDebug()<<"SignalNetClientLobbyChatMsg()";
}
void GuiWrapper::SignalNetClientPrivateChatMsg(const std::string &playerName, const std::string &msg)
{
    qDebug()<<"SignalNetClientPrivateChatMsg()";
}
void GuiWrapper::SignalNetClientMsgBox(const string &msg)
{
    qDebug()<<"SignalNetClientMsgBox()";
}
void GuiWrapper::SignalNetClientMsgBox(unsigned msgId)
{
    qDebug()<<"SignalNetClientMsgBox()";
}

void GuiWrapper::SignalNetServerSuccess(int /*actionID*/) {
    qDebug()<<"SignalNetServerSuccess()";
}
void GuiWrapper::SignalNetServerError(int errorID, int osErrorID)
{
    qDebug()<<"SignalNetServerError()";
}

void GuiWrapper::SignalIrcConnect(const string &/*server*/)
{
    qDebug()<<"SignalIrcConnect()";
    /*myManager->signalIrcConnect(QString::fromUtf8(server.c_str()));*/
}
void GuiWrapper::SignalIrcSelfJoined(const string &/*nickName*/, const string &/*channel*/)
{
    qDebug()<<"SignalIrcSelfJoined()";
    /*myManager->signalIrcSelfJoined(QString::fromUtf8(nickName.c_str()), QString::fromUtf8(channel.c_str()));*/
}
void GuiWrapper::SignalIrcPlayerJoined(const string &/*nickName*/)
{
    qDebug()<<"SignalIrcPlayerJoined()";
    /*myManager->signalIrcPlayerJoined(QString::fromUtf8(nickName.c_str()));*/
}
void GuiWrapper::SignalIrcPlayerChanged(const string &/*oldNick*/, const string &/*newNick*/)
{
    qDebug()<<"SignalIrcPlayerChanged()";
    /* myManager->signalIrcPlayerChanged(QString::fromUtf8(oldNick.c_str()), QString::fromUtf8(newNick.c_str()));*/
}
void GuiWrapper::SignalIrcPlayerKicked(const std::string &/*nickName*/, const std::string &/*byWhom*/, const std::string &/*reason*/)
{
    qDebug()<<"SignalIrcPlayerKicked()";
    /*myManager->signalIrcPlayerKicked(QString::fromUtf8(nickName.c_str()), QString::fromUtf8(byWhom.c_str()), QString::fromUtf8(reason.c_str()));*/
}
void GuiWrapper::SignalIrcPlayerLeft(const std::string &/*nickName*/)
{
    qDebug()<<"SignalIrcPlayerLeft()";
    /*myManager->signalIrcPlayerLeft(QString::fromUtf8(nickName.c_str()));*/
}
void GuiWrapper::SignalIrcChatMsg(const std::string &/*nickName*/, const std::string &/*msg*/)
{
    qDebug()<<"SignalIrcChatMsg()";
    /*myManager->signalIrcChatMessage(QString::fromUtf8(nickName.c_str()), QString::fromUtf8(msg.c_str()));*/
}
void GuiWrapper::SignalIrcError(int /*errorCode*/)
{
    qDebug()<<"SignalIrcError()";
    /*myManager->signalIrcError(errorCode);*/
}
void GuiWrapper::SignalIrcServerError(int /*errorCode*/)
{
    qDebug()<<"SignalIrcServerError()";
    /*myManager->signalIrcServerError(errorCode);*/
}

void GuiWrapper::SignalLobbyPlayerJoined(unsigned playerId, const string &nickName)
{
    qDebug()<<"SignalLobbyPlayerJoined()";
}
void GuiWrapper::SignalLobbyPlayerKicked(const std::string &nickName, const std::string &byWhom, const std::string &reason)
{
    qDebug()<<"SignalLobbyPlayerKicked()";
}
void GuiWrapper::SignalLobbyPlayerLeft(unsigned playerId)
{
    qDebug()<<"SignalLobbyPlayerLeft()";
}

void GuiWrapper::SignalSelfGameInvitation(unsigned gameId, unsigned playerIdFrom)
{
    qDebug()<<"SignalSelfGameInvitation()";
}
void GuiWrapper::SignalPlayerGameInvitation(unsigned gameId, unsigned playerIdWho, unsigned playerIdFrom)
{
    qDebug()<<"SignalPlayerGameInvitation()";
}
void GuiWrapper::SignalRejectedGameInvitation(unsigned gameId, unsigned playerIdWho, DenyGameInvitationReason reason)
{
    qDebug()<<"SignalRejectedGameInvitation()";
}
