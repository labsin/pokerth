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
#include "qmlplayer.h"
#include "qmlgame.h"
#include "qmlserver.h"
#include "playermodel.h"
#include <net/socket_msg.h>

using namespace std;


GuiWrapper::GuiWrapper()
{
    myManager = ManagerSingleton::Instance();
    myConfig = myManager->getConfig();
    myGame = myManager->getGame();
    myServer = myManager->getServer();
    emit myManager->afterInit();
}


GuiWrapper::~GuiWrapper()
{
}

void GuiWrapper::initGui(int speed)
{
    qDebug()<<"initGui()";
    emit myGame->initGui(speed);
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
    emit myGame->signalRefreshSet();
}
void GuiWrapper::refreshCash() const
{
    emit myGame->signalRefreshCash();
}
void GuiWrapper::refreshAction(int playerID, int playerAction) const
{
    emit myGame->signalRefreshAction(playerID, playerAction);
}
void GuiWrapper::refreshChangePlayer() const
{
    qDebug()<<"refreshChangePlayer()";
    emit myGame->signalRefreshChangePlayer();

}
void GuiWrapper::refreshAll() const
{
    qDebug()<<"refreshAll()";
    emit myGame->signalRefreshAll();
}
void GuiWrapper::refreshPot() const
{
    qDebug()<<"refreshPot()";
    emit myGame->signalRefreshPot();
}
void GuiWrapper::refreshGroupbox(int playerID, int status) const
{
    qDebug()<<"refreshGroupbox()";
    emit myGame->signalRefreshGroupbox(playerID, status);
}
void GuiWrapper::refreshPlayerName() const
{
    qDebug()<<"refreshPlayerName()";
    emit myGame->signalRefreshPlayerName();
}
void GuiWrapper::refreshButton() const
{
    qDebug()<<"refreshButton()";
    emit myGame->signalRefreshButton();
}
void GuiWrapper::refreshGameLabels(GameState gameState) const
{
    qDebug()<<"refreshGameLabels()";
    emit myGame->signalRefreshGameLabels(static_cast<int>(gameState));
}
void GuiWrapper::setPlayerAvatar(int myUniqueID, const std::string &myAvatar) const
{
    qDebug()<<"setPlayerAvatar()";
    emit myGame->signalSetPlayerAvatar(myUniqueID, QString::fromStdString(myAvatar));
}

void GuiWrapper::waitForGuiUpdateDone() const
{
    qDebug()<<"waitForGuiUpdateDone()";
    emit myGame->signalGuiUpdateDone();
    emit myGame->waitForGuiUpdateDone();
}

void GuiWrapper::dealBeRoCards(int myBeRoID)
{
    qDebug()<<"dealBeRoCards()";
    emit myGame->signalDealBeRoCards(myBeRoID);
}

void GuiWrapper::dealHoleCards()
{
    qDebug()<<"dealHoleCards()";
    emit myGame->signalDealHoleCards();
}
void GuiWrapper::dealFlopCards()
{
    qDebug()<<"dealFlopCards()";
    emit myGame->signalDealFlopCards();
}
void GuiWrapper::dealTurnCard()
{
    qDebug()<<"dealTurnCard()";
    emit myGame->signalDealTurnCards();
}
void GuiWrapper::dealRiverCard()
{
    qDebug()<<"dealRiverCard()";
    emit myGame->signalDealRiverCards();
}

void GuiWrapper::nextPlayerAnimation()
{
    qDebug()<<"nextPlayerAnimation()";
    emit myGame->signalNextPlayerAnimation();
}

void GuiWrapper::beRoAnimation2(int myBeRoID)
{
    qDebug()<<"beRoAnimation2()";
    emit myGame->signalBeRoAnimation2(myBeRoID);
}

void GuiWrapper::preflopAnimation1()
{
    qDebug()<<"preflopAnimation1()";
    emit myGame->signalPreflopAnimation1();
}
void GuiWrapper::preflopAnimation2()
{
    qDebug()<<"preflopAnimation2()";
    emit myGame->signalPreflopAnimation2();
}

void GuiWrapper::flopAnimation1()
{
    qDebug()<<"flopAnimation1()";
    emit myGame->signalFlopAnimation1();
}
void GuiWrapper::flopAnimation2()
{
    qDebug()<<"flopAnimation2()";
    emit myGame->signalFlopAnimation2();
}

void GuiWrapper::turnAnimation1()
{
    qDebug()<<"turnAnimation1()";
    emit myGame->signalTurnAnimation1();
}
void GuiWrapper::turnAnimation2()
{
    qDebug()<<"turnAnimation2()";
    emit myGame->signalTurnAnimation2();
}

void GuiWrapper::riverAnimation1()
{
    qDebug()<<"riverAnimation1()";
    emit myGame->signalRiverAnimation1();
}
void GuiWrapper::riverAnimation2()
{
    qDebug()<<"riverAnimation2()";
    emit myGame->signalRiverAnimation2();
}

void GuiWrapper::postRiverAnimation1()
{
    qDebug()<<"postRiverAnimation1()";
    emit myGame->signalPostRiverAnimation1();
}
void GuiWrapper::postRiverRunAnimation1()
{
    qDebug()<<"postRiverRunAnimation1()";
    emit myGame->signalPostRiverRunAnimation1();
}

void GuiWrapper::flipHolecardsAllIn()
{
    qDebug()<<"flipHolecardsAllIn()";
    emit myGame->signalFlipHolecardsAllIn();
}

void GuiWrapper::handSwitchRounds()
{
    emit myGame->signalHandSwitchRounds();
}

void GuiWrapper::nextRoundCleanGui()
{
    qDebug()<<"nextRoundCleanGui()";
    refreshAll();
    emit myGame->signalNextRoundCleanGui();
}

void GuiWrapper::meInAction()
{
    qDebug()<<"meInAction()";
    emit myGame->signalMeInAction();
}
void GuiWrapper::updateMyButtonsState()
{
    qDebug()<<"updateMyButtonsState()";
    emit myGame->signalUpdateMyButtonsState();
}
void GuiWrapper::disableMyButtons()
{
    qDebug()<<"disableMyButtons()";
    emit myGame->signalDisableMyButtons();
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
    emit myServer->SignalNetClientConnect(actionID);
}
void GuiWrapper::SignalNetClientServerListClear()
{
    qDebug()<<"SignalNetClientServerListClear()";
    emit myManager->SignalNetClientServerListClear();
}
void GuiWrapper::SignalNetClientServerListAdd(unsigned serverId)
{
    qDebug()<<"SignalNetClientServerListAdd()";
    emit myManager->SignalNetClientServerListAdd(serverId);
}
void GuiWrapper::SignalNetClientServerListShow()
{
    qDebug()<<"SignalNetClientServerListShow()";
    emit myServer->SignalNetClientServerListShow();
}
void GuiWrapper::SignalNetClientLoginShow()
{
    qDebug()<<"SignalNetClientLoginShow()";
    emit myServer->loginNeeded();
}
void GuiWrapper::SignalNetClientRejoinPossible(unsigned gameId)
{
    qDebug()<<"SignalNetClientRejoinPossible()";
}
void GuiWrapper::SignalNetClientPostRiverShowCards(unsigned playerId)
{
    qDebug()<<"SignalNetClientPostRiverShowCards()";
}
void GuiWrapper::SignalNetClientGameInfo(int actionID)
{
    qDebug()<<"SignalNetClientGameInfo()";
}
void GuiWrapper::SignalNetClientError(int errorID, int osErrorID)
{
    qDebug()<<"SignalNetClientError(): "<<errorID;
    emit myManager->getServer()->networkError(errorID, osErrorID);
}
void GuiWrapper::SignalNetClientNotification(int notificationId)
{
    qDebug()<<"SignalNetClientNotification(): "<<notificationId;
    emit myServer->networkNotification(notificationId);
}
void GuiWrapper::SignalNetClientStatsUpdate(const ServerStats &stats)
{
    qDebug()<<"SignalNetClientStatsUpdate()";
    emit myServer->SignalNetClientStatsUpdate(stats);
}
void GuiWrapper::SignalNetClientShowTimeoutDialog(NetTimeoutReason reason, unsigned remainingSec)
{
    qDebug()<<"SignalNetClientShowTimeoutDialog()";
}
void GuiWrapper::SignalNetClientPingUpdate(unsigned minPing, unsigned avgPing, unsigned maxPing)
{
    qDebug()<<"SignalNetClientPingUpdate()";
}

void GuiWrapper::SignalNetClientRemovedFromGame(int notificationId)
{
    qDebug()<<"SignalNetClientRemovedFromGame(): "<<notificationId;
    emit myServer->SignalNetClientRemovedFromGame(notificationId);
    emit myServer->networkNotification(notificationId);
}
void GuiWrapper::SignalNetClientSelfJoined(unsigned playerId, const string &playerName, bool isGameAdmin)
{
    qDebug()<<"SignalNetClientSelfJoined()";
    emit myServer->SignalNetClientSelfJoined(playerId, QString::fromStdString(playerName), isGameAdmin);
}
void GuiWrapper::SignalNetClientPlayerJoined(unsigned playerId, const string &playerName, bool isGameAdmin)
{
    qDebug()<<"SignalNetClientPlayerJoined()";
    emit myServer->SignalNetClientPlayerJoined(playerId, QString::fromStdString(playerName), isGameAdmin);
}
void GuiWrapper::SignalNetClientPlayerChanged(unsigned playerId, const string &newPlayerName)
{
    qDebug()<<"SignalNetClientPlayerChanged()";
    emit myServer->SignalNetClientPlayerChanged(playerId, QString::fromStdString(newPlayerName));
}
void GuiWrapper::SignalNetClientPlayerLeft(unsigned playerId, const string &playerName, int removeReason)
{
    qDebug()<<"SignalNetClientPlayerLeft()";
    emit myServer->SignalNetClientPlayerLeft(playerId, QString::fromStdString(playerName), removeReason);
}
void GuiWrapper::SignalNetClientSpectatorJoined(unsigned playerId, const string &playerName)
{
    qDebug()<<"SignalNetClientSpectatorJoined()";
    emit myServer->SignalNetClientSpectatorJoined(playerId, QString::fromStdString(playerName));
}
void GuiWrapper::SignalNetClientSpectatorLeft(unsigned playerId, const string &playerName, int removeReason)
{
    qDebug()<<"SignalNetClientSpectatorLeft()";
    emit myServer->SignalNetClientSpectatorLeft(playerId, QString::fromStdString(playerName), removeReason);
}
void GuiWrapper::SignalNetClientNewGameAdmin(unsigned playerId, const string &playerName)
{
    qDebug()<<"SignalNetClientNewGameAdmin()";
    emit myServer->SignalNetClientNewGameAdmin(playerId, QString::fromStdString(playerName));
}

void GuiWrapper::SignalNetClientGameListNew(unsigned gameId)
{
    qDebug()<<"SignalNetClientGameListNew()";
    emit myServer->SignalNetClientGameListNew(gameId);
}
void GuiWrapper::SignalNetClientGameListRemove(unsigned gameId)
{
    qDebug()<<"SignalNetClientGameListRemove()";
    emit myServer->SignalNetClientGameListRemove(gameId);
}
void GuiWrapper::SignalNetClientGameListUpdateMode(unsigned gameId, GameMode mode)
{
    qDebug()<<"SignalNetClientGameListUpdateMode()";
    emit myServer->SignalNetClientGameListUpdateMode(gameId);
}
void GuiWrapper::SignalNetClientGameListUpdateAdmin(unsigned gameId, unsigned adminPlayerId)
{
    qDebug()<<"SignalNetClientGameListUpdateAdmin()";
    emit myServer->SignalNetClientGameListUpdateAdmin(gameId, adminPlayerId);
}
void GuiWrapper::SignalNetClientGameListPlayerJoined(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListPlayerJoined()";
    emit myServer->SignalNetClientGameListPlayerJoined(gameId, playerId);
}
void GuiWrapper::SignalNetClientGameListPlayerLeft(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListPlayerLeft()";
    emit myServer->SignalNetClientGameListPlayerLeft(gameId, playerId);
}
void GuiWrapper::SignalNetClientGameListSpectatorJoined(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListSpectatorJoined()";
    emit myServer->SignalNetClientGameListSpectatorJoined(gameId, playerId);
}
void GuiWrapper::SignalNetClientGameListSpectatorLeft(unsigned gameId, unsigned playerId)
{
    qDebug()<<"SignalNetClientGameListSpectatorLeft()";
    emit myServer->SignalNetClientGameListSpectatorLeft(gameId, playerId);
}
void GuiWrapper::SignalNetClientGameStart(boost::shared_ptr<Game> game)
{
    qDebug()<<"SignalNetClientGameStart()";
    myManager->getSession()->startClientGame(game);
}

void GuiWrapper::SignalNetClientWaitDialog()
{
    qDebug()<<"SignalNetClientWaitDialog()";
}
void GuiWrapper::SignalLobbyPlayerJoined(unsigned playerId, const string &nickName)
{
    qDebug()<<"SignalLobbyPlayerJoined()";
    emit myServer->SignalLobbyPlayerJoined(playerId, QString::fromStdString(nickName));
}
void GuiWrapper::SignalLobbyPlayerKicked(const std::string &nickName, const std::string &byWhom, const std::string &reason)
{
    qDebug()<<"SignalLobbyPlayerKicked()";
}
void GuiWrapper::SignalLobbyPlayerLeft(unsigned playerId)
{
    qDebug()<<"SignalLobbyPlayerLeft()";
    emit myServer->SignalLobbyPlayerLeft(playerId);
}

//Chat
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

//ServerError
void GuiWrapper::SignalNetServerSuccess(int /*actionID*/) {
    qDebug()<<"SignalNetServerSuccess()";
}
void GuiWrapper::SignalNetServerError(int errorID, int osErrorID)
{
    qDebug()<<"SignalNetServerError()"<<errorID;
    emit myManager->getServer()->networkError(errorID, osErrorID);
}

// IRC not used
void GuiWrapper::SignalIrcConnect(const string &/*server*/)
{
    qDebug()<<"SignalIrcConnect()";
}
void GuiWrapper::SignalIrcSelfJoined(const string &/*nickName*/, const string &/*channel*/)
{
    qDebug()<<"SignalIrcSelfJoined()";
}
void GuiWrapper::SignalIrcPlayerJoined(const string &/*nickName*/)
{
    qDebug()<<"SignalIrcPlayerJoined()";
}
void GuiWrapper::SignalIrcPlayerChanged(const string &/*oldNick*/, const string &/*newNick*/)
{
    qDebug()<<"SignalIrcPlayerChanged()";
}
void GuiWrapper::SignalIrcPlayerKicked(const std::string &/*nickName*/, const std::string &/*byWhom*/, const std::string &/*reason*/)
{
    qDebug()<<"SignalIrcPlayerKicked()";
}
void GuiWrapper::SignalIrcPlayerLeft(const std::string &/*nickName*/)
{
    qDebug()<<"SignalIrcPlayerLeft()";
}
void GuiWrapper::SignalIrcChatMsg(const std::string &/*nickName*/, const std::string &/*msg*/)
{
    qDebug()<<"SignalIrcChatMsg()";
}
void GuiWrapper::SignalIrcError(int /*errorCode*/)
{
    qDebug()<<"SignalIrcError()";
}
void GuiWrapper::SignalIrcServerError(int /*errorCode*/)
{
    qDebug()<<"SignalIrcServerError()";
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
