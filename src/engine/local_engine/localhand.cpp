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

#include "localhand.h"
#include "tools.h"
#include "cardsvalue.h"
#include <game_defs.h>
#include <core/loghelper.h>

#include "localexception.h"
#include "engine_msg.h"

#include <iostream>

using namespace std;

LocalHand::LocalHand(boost::shared_ptr<EngineFactory> f, GuiInterface *g, boost::shared_ptr<BoardInterface> b, Log *l, PlayerList sl, PlayerList apl, PlayerList rpl, int id, int sP, unsigned dP, int sB,int sC)
	: myFactory(f), myGui(g),  myBoard(b), myLog(l), seatsList(sl), activePlayerList(apl), runningPlayerList(rpl), myBeRo(0), myID(id), startQuantityPlayers(sP), dealerPosition(dP), smallBlindPosition(dP), bigBlindPosition(dP), currentRound(GAME_STATE_PREFLOP), roundBeforePostRiver(GAME_STATE_PREFLOP), smallBlind(sB), startCash(sC), previousPlayerID(-1), lastActionPlayerID(0), allInCondition(false),
	  cardsShown(false)
{

	int i, j, k;
	PlayerListIterator it;

	for(it=seatsList->begin(); it!=seatsList->end(); ++it) {
		(*it)->setHand(this);
		// set myFlipCards 0
		(*it)->setMyHoleCardsFlip(0, 0);
	}

	// generate cards and assign to board and player
	const int NumCards = 52;
	int cards[NumCards] = {
		0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
		50, 51
	};
	Tools::ShuffleArrayNonDeterministic(cards, NumCards);
	int boardCards[5];
	int playerHoleCards[2];
	int playerCards[7];
	int sBluff;

	// board cards
	for(i=0; i<5; i++) boardCards[i] = cards[i];

	// debug mode
	if(myLog) myLog->debugMode_getBoardCards(boardCards,myID);

	// prepare whole player hand
	for(i=0; i<5; i++) playerCards[i+2] = boardCards[i];

	k = 0;
	myBoard->setMyCards(boardCards);
	for(it=activePlayerList->begin(); it!=activePlayerList->end(); ++it, k++) {

		// hole cards
		for(j=0; j<2; j++) playerHoleCards[j] = cards[2*k+j+5];
		(*it)->setMyHoleCards(playerHoleCards);
		if(myLog) myLog->debugMode_getPlayerCards(playerHoleCards,myID,k); // debug mode

		// complete whole player hand
		for(j=0; j<2; j++) playerCards[j] = playerHoleCards[j];

		// determine cards value
		int bestHand[4] = { 0,0,0,0 };
		int playerCardsColor[4] = { 0,0,0,0 };
		for(j=0; j<7; j++) playerCardsColor[playerCards[j]/13] |= (1 << playerCards[j]%13);
		(*it)->setMyCardsValueInt(CardsValue::cardsValue(playerCardsColor,bestHand));

		// determine position of best 5 cards
		int bestHandPos[5];
		if(CardsValue::bestHandToPosition(bestHand,playerCards,bestHandPos)) {
			(*it)->setMyBestHandPosition(bestHandPos);
		} else {
			LOG_ERROR(__FILE__ << " (" << __LINE__ << "): ERROR getMyBestHandPosition");
		}

		// set round start cash
		(*it)->setMyRoundStartCash((*it)->getMyCash());

		// set sBluff for all players --> TODO for ai-player in internet
		if((*it)->getMyID() != 0) {
			Tools::GetRand(1, 100, 1, &sBluff);
			(*it)->setSBluff(sBluff);
			(*it)->setSBluffStatus(0);
		}
	}

	// determine dealer, small blind, big blind
	assignButtons();
	setBlinds();

	if(myLog) myLog->logNewHandMsg(myID, dealerPosition+1, smallBlind, smallBlindPosition+1, 2*smallBlind, bigBlindPosition+1, seatsList);

	myBeRo = myFactory->createBeRo(this, dealerPosition, smallBlind);
}



LocalHand::~LocalHand()
{
}

void LocalHand::start()
{

	//Log blinds sets for new Hand
	PlayerListConstIterator it_sB, it_bB;
	it_sB = getActivePlayerIt(getCurrentBeRo()->getSmallBlindPositionId());
	it_bB = getActivePlayerIt(getCurrentBeRo()->getBigBlindPositionId());
	if(it_sB != getActivePlayerList()->end() && it_bB != getActivePlayerList()->end()) {
		myGui->logNewBlindsSetsMsg((*it_sB)->getMySet(), (*it_bB)->getMySet(), (*it_sB)->getMyName().c_str(), (*it_bB)->getMyName().c_str());
	} else {
		LOG_ERROR(__FILE__ << " (" << __LINE__ << "): Log Error: cannot find sBID or bBID");
	}

	// deal cards
	myGui->dealHoleCards();

	getBoard()->collectSets();
	getGuiInterface()->refreshPot();

	// change rounds | first start preflop
	myGui->nextPlayerAnimation();
}

void LocalHand::assignButtons()
{

	PlayerListIterator seatIt;

	// delete all buttons
	for (seatIt=seatsList->begin(); seatIt!=seatsList->end(); ++seatIt) {
		(*seatIt)->setMyButton(BUTTON_NONE);
	}

	// assign dealer button
	seatIt = getSeatIt(dealerPosition);
	if(seatIt == seatsList->end()) {
		throw LocalException(__FILE__, __LINE__, ERR_SEAT_NOT_FOUND);
	}
	(*seatIt)->setMyButton(BUTTON_DEALER);


	// assign Small Blind next to dealer. ATTENTION: in heads up it is big blind
	// assign big blind next to small blind. ATTENTION: in heads up it is small blind
	bool nextActivePlayerFound = false;
	PlayerListIterator dealerPositionIt = getSeatIt(dealerPosition);
	if(dealerPositionIt == seatsList->end()) {
		throw LocalException(__FILE__, __LINE__, ERR_SEAT_NOT_FOUND);
	}

	for(size_t i=0; i<seatsList->size(); i++) {

		++dealerPositionIt;
		if(dealerPositionIt == seatsList->end()) dealerPositionIt = seatsList->begin();

		PlayerListIterator activePlayerIt = getActivePlayerIt( (*dealerPositionIt)->getMyUniqueID() );
		if(activePlayerIt != activePlayerList->end() ) {
			nextActivePlayerFound = true;
			if(activePlayerList->size() > 2) {
				//small blind normal
				(*activePlayerIt)->setMyButton(2);
				smallBlindPosition = (*activePlayerIt)->getMyUniqueID();
			} else {
				//big blind in heads up
				(*activePlayerIt)->setMyButton(3);
				bigBlindPosition = (*activePlayerIt)->getMyUniqueID();
				// lastPlayerAction for showing cards
			}

			// first player after dealer have to show his cards first (in showdown)
			lastActionPlayerID = (*activePlayerIt)->getMyUniqueID();
			myBoard->setLastActionPlayerID(lastActionPlayerID);

			++activePlayerIt;
			if(activePlayerIt == activePlayerList->end()) activePlayerIt = activePlayerList->begin();

			if(activePlayerList->size() > 2) {
				//big blind normal
				(*activePlayerIt)->setMyButton(3);
				bigBlindPosition = (*activePlayerIt)->getMyUniqueID();
			} else {
				//small blind in heads up
				(*activePlayerIt)->setMyButton(2);
				smallBlindPosition = (*activePlayerIt)->getMyUniqueID();
			}

			break;
		}

	}
	if(!nextActivePlayerFound) {
		throw LocalException(__FILE__, __LINE__, ERR_NEXT_ACTIVE_PLAYER_NOT_FOUND);
	}

//        cout << "lAP-Button: " << lastActionPlayer << endl;
}

void LocalHand::setBlinds()
{

	PlayerListConstIterator it_c;

	//do sets --> TODO switch?
	for (it_c=runningPlayerList->begin(); it_c!=runningPlayerList->end(); ++it_c) {

		//small blind
		if((*it_c)->getMyButton() == BUTTON_SMALL_BLIND) {

			// All in ?
			if((*it_c)->getMyCash() <= smallBlind) {

				(*it_c)->setMySet((*it_c)->getMyCash());
				// 1 to do not log this
				(*it_c)->setMyAction(PLAYER_ACTION_ALLIN);

			} else {
				(*it_c)->setMySet(smallBlind);
			}
		}

	}

	//do sets --> TODO switch?
	for (it_c=runningPlayerList->begin(); it_c!=runningPlayerList->end(); ++it_c) {

		//big blind
		if((*it_c)->getMyButton() == BUTTON_BIG_BLIND) {

			// all in ?
			if((*it_c)->getMyCash() <= 2*smallBlind) {

				(*it_c)->setMySet((*it_c)->getMyCash());
				// 1 to do not log this
				(*it_c)->setMyAction(PLAYER_ACTION_ALLIN);

			} else {
				(*it_c)->setMySet(2*smallBlind);
			}
		}
	}
}


void LocalHand::switchRounds()
{

	// logging last player action
	PlayerListConstIterator previousPlayerIt = getRunningPlayerIt(previousPlayerID);
	if(previousPlayerIt != runningPlayerList->end()) {
		if(myLog) myLog->logPlayerAction((*previousPlayerIt)->getMyName(),myLog->transformPlayerActionLog((*previousPlayerIt)->getMyAction()),(*previousPlayerIt)->getMyLastRelativeSet());
	}

	PlayerListIterator it, it_1;
	PlayerListConstIterator it_c;

	// refresh runningPlayerList
	for(it=runningPlayerList->begin(); it!=runningPlayerList->end(); ) {
		if((*it)->getMyAction() == PLAYER_ACTION_FOLD || (*it)->getMyAction() == PLAYER_ACTION_ALLIN) {

			it = runningPlayerList->erase(it);
			if(!(runningPlayerList->empty())) {

				it_1 = it;
				if(it_1 == runningPlayerList->begin()) it_1 = runningPlayerList->end();
				--it_1;
				getCurrentBeRo()->setCurrentPlayersTurnId((*it_1)->getMyUniqueID());

			}
		} else {
			++it;
		}
	}

	// determine number of all in players
	int allInPlayersCounter = 0;
	for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
		if ((*it_c)->getMyAction() == PLAYER_ACTION_ALLIN) allInPlayersCounter++;
	}

	// determine number of non-fold players
	int nonFoldPlayerCounter = 0;
	for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
		if ((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) nonFoldPlayerCounter++;
	}

	// if only one player non-fold -> distribute pot
	if(nonFoldPlayerCounter==1) {
		myBoard->collectPot();
		myGui->refreshPot();
		myGui->refreshSet();
		currentRound = GAME_STATE_POST_RIVER;
		if(myLog) myLog->setCurrentRound(currentRound);
	}

	// check for all in condition
	// for all in condition at least two active players have to remain
	else {

		// 1) all players all in
		if(allInPlayersCounter == nonFoldPlayerCounter) {
			allInCondition = true;
			myBoard->setAllInCondition(true);
		}

		// 2) all players but one all in and he has highest set
		if(allInPlayersCounter+1 == nonFoldPlayerCounter) {

			for(it_c=runningPlayerList->begin(); it_c!=runningPlayerList->end(); ++it_c) {

				if((*it_c)->getMySet() >= myBeRo[currentRound]->getHighestSet()) {
					allInCondition = true;
					myBoard->setAllInCondition(true);
				}

			}

			// exception
			// no.1: if in first Preflop Round next player is small blind and only all-in-big-blind with less than smallblind amount and other all-in players with less than small blind are nonfold too -> preflop is over
			PlayerListConstIterator smallBlindIt_c = getRunningPlayerIt(myBeRo[currentRound]->getSmallBlindPositionId());
			PlayerListConstIterator bigBlindIt_c = getActivePlayerIt(myBeRo[currentRound]->getBigBlindPositionId());
			if(smallBlindIt_c!=runningPlayerList->end() &&  bigBlindIt_c!=activePlayerList->end() && currentRound == GAME_STATE_PREFLOP && myBeRo[currentRound]->getFirstRound()) {
				// determine player who are all in with less than small blind amount
				int tempCounter = 0;
				for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
					if((*it_c)->getMyAction() == PLAYER_ACTION_ALLIN && (*it_c)->getMySet() <= smallBlind) {
						tempCounter++;
					}
				}
				if( (*bigBlindIt_c)->getMySet() <= smallBlind  && tempCounter == allInPlayersCounter) {
					allInCondition = true;
					myBoard->setAllInCondition(true);
				}
			}

			// no.2: heads up -> detect player who is all in and bb but could set less than sb
			for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {

				if(activePlayerList->size()==2 && (*it_c)->getMyAction() == PLAYER_ACTION_ALLIN && (*it_c)->getMyButton()==BUTTON_BIG_BLIND && (*it_c)->getMySet()<=smallBlind && currentRound == GAME_STATE_PREFLOP) {
					allInCondition = true;
					myBoard->setAllInCondition(true);
				}
			}
		}
	}

	// special routine
	if(allInCondition) {
		myBoard->collectPot();
		myGui->refreshPot();
		myGui->refreshSet();
		myGui->flipHolecardsAllIn();
		// Logging HoleCards
		if(currentRound<GAME_STATE_RIVER) {
			if(myLog) myLog->logHoleCardsHandName(activePlayerList);
		}

		if (currentRound < GAME_STATE_POST_RIVER) { // do not increment past 4
			currentRound = GameState(currentRound + 1);
			if(myLog) myLog->setCurrentRound(currentRound);
		}

		//log board cards for allin
		if(currentRound >= GAME_STATE_FLOP) {
			int boardCards[5];

			myBoard->getMyCards(boardCards);
			myGui->logDealBoardCardsMsg(currentRound, boardCards[0], boardCards[1], boardCards[2], boardCards[3], boardCards[4]);
			if(myLog) myLog->logBoardCards(boardCards);
		}

	}

	//unhighlight current players groupbox
	it_c = getActivePlayerIt(previousPlayerID);
	if( it_c != activePlayerList->end() ) {
		// lastPlayersTurn is active
		myGui->refreshGroupbox(previousPlayerID,1);
	}

	myGui->refreshGameLabels((GameState)getCurrentRound());

	if(currentRound < GAME_STATE_POST_RIVER) {
		roundBeforePostRiver = currentRound;
	}

	switch(currentRound) {
	case GAME_STATE_PREFLOP: {
		myGui->preflopAnimation1();
	}
	break;
	case GAME_STATE_FLOP: {
		myGui->flopAnimation1();
	}
	break;
	case GAME_STATE_TURN: {
		myGui->turnAnimation1();
	}
	break;
	case GAME_STATE_RIVER: {
		myGui->riverAnimation1();
	}
	break;
	case GAME_STATE_POST_RIVER: {
		myGui->postRiverAnimation1();
	}
	break;
	default:
	{}

	}

}

PlayerListIterator LocalHand::getSeatIt(unsigned uniqueId) const
{

	PlayerListIterator it;

	for(it=seatsList->begin(); it!=seatsList->end(); ++it) {
		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

	return it;

}

PlayerListIterator LocalHand::getActivePlayerIt(unsigned uniqueId) const
{

	PlayerListIterator it;

	for(it=activePlayerList->begin(); it!=activePlayerList->end(); ++it) {
		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

	return it;

}

PlayerListIterator LocalHand::getRunningPlayerIt(unsigned uniqueId) const
{

	PlayerListIterator it;

	for(it=runningPlayerList->begin(); it!=runningPlayerList->end(); ++it) {

		if((*it)->getMyUniqueID() == uniqueId) {
			break;
		}
	}

	return it;

}

void LocalHand::setLastActionPlayerID(unsigned theValue)
{
	lastActionPlayerID = theValue;
	myBoard->setLastActionPlayerID(theValue);
}
