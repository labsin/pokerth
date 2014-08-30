#include "qmlgame.h"
#include "manager.h"
#include "guiwrapper.h"
#include "playermodel.h"
#include "playerinterface.h"
#include "qmlcard.h"
#include <QDebug>
#include <QtQml>

#include "playerinterface.h"
#include "boardinterface.h"
#include "handinterface.h"
#include "game.h"
#include "configfile.h"
#include "session.h"
#include "cardsvalue.h"

using namespace std;

QmlGame::QmlGame(QObject *parent) :
    QObject(parent),
    m_title("PokerTh"), m_pot(0), m_bets(0), m_gameNr(0)
  , m_handNr(0), guiGameSpeed(0), flipHolecardsAllInAlreadyDone(false)
  , currentGameOver(false), m_breakAfterCurrentHand(false)
  , m_blinkStartButton(false), m_checkedButton(NoButton), m_playingMode(ManualMode)
  , m_buttonsCheckable(false), m_highestSet(0), m_minimumRaise(0), m_fullBetRule(false)
  , m_smallBlind(0), m_gameState(QML_GAME_STATE_PREFLOP), m_myPlayer(NULL)
  , m_showMyCardsButton(false), m_checkCallRule(NoCheckCall), m_betRaiseRule(NoBetRaise)
{
    myPlayerModel = new PlayerModel(this);
    myManager = ManagerSingleton::Instance();

    m_flopCard1 = new QmlCard(this);
    m_flopCard2 = new QmlCard(this);
    m_flopCard3 = new QmlCard(this);
    m_turnCard = new QmlCard(this);
    m_riverCard = new QmlCard(this);

    m_voteKickData = new VoteKickDataClass(this);

    //Timer Objekt erstellen
    dealFlopCards0Timer = new QTimer(this);
    dealFlopCards1Timer = new QTimer(this);
    dealFlopCards2Timer = new QTimer(this);
    dealFlopCards3Timer = new QTimer(this);
    dealFlopCards4Timer = new QTimer(this);
    dealFlopCards5Timer = new QTimer(this);
    dealFlopCards6Timer = new QTimer(this);
    dealTurnCards0Timer = new QTimer(this);
    dealTurnCards1Timer = new QTimer(this);
    dealTurnCards2Timer = new QTimer(this);
    dealRiverCards0Timer = new QTimer(this);
    dealRiverCards1Timer = new QTimer(this);
    dealRiverCards2Timer = new QTimer(this);

    nextPlayerAnimationTimer = new QTimer(this);
    preflopAnimation1Timer = new QTimer(this);
    preflopAnimation2Timer = new QTimer(this);
    flopAnimation1Timer = new QTimer(this);
    flopAnimation2Timer = new QTimer(this);
    turnAnimation1Timer = new QTimer(this);
    turnAnimation2Timer = new QTimer(this);
    riverAnimation1Timer = new QTimer(this);
    riverAnimation2Timer = new QTimer(this);

    postRiverAnimation1Timer = new QTimer(this);
    postRiverRunAnimation1Timer = new QTimer(this);
    postRiverRunAnimation2Timer = new QTimer(this);
    postRiverRunAnimation2_flipHoleCards1Timer = new QTimer(this);
    postRiverRunAnimation2_flipHoleCards2Timer = new QTimer(this);
    postRiverRunAnimation3Timer = new QTimer(this);
    postRiverRunAnimation5Timer = new QTimer(this);
    postRiverRunAnimation6Timer = new QTimer(this);

    potDistributeTimer = new QTimer(this);
    voteOnKickTimeoutTimer = new QTimer(this);

    dealFlopCards0Timer->setSingleShot(true);
    dealFlopCards1Timer->setSingleShot(true);
    dealFlopCards2Timer->setSingleShot(true);
    dealFlopCards3Timer->setSingleShot(true);
    dealFlopCards4Timer->setSingleShot(true);
    dealFlopCards5Timer->setSingleShot(true);
    dealFlopCards6Timer->setSingleShot(true);
    dealTurnCards0Timer->setSingleShot(true);
    dealTurnCards1Timer->setSingleShot(true);
    dealTurnCards2Timer->setSingleShot(true);
    dealRiverCards0Timer->setSingleShot(true);
    dealRiverCards1Timer->setSingleShot(true);
    dealRiverCards2Timer->setSingleShot(true);

    nextPlayerAnimationTimer->setSingleShot(true);
    preflopAnimation1Timer->setSingleShot(true);
    preflopAnimation2Timer->setSingleShot(true);
    flopAnimation1Timer->setSingleShot(true);
    flopAnimation2Timer->setSingleShot(true);
    turnAnimation1Timer->setSingleShot(true);
    turnAnimation2Timer->setSingleShot(true);
    riverAnimation1Timer->setSingleShot(true);
    riverAnimation2Timer->setSingleShot(true);

    postRiverAnimation1Timer->setSingleShot(true);
    postRiverRunAnimation1Timer->setSingleShot(true);
    postRiverRunAnimation2Timer->setSingleShot(true);
    postRiverRunAnimation3Timer->setSingleShot(true);
    postRiverRunAnimation5Timer->setSingleShot(true);
    postRiverRunAnimation6Timer->setSingleShot(true);

    connect(dealFlopCards0Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards1() ));
    connect(dealFlopCards1Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards2() ));
    connect(dealFlopCards2Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards3() ));
    connect(dealFlopCards3Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards4() ));
    connect(dealFlopCards4Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards5() ));
    connect(dealFlopCards5Timer, SIGNAL(timeout()), this, SLOT( dealFlopCards6() ));
    connect(dealFlopCards6Timer, SIGNAL(timeout()), this, SLOT( handSwitchRounds() ));
    connect(dealTurnCards0Timer, SIGNAL(timeout()), this, SLOT( dealTurnCards1() ));
    connect(dealTurnCards1Timer, SIGNAL(timeout()), this, SLOT( dealTurnCards2() ));
    connect(dealTurnCards2Timer, SIGNAL(timeout()), this, SLOT( handSwitchRounds() ));
    connect(dealRiverCards0Timer, SIGNAL(timeout()), this, SLOT( dealRiverCards1() ));
    connect(dealRiverCards1Timer, SIGNAL(timeout()), this, SLOT( dealRiverCards2() ));
    connect(dealRiverCards2Timer, SIGNAL(timeout()), this, SLOT( handSwitchRounds() ));

    connect(nextPlayerAnimationTimer, SIGNAL(timeout()), this, SLOT( handSwitchRounds() ));
    connect(preflopAnimation1Timer, SIGNAL(timeout()), this, SLOT( preflopAnimation1Action() ));
    connect(preflopAnimation2Timer, SIGNAL(timeout()), this, SLOT( preflopAnimation2Action() ));
    connect(flopAnimation1Timer, SIGNAL(timeout()), this, SLOT( flopAnimation1Action() ));
    connect(flopAnimation2Timer, SIGNAL(timeout()), this, SLOT( flopAnimation2Action() ));
    connect(turnAnimation1Timer, SIGNAL(timeout()), this, SLOT( turnAnimation1Action() ));
    connect(turnAnimation2Timer, SIGNAL(timeout()), this, SLOT( turnAnimation2Action() ));
    connect(riverAnimation1Timer, SIGNAL(timeout()), this, SLOT( riverAnimation1Action() ));
    connect(riverAnimation2Timer, SIGNAL(timeout()), this, SLOT( riverAnimation2Action() ));

    connect(postRiverAnimation1Timer, SIGNAL(timeout()), this, SLOT( postRiverAnimation1Action() ));
    connect(postRiverRunAnimation1Timer, SIGNAL(timeout()), this, SLOT( postRiverRunAnimation2() ));
    connect(postRiverRunAnimation2Timer, SIGNAL(timeout()), this, SLOT( postRiverRunAnimation3() ));
    connect(postRiverRunAnimation3Timer, SIGNAL(timeout()), this, SLOT( postRiverRunAnimation4() ));
    connect(postRiverRunAnimation5Timer, SIGNAL(timeout()), this, SLOT( postRiverRunAnimation6() ));
    connect(postRiverRunAnimation6Timer, SIGNAL(timeout()), this, SLOT( startNewHand() ));

    connect(potDistributeTimer, SIGNAL(timeout()), this, SLOT(postRiverRunAnimation5()));
    //    connect(voteOnKickTimeoutTimer, SIGNAL(timeout()), this, SLOT(nextVoteOnKickTimeoutAnimationFrame()));

    connect(this, SIGNAL(signalInitGui(int)), this, SLOT(initGui(int)));
    connect(this, SIGNAL(signalRefreshSet()), this, SLOT(refreshSet()));
    connect(this, SIGNAL(signalRefreshCash()), this, SLOT(refreshCash()));
    connect(this, SIGNAL(signalRefreshAction(int, int)), this, SLOT(refreshAction(int,int)));
    connect(this, SIGNAL(signalRefreshPlayerAvatar()), this, SLOT(refreshPlayerAvatar()));
    connect(this, SIGNAL(signalRefreshChangePlayer()), this, SLOT(refreshSet()));
    connect(this, SIGNAL(signalRefreshChangePlayer()), this, SLOT(refreshActions()));
    connect(this, SIGNAL(signalRefreshChangePlayer()), this, SLOT(refreshCash()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshSet()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshButton()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshActions()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshCash()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshGroupbox()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshPlayerName()));
    connect(this, SIGNAL(signalRefreshAll()), this, SLOT(refreshPlayerAvatar()));
    connect(this, SIGNAL(signalRefreshPot()), this, SLOT(refreshPot()));
    connect(this, SIGNAL(signalRefreshGroupbox(int, int)), this, SLOT(refreshGroupbox(int, int)));
    connect(this, SIGNAL(signalRefreshPlayerName()), this, SLOT(refreshPlayerName()));
    connect(this, SIGNAL(signalRefreshButton()), this, SLOT(refreshButton()));
    connect(this, SIGNAL(signalRefreshGameLabels(int)), this, SLOT(refreshGameLabels(int)));

    connect(this, SIGNAL(signalSetPlayerAvatar(int,QString)), this, SLOT(setPlayerAvatar(int,QString)));

    connect(this, SIGNAL(signalDealBeRoCards(int)), this, SLOT(dealBeRoCards(int)));
    connect(this, SIGNAL(signalBeRoAnimation2(int)), this, SLOT(beRoAnimation2(int)));
    connect(this, SIGNAL(signalDealHoleCards()), this, SLOT(dealHoleCards()));
    connect(this, SIGNAL(signalDealFlopCards()), this, SLOT(dealFlopCards0()));
    connect(this, SIGNAL(signalDealTurnCards()), this, SLOT(dealTurnCards0()));
    connect(this, SIGNAL(signalDealRiverCards()), this, SLOT(dealRiverCards0()));
    connect(this, SIGNAL(signalNextPlayerAnimation()), this, SLOT(nextPlayerAnimation()));
    connect(this, SIGNAL(signalPreflopAnimation1()), this, SLOT(preflopAnimation1()));
    connect(this, SIGNAL(signalPreflopAnimation2()), this, SLOT(preflopAnimation2()));
    connect(this, SIGNAL(signalFlopAnimation1()), this, SLOT(flopAnimation1()));
    connect(this, SIGNAL(signalFlopAnimation2()), this, SLOT(flopAnimation2()));
    connect(this, SIGNAL(signalTurnAnimation1()), this, SLOT(turnAnimation1()));
    connect(this, SIGNAL(signalTurnAnimation2()), this, SLOT(turnAnimation2()));
    connect(this, SIGNAL(signalRiverAnimation1()), this, SLOT(riverAnimation1()));
    connect(this, SIGNAL(signalRiverAnimation2()), this, SLOT(riverAnimation2()));
    connect(this, SIGNAL(signalPostRiverAnimation1()), this, SLOT(postRiverAnimation1()));
    connect(this, SIGNAL(signalPostRiverRunAnimation1()), this, SLOT(postRiverRunAnimation1()));
    connect(this, SIGNAL(signalFlipHolecardsAllIn()), this, SLOT(flipHolecardsAllIn()));
    connect(this, SIGNAL(signalHandSwitchRounds()), this, SLOT(handSwitchRounds()));
    connect(this, SIGNAL(signalNextRoundCleanGui()), this, SLOT(nextRoundCleanGui()));
    connect(this, SIGNAL(signalNextRoundCleanGui()), this, SIGNAL(signalRefreshAll()));
    connect(this, SIGNAL(signalMeInAction()), this, SLOT(meInAction()));
    connect(this, SIGNAL(signalUpdateMyButtonsState()), this, SLOT(updateMyButtonsState()));
    connect(this, SIGNAL(signalDisableMyButtons()), this, SLOT(disableMyButtons()));
    connect(this, SIGNAL(signalStartTimeoutAnimation(int, int)), this, SLOT(startTimeoutAnimation(int, int)));
    connect(this, SIGNAL(signalStopTimeoutAnimation(int)), this, SLOT(stopTimeoutAnimation(int)));
    connect(this, SIGNAL(signalStartVoteOnKick(unsigned, unsigned, int, int)), this, SLOT(startVoteOnKick(unsigned, unsigned, int, int)));
    connect(this, SIGNAL(signalRefreshVotesMonitor(int,int)), this, SLOT(refreshVotesMonitor(int,int)));
    connect(this, SIGNAL(signalChangeVoteOnKickButtonsState(bool)), this, SLOT(changeVoteOnKickButtonsState(bool)));
    connect(this, SIGNAL(signalEndVoteOnKick()), this, SLOT(endVoteOnKick()));

    connect(this, SIGNAL(signalGuiUpdateDone()), this, SLOT(guiUpdateDone()));
    connect(this, SIGNAL(signalWaitForGuiUpdateDone()), this, SLOT(waitForGuiUpdateDone()));

    connect(this, SIGNAL(signalPostRiverShowCards(unsigned)), this, SLOT(showHoleCards(unsigned)));
    connect(this, SIGNAL(signalNetClientPlayerLeft(uint,QString,int)), SLOT(refreshPlayerAvatar()));
    connect(this, SIGNAL(signalNetClientPlayerLeft(uint,QString,int)), SLOT(refreshPlayerName()));
}

QmlGame::~QmlGame() {
    delete myPlayerModel;
}

void QmlGame::registerType()
{
    qmlRegisterType<QmlGame>("PokerTH",1, 0, "Game");
}

void QmlGame::setSpeeds()
{
    mainGameSpeed = (11-guiGameSpeed)*10;
    dealCardsSpeed = (mainGameSpeed/2)*10; //milliseconds
    preDealCardsSpeed = dealCardsSpeed*2; //Zeit for Karten aufdecken auf dem Board (Flop, Turn, River)
    postDealCardsSpeed = dealCardsSpeed*3; //Zeit nach Karten aufdecken auf dem Board (Flop, Turn, River)
    AllInDealCardsSpeed = dealCardsSpeed*4; //Zeit nach Karten aufdecken auf dem Board (Flop, Turn, River) bei AllIn
    postRiverRunAnimationSpeed = mainGameSpeed*18;
    winnerBlinkSpeed = mainGameSpeed*3; //milliseconds
    newRoundSpeed = mainGameSpeed*35;
    nextPlayerSpeed1 = mainGameSpeed*10; // Zeit zwischen dem Setzen des Spielers und dem Verdunkeln
    nextPlayerSpeed2 = mainGameSpeed*4; // Zeit zwischen Verdunkeln des einen und aufhellen des anderen Spielers
    nextPlayerSpeed3 = mainGameSpeed*7; // Zeit bis zwischen Aufhellen und Aktion
    preflopNextPlayerSpeed = mainGameSpeed*10; // Zeit bis zwischen Aufhellen und Aktion im Preflop (etwas langsamer da nicht gerechnet wird. )
}

QObject *QmlGame::getModel() const
{
    return static_cast<QObject *>(myPlayerModel);
}

PlayerModel *QmlGame::getPlayerModel() const
{
    return myPlayerModel;
}

void QmlGame::nextRoundCleanGui()
{
    myPlayerModel->nextRound();
    setblinkStartButton(false);
    m_flopCard1->init();
    m_flopCard2->init();
    m_flopCard3->init();
    m_turnCard->init();
    m_riverCard->init();
    setbuttonsCheckable(false);
    setbets(0);

    setShowMyCardsButton(false);

    flipHolecardsAllInAlreadyDone = false;
    emit nextRound();
}

void QmlGame::startTimeoutAnimation(int playerId, int timeoutSec)
{
    emit myPlayerModel->at(playerId)->startTimeoutAnimation(timeoutSec);
}

void QmlGame::stopTimeoutAnimation(int playerId)
{
    emit myPlayerModel->at(playerId)->stopTimeoutAnimation();
}

void QmlGame::startVoteOnKick(unsigned playerId, unsigned voteStarterPlayerId, int timeoutSec, int numVotesNeededToKick)
{
    m_voteKickData->reset();
    m_voteKickData->setNumberVotesNeeded(numVotesNeededToKick);
    m_voteKickData->setTimeOut(timeoutSec);
    m_voteKickData->setplayer(myPlayerModel->at(playerId));
    m_voteKickData->setvoteStarterPlayer(myPlayerModel->at(voteStarterPlayerId));
}

void QmlGame::refreshVotesMonitor(int currentVotes, int numVotesNeededToKick)
{
    m_voteKickData->setCurrentVotes(currentVotes);
    m_voteKickData->setNumberVotesNeeded(numVotesNeededToKick);
}

void QmlGame::changeVoteOnKickButtonsState(bool showHide)
{
    m_voteKickData->setShown(showHide);
}

void QmlGame::endVoteOnKick()
{
    m_voteKickData->reset();
}

void QmlGame::stopTimer()
{
    dealFlopCards0Timer->stop();
    dealFlopCards1Timer->stop();
    dealFlopCards2Timer->stop();
    dealFlopCards3Timer->stop();
    dealFlopCards4Timer->stop();
    dealFlopCards5Timer->stop();
    dealFlopCards6Timer->stop();
    dealTurnCards0Timer->stop();
    dealTurnCards1Timer->stop();
    dealTurnCards2Timer->stop();
    dealRiverCards0Timer->stop();
    dealRiverCards1Timer->stop();
    dealRiverCards2Timer->stop();

    nextPlayerAnimationTimer->stop();
    preflopAnimation1Timer->stop();
    preflopAnimation2Timer->stop();
    flopAnimation1Timer->stop();
    flopAnimation2Timer->stop();
    turnAnimation1Timer->stop();
    turnAnimation2Timer->stop();
    riverAnimation1Timer->stop();
    riverAnimation2Timer->stop();

    postRiverAnimation1Timer->stop();
    postRiverRunAnimation1Timer->stop();
    postRiverRunAnimation2Timer->stop();
    postRiverRunAnimation3Timer->stop();
    postRiverRunAnimation5Timer->stop();
    postRiverRunAnimation6Timer->stop();
    potDistributeTimer->stop();
}

bool QmlGame::check()
{
    if(buttonsCheckable()) {
        boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
        boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
        humanPlayer->setMyAction(PLAYER_ACTION_CHECK,true);
        humanPlayer->setMyTurn(0);

        //set that i was the last active player. need this for unhighlighting groupbox
        currentHand->setPreviousPlayerID(0);

        //Spiel läuft weiter
        myActionDone();
        return true;
    }
    return false;
}

bool QmlGame::call()
{
    if(buttonsCheckable()) {
        boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
        boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
        int tempHighestSet = currentHand->getCurrentBeRo()->getHighestSet();

        if (humanPlayer->getMyCash() + humanPlayer->getMySet() <= tempHighestSet) {

            humanPlayer->setMySet(humanPlayer->getMyCash());
            humanPlayer->setMyCash(0);
            humanPlayer->setMyAction(PLAYER_ACTION_ALLIN,true);
        } else {
            humanPlayer->setMySet(tempHighestSet - humanPlayer->getMySet());
            humanPlayer->setMyAction(PLAYER_ACTION_CALL,true);
        }
        humanPlayer->setMyTurn(0);

        currentHand->getBoard()->collectSets();
        myManager->getGui()->refreshPot();

        //set that i was the last active player. need this for unhighlighting groupbox
        currentHand->setPreviousPlayerID(0);

        //Spiel läuft weiter
        myActionDone();
        return true;
    }
    return false;
}

bool QmlGame::fold()
{
    if(buttonsCheckable()) {
        boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
        boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
        humanPlayer->setMyAction(PLAYER_ACTION_FOLD,true);
        humanPlayer->setMyTurn(0);

        //set that i was the last active player. need this for unhighlighting groupbox
        currentHand->setPreviousPlayerID(0);

        //Spiel läuft weiter
        myActionDone();
        return true;
    }
    return false;
}

bool QmlGame::allIn()
{
    if(buttonsCheckable()) {
        boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
        boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
        humanPlayer->setMySet(humanPlayer->getMyCash());
        humanPlayer->setMyCash(0);
        humanPlayer->setMyAction(PLAYER_ACTION_ALLIN,true);

        // full bet rule
        if(currentHand->getCurrentBeRo()->getHighestSet() + currentHand->getCurrentBeRo()->getMinimumRaise() > humanPlayer->getMySet()) {
            currentHand->getCurrentBeRo()->setFullBetRule(true);
        }

        if(humanPlayer->getMySet() > currentHand->getCurrentBeRo()->getHighestSet()) {
            currentHand->getCurrentBeRo()->setMinimumRaise(humanPlayer->getMySet() - currentHand->getCurrentBeRo()->getHighestSet());

            currentHand->getCurrentBeRo()->setHighestSet(humanPlayer->getMySet());

            // lastPlayerAction für Karten umblättern reihenfolge setzrn
            currentHand->setLastActionPlayerID(humanPlayer->getMyUniqueID());

        }

        humanPlayer->setMyTurn(0);

        currentHand->getBoard()->collectSets();
        myManager->getGui()->refreshPot();

        //set that i was the last active player. need this for unhighlighting groupbox
        currentHand->setPreviousPlayerID(0);

        //Spiel läuft weiter
        myActionDone();
        return true;
    }
    return false;
}

bool QmlGame::betRaise()
{
    if(buttonsCheckable()) {
        boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
        boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
        int tempCash = humanPlayer->getMyCash();
        humanPlayer->setMySet(m_bets);

        if (m_bets >= tempCash ) {

            humanPlayer->setMySet(humanPlayer->getMyCash());
            humanPlayer->setMyCash(0);
            humanPlayer->setMyAction(PLAYER_ACTION_ALLIN,true);

            // full bet rule
            if(currentHand->getCurrentBeRo()->getHighestSet() + currentHand->getCurrentBeRo()->getMinimumRaise() > humanPlayer->getMySet()) {
                currentHand->getCurrentBeRo()->setFullBetRule(true);
            }
        }

        if (m_highestSet == 0) {
            //do not if allIn
            if(humanPlayer->getMyAction() != 6) {
                humanPlayer->setMyAction(PLAYER_ACTION_BET,true);
            }

            currentHand->getCurrentBeRo()->setMinimumRaise(humanPlayer->getMySet());
        }
        else {
            //do not if allIn
            if(humanPlayer->getMyAction() != 6) {
                humanPlayer->setMyAction(PLAYER_ACTION_RAISE,true);
            }

            currentHand->getCurrentBeRo()->setMinimumRaise(humanPlayer->getMySet() - currentHand->getCurrentBeRo()->getHighestSet());
        }

        currentHand->getCurrentBeRo()->setHighestSet(humanPlayer->getMySet());

        humanPlayer->setMyTurn(0);

        currentHand->getBoard()->collectSets();
        myManager->getGui()->refreshPot();

        //set that i was the last active player. need this for unhighlighting groupbox
        currentHand->setPreviousPlayerID(0);

        // lastPlayerAction für Karten umblättern reihenfolge setzrn
        currentHand->setLastActionPlayerID(humanPlayer->getMyUniqueID());

        //Spiel läuft weiter
        myActionDone();
        return true;
    }
    return false;
}

void QmlGame::myActionDone()
{
    qDebug() << "myActionDone";
    // If a network client is running, we need
    // to transfer the action to the server.
    myManager->getSession()->sendClientPlayerAction();

    setmyTurn(false);
    setbuttonsCheckable(false);
    setbets(0);

    if (!myManager->getSession()->isNetworkClientRunning())
        nextPlayerAnimation();
}

void QmlGame::dealBeRoCards(int myBeRoID)
{
    switch(myBeRoID) {

    case 1:
        dealFlopCards0();
        break;
    case 2:
        dealTurnCards0();
        break;
    case 3:
        dealRiverCards0();
        break;
    default:
        qDebug() << "dealBeRoCards() Error";
    }
}

void QmlGame::dealHoleCards()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentGame->getSeatsList();
    int tempCardsIntArray[2];
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        (*it_c)->getMyHoleCards(tempCardsIntArray);
        if((*it_c)->getMyActiveStatus()) {
            myPlayerModel->at((*it_c)->getMyID())->setcardsDealt(true);
            myPlayerModel->at((*it_c)->getMyID())->setCards(tempCardsIntArray);
            if (( (*it_c)->getMyID() == 0)/* || DEBUG_MODE*/) {
                myPlayerModel->at((*it_c)->getMyID())->setFlip(true);
            }
        } else {
            myPlayerModel->at((*it_c)->getMyID())->setcardsDealt(false);
        }
    }

    //refresh CardsChanceMonitor Tool
//	refreshCardsChance(GAME_STATE_PREFLOP);
}


void QmlGame::dealFlopCards0()
{
    dealFlopCards0Timer->start(preDealCardsSpeed);
}

void QmlGame::dealFlopCards1()
{
    m_flopCard1->setdealt(true);
    dealFlopCards1Timer->start(dealCardsSpeed);
}

void QmlGame::dealFlopCards2()
{
    m_flopCard2->setdealt(true);
    dealFlopCards2Timer->start(dealCardsSpeed);
}

void QmlGame::dealFlopCards3()
{
    m_flopCard3->setdealt(true);
    dealFlopCards3Timer->start(dealCardsSpeed);
}

void QmlGame::dealFlopCards4()
{
    int tempBoardCardsArray[5];

    myManager->getSession()->getCurrentGame()->getCurrentHand()->getBoard()->getMyCards(tempBoardCardsArray);

    m_flopCard1->setcard(tempBoardCardsArray[0]);
    m_flopCard1->setflipped(true);
    dealFlopCards4Timer->start(dealCardsSpeed);
}

void QmlGame::dealFlopCards5()
{
    int tempBoardCardsArray[5];

    myManager->getSession()->getCurrentGame()->getCurrentHand()->getBoard()->getMyCards(tempBoardCardsArray);

    m_flopCard2->setcard(tempBoardCardsArray[1]);
    m_flopCard2->setflipped(true);
    dealFlopCards5Timer->start(dealCardsSpeed);
}

void QmlGame::dealFlopCards6()
{
    int tempBoardCardsArray[5];

    myManager->getSession()->getCurrentGame()->getCurrentHand()->getBoard()->getMyCards(tempBoardCardsArray);

    m_flopCard3->setcard(tempBoardCardsArray[2]);
    m_flopCard3->setflipped(true);

    // stable
    // wenn alle All In
    if(myManager->getSession()->getCurrentGame()->getCurrentHand()->getAllInCondition()) {
        dealFlopCards6Timer->start(AllInDealCardsSpeed);
    }
    // sonst normale Variante
    else {
        dealFlopCards6Timer->start(postDealCardsSpeed);
    }

    //refresh CardsChanceMonitor Tool
    //TODO: refreshCardsChance(GAME_STATE_FLOP);
}

void QmlGame::dealTurnCards0()
{
    dealTurnCards0Timer->start(preDealCardsSpeed);
}

void QmlGame::dealTurnCards1()
{
    m_turnCard->setdealt(true);
    dealTurnCards1Timer->start(dealCardsSpeed);
}

void QmlGame::dealTurnCards2()
{
    int tempBoardCardsArray[5];

    myManager->getSession()->getCurrentGame()->getCurrentHand()->getBoard()->getMyCards(tempBoardCardsArray);

    m_turnCard->setcard(tempBoardCardsArray[3]);
    m_turnCard->setflipped(true);

    // stable
    // wenn alle All In
    if(myManager->getSession()->getCurrentGame()->getCurrentHand()->getAllInCondition()) {
        dealTurnCards2Timer->start(AllInDealCardsSpeed);
    }
    // sonst normale Variante
    else {
        dealTurnCards2Timer->start(postDealCardsSpeed);
    }

    //refresh CardsChanceMonitor Tool
    //TODO: refreshCardsChance(GAME_STATE_TURN);
}

void QmlGame::dealRiverCards0()
{
    dealRiverCards0Timer->start(preDealCardsSpeed);
}

void QmlGame::dealRiverCards1()
{
    m_riverCard->setdealt(true);
    dealRiverCards1Timer->start(dealCardsSpeed);
}

void QmlGame::dealRiverCards2()
{
    int tempBoardCardsArray[5];

    myManager->getSession()->getCurrentGame()->getCurrentHand()->getBoard()->getMyCards(tempBoardCardsArray);

    m_riverCard->setcard(tempBoardCardsArray[4]);
    m_riverCard->setflipped(true);

    // stable
    // wenn alle All In
    if(myManager->getSession()->getCurrentGame()->getCurrentHand()->getAllInCondition()) {
        dealRiverCards2Timer->start(AllInDealCardsSpeed);
    }
    // sonst normale Variante
    else {
        dealRiverCards2Timer->start(postDealCardsSpeed);
    }

    //refresh CardsChanceMonitor Tool
    //TODO: refreshCardsChance(GAME_STATE_RIVER);
}

void QmlGame::nextPlayerAnimation()
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

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
    updateMyButtonsState();
    nextPlayerAnimationTimer->start(nextPlayerSpeed1);
}

void QmlGame::beRoAnimation2(int myBeRoID)
{

    switch(myBeRoID) {
    case 0:
        preflopAnimation2();
        break;
    case 1:
        flopAnimation2();
        break;
    case 2:
        turnAnimation2();
        break;
    case 3:
        riverAnimation2();
        break;
    default:
        qDebug() << "beRoAnimation2() Error";
    }
}


void QmlGame::preflopAnimation1()
{
    preflopAnimation1Timer->start(nextPlayerSpeed2);
}
void QmlGame::preflopAnimation1Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->run();
}

void QmlGame::preflopAnimation2()
{
    preflopAnimation2Timer->start(preflopNextPlayerSpeed);
}
void QmlGame::preflopAnimation2Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->nextPlayer();
}


void QmlGame::flopAnimation1()
{
    flopAnimation1Timer->start(nextPlayerSpeed2);
}
void QmlGame::flopAnimation1Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->run();
}

void QmlGame::flopAnimation2()
{
    flopAnimation2Timer->start(nextPlayerSpeed3);
}
void QmlGame::flopAnimation2Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->nextPlayer();
}

void QmlGame::turnAnimation1()
{
    turnAnimation1Timer->start(nextPlayerSpeed2);
}
void QmlGame::turnAnimation1Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->run();
}

void QmlGame::turnAnimation2()
{
    turnAnimation2Timer->start(nextPlayerSpeed3);
}
void QmlGame::turnAnimation2Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->nextPlayer();
}

void QmlGame::riverAnimation1()
{
    riverAnimation1Timer->start(nextPlayerSpeed2);
}
void QmlGame::riverAnimation1Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->run();
}

void QmlGame::riverAnimation2()
{
    riverAnimation2Timer->start(nextPlayerSpeed3);
}
void QmlGame::riverAnimation2Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->nextPlayer();
}

void QmlGame::postRiverAnimation1()
{
    postRiverAnimation1Timer->start(nextPlayerSpeed2);
}
void QmlGame::postRiverAnimation1Action()
{
    myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentBeRo()->postRiverRun();
}

void QmlGame::postRiverRunAnimation1()
{
    postRiverRunAnimation1Timer->start(postRiverRunAnimationSpeed);
}

void QmlGame::postRiverRunAnimation2()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    bool internetOrNetworkGame = (myManager->getSession()->getGameType() == Session::GAME_TYPE_INTERNET || myManager->getSession()->getGameType() == Session::GAME_TYPE_NETWORK);

    int nonfoldPlayersCounter = 0;
    PlayerListConstIterator it_c;
    PlayerList activePlayerList = currentGame->getActivePlayerList();
    for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if ((*it_c)->getMyAction() != PLAYER_ACTION_FOLD)
            nonfoldPlayersCounter++;
    }

    if(nonfoldPlayersCounter!=1) {

        if(!flipHolecardsAllInAlreadyDone) {

            for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
                if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD && (*it_c)->checkIfINeedToShowCards()) {
                    showHoleCards((*it_c)->getMyUniqueID());
                }

                //if human player dont need to show cards he gets the button "show cards" in internet or network game
                if( internetOrNetworkGame && (*it_c)->getMyID() == 0 && (*it_c)->getMyAction() != PLAYER_ACTION_FOLD && !(*it_c)->checkIfINeedToShowCards()) {
                    setShowMyCardsButton(true);
                }
            }
            //Wenn einmal umgedreht dann fertig!!
            flipHolecardsAllInAlreadyDone = true;
        } else {
            for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
                if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) {
                    //set Player value (logging) for all in already shown cards
                    (*it_c)->setMyHoleCardsFlip(1,3);
                }
            }
        }
        postRiverRunAnimation2Timer->start(postRiverRunAnimationSpeed);
    } else {

        //display show! button if human player is active and the latest non foldedone
        boost::shared_ptr<PlayerInterface> humanPlayer = myManager->getSession()->getCurrentGame()->getCurrentHand()->getSeatsList()->front();
        if( internetOrNetworkGame && humanPlayer->getMyActiveStatus() && humanPlayer->getMyAction() != PLAYER_ACTION_FOLD) {
            setShowMyCardsButton(true);
        }

        postRiverRunAnimation3();
    }

}

void QmlGame::postRiverRunAnimation3()
{

    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    int nonfoldPlayerCounter = 0;
    PlayerListConstIterator it_c;

    PlayerList activePlayerList = currentHand->getActivePlayerList();
    for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) {
            nonfoldPlayerCounter++;
        }
    }

    list<unsigned> winners = currentHand->getBoard()->getWinners();

    for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD && (*it_c)->getMyCardsValueInt() == currentHand->getCurrentBeRo()->getHighestCardsValue() ) {

            //Show "Winner" label
            myPlayerModel->at((*it_c)->getMyID())->setAction(QmlPlayer::WonAction);


            //show winnercards if more than one player is active TODO
            if ( nonfoldPlayerCounter != 1 && myManager->getConfig()->readConfigInt("ShowFadeOutCardsAnimation")) {

                int j;
                int bestHandPos[5];
                (*it_c)->getMyBestHandPosition(bestHandPos);

                bool index0 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 0 ) {
                        index0 = false;
                    }
                }
                if (index0) {
                    myPlayerModel->at((*it_c)->getMyID())->getCard(0)->setblur(true);
                }
                //index 1 testen
                bool index1 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 1 ) {
                        index1 = false;
                    }
                }
                if (index1) {
                    myPlayerModel->at((*it_c)->getMyID())->getCard(1)->setblur(true);
                }
                //index 2 testen
                bool index2 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 2 ) {
                        index2 = false;
                    }
                }
                if (index2) {
                    m_flopCard1->setblur(true);
                }
                //index 3 testen
                bool index3 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 3 ) {
                        index3 = false;
                    }
                }
                if (index3) {
                    m_flopCard2->setblur(true);
                }
                //index 4 testen
                bool index4 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 4 ) {
                        index4 = false;
                    }
                }
                if (index4) {
                    m_flopCard3->setblur(true);
                }
                //index 5 testen
                bool index5 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 5 ) {
                        index5 = false;
                    }
                }
                if (index5) {
                    m_turnCard->setblur(true);
                }
                //index 6 testen
                bool index6 = true;
                for(j=0; j<5; j++) {
                    if (bestHandPos[j] == 6 ) {
                        index6 = false;
                    }
                }
                if (index6) {
                    m_riverCard->setblur(true);
                }
            }
        } else {

            if( activePlayerList->size() != 1 && (*it_c)->getMyAction() != PLAYER_ACTION_FOLD && myManager->getConfig()->readConfigInt("ShowFadeOutCardsAnimation") ) {
                myPlayerModel->at((*it_c)->getMyID())->getCard(0)->setblur(true);
                myPlayerModel->at((*it_c)->getMyID())->getCard(1)->setblur(true);
            }
        }
    }

    // log side pot winners -> TODO
    list<unsigned>::iterator it_int;
    for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD && (*it_c)->getMyCardsValueInt() != currentHand->getCurrentBeRo()->getHighestCardsValue() ) {

            for(it_int = winners.begin(); it_int != winners.end(); ++it_int) {
                if((*it_int) == (*it_c)->getMyUniqueID()) {
                    //                    myGuiLog->logPlayerWinsMsg(QString::fromUtf8((*it_c)->getMyName().c_str()), (*it_c)->getLastMoneyWon(), false);
                }
            }

        }
    }

    for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if((*it_c)->getMyCash() == 0) {
            currentHand->getGuiInterface()->logPlayerSitsOut((*it_c)->getMyName());
        }
    }

    //	textBrowser_Log->append("");

    postRiverRunAnimation3Timer->start(postRiverRunAnimationSpeed/2);
}

void QmlGame::postRiverRunAnimation4()
{

    distributePotAnimCounter=0;
    potDistributeTimer->start(winnerBlinkSpeed);
}

void QmlGame::postRiverRunAnimation5()
{

    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    PlayerList activePlayerList = currentHand->getActivePlayerList();
    PlayerListConstIterator it_c;

    if (distributePotAnimCounter<10) {

        if (distributePotAnimCounter==0 || distributePotAnimCounter==2 || distributePotAnimCounter==4 || distributePotAnimCounter==6 || distributePotAnimCounter==8) {

            for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {

                if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD && (*it_c)->getMyCardsValueInt() == currentHand->getCurrentBeRo()->getHighestCardsValue() ) {

                    //                   playerNameLabelArray[(*it_c)->getMyID()]->hide();
                }
            }
        } else {

            for(it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {

                if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD && (*it_c)->getMyCardsValueInt() == currentHand->getCurrentBeRo()->getHighestCardsValue() ) {

                    //                    playerNameLabelArray[(*it_c)->getMyID()]->show();
                }
            }
        }

        distributePotAnimCounter++;
    } else {
        potDistributeTimer->stop();
        postRiverRunAnimation5Timer->start(mainGameSpeed);
    }
}

void QmlGame::postRiverRunAnimation6()
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    myManager->getGui()->refreshCash();
    myManager->getGui()->refreshPot();

    // TODO HACK
    // Check for network client, do not start new hand if client is running.
    if (myManager->getSession()->isNetworkClientRunning())
        return;

    // wenn nur noch ein Spieler aktive "neues Spiel"-Dialog anzeigen
    int playersPositiveCashCounter = 0;

    PlayerListConstIterator it_c;
    PlayerList activePlayerList = currentHand->getActivePlayerList();
    for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {

        if ((*it_c)->getMyCash() > 0) playersPositiveCashCounter++;
    }

    if (playersPositiveCashCounter==1) {

        for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {

            if ((*it_c)->getMyCash() > 0) {
                currentHand->getGuiInterface()->logPlayerWinGame((*it_c)->getMyName(),  myManager->getSession()->getCurrentGame()->getMyGameID());
            }
        }

        if( !DEBUG_MODE ) {

            if(myManager->getSession()->getGameType() == Session::GAME_TYPE_LOCAL) {
                currentGameOver = true;
                setblinkStartButton(true);
            }
        } else {
            //            myManager->callNewGameDialog();
            //Bei Cancel nichts machen!!!
        }
        return;
    }

    postRiverRunAnimation6Timer->start(newRoundSpeed);
}

void QmlGame::handSwitchRounds()
{
    qDebug() << "handSwitchRounds()";
    myManager->getSession()->getCurrentGame()->getCurrentHand()->switchRounds();
}

void QmlGame::startNewHand()
{

    if( !m_breakAfterCurrentHand) {
        myManager->getSession()->getCurrentGame()->initHand();
        myManager->getSession()->getCurrentGame()->startHand();
    } else {

        if(myManager->getSession()->getGameType() == Session::GAME_TYPE_LOCAL) {
            m_breakAfterCurrentHand=false;
            setblinkStartButton(true);
        }
    }
}

void QmlGame::meInAction()
{
    qDebug() << "meInAction()";
    setmyTurn(true);
    bool needAction = false;
    switch(m_playingMode) {
    case AutoCheckCallMode:
        break;
    case AutoCheckFoldMode:
        break;
    default:
        if(m_buttonsCheckable) {
            switch(m_checkedButton) {
            case BetRaiseButton:
                betRaise();
                break;
            case CheckCallButton:
                if(m_highestSet != 0) {
                    call();
                }
                else {
                    check();
                }
                break;
            case FoldButton:
                fold();
                break;
            case AllInButton:
                allIn();
                break;
            default:
                needAction = true;
                break;
            }
        }
        else {
            needAction = true;
        }
        break;
    }
    if(needAction) {
        provideMyActions();
    }
}

void QmlGame::flipHolecardsAllIn()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    if(!flipHolecardsAllInAlreadyDone && currentGame->getCurrentHand()->getCurrentRound() < GAME_STATE_RIVER) {
        //Aktive Spieler zählen --> wenn nur noch einer nicht-folded dann keine Karten umdrehen
        int nonfoldPlayersCounter = 0;
        PlayerListConstIterator it_c;
        PlayerList activePlayerList = currentGame->getActivePlayerList();
        for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
            if ((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) nonfoldPlayersCounter++;
        }

        if(nonfoldPlayersCounter!=1) {
            for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
                if((*it_c)->getMyAction() != PLAYER_ACTION_FOLD) {
                    showHoleCards((*it_c)->getMyUniqueID());
                }
            }
        }

        //Wenn einmal umgedreht dann fertig!!
        flipHolecardsAllInAlreadyDone = true;
    }
}

void QmlGame::updateMyButtonsState(int mode)
{
    qDebug() << "updateMyButtonsState";
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    if(currentHand->getPreviousPlayerID() == 0) {
        qDebug() << "previous was me";
        setbuttonsCheckable(false);
    } else {
        if(currentHand->getSeatsList()->front()->getMyAction() != PLAYER_ACTION_ALLIN) { // dont show pre-actions after flip cards when allin
//            setbuttonsCheckable(true);
            provideMyActions(mode);
        }
    }
}

void QmlGame::provideMyActions(int mode)
{
    qDebug() << "provideMyActions";
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();

    if(!m_myPlayer || m_myPlayer->getId() != humanPlayer->getMyID()) {
        m_myPlayer = myPlayerModel->at(humanPlayer->getMyID());
        emit myPlayerChanged(m_myPlayer);
    }
    if(highestSet() != currentHand->getCurrentBeRo()->getHighestSet()) {
        sethighestSet(currentHand->getCurrentBeRo()->getHighestSet());
        if((checkCallRule() == NoCheckCall && checkedButton() == CheckCallButton) || (betRaiseRule() == NoBetRaise && checkedButton() == BetRaiseButton))
            setcheckedButton(NoButton);
    }
    setfullBetRule(currentHand->getCurrentBeRo()->getFullBetRule());
    setminimumRaise(currentHand->getCurrentBeRo()->getMinimumRaise());
    setsmallBlind(currentHand->getSmallBlind());

    //really disabled buttons if human player is fold/all-in or server-autofold... and not called from dealberocards
    if(/*pushButton_BetRaise->isCheckable() && */(mode != 0 && (humanPlayer->getMyAction() == PLAYER_ACTION_ALLIN || humanPlayer->getMyAction() == PLAYER_ACTION_FOLD || (humanPlayer->getMySet() == currentHand->getCurrentBeRo()->getHighestSet() && (humanPlayer->getMyAction() != PLAYER_ACTION_NONE)))) || !humanPlayer->isSessionActive() /*server-autofold*/) {
        qDebug() << "Fold/All-int or I have highest set";
        setbuttonsCheckable(false);
    } else {
        if(mode == 0) {
            if( humanPlayer->getMyAction() == PLAYER_ACTION_ALLIN ) {
                qDebug() << "All in from dealberocards";
                setbuttonsCheckable(false);
            }
        }
        else {
            setbuttonsCheckable(true);
        }
    }
}

void QmlGame::initGui(int speed)
{
    stopTimer();
    assert(myManager->getSession());
    //set WindowTitle dynamically
    QString titleString = "";
    if(myManager->getSession()->getGameType() == Session::GAME_TYPE_INTERNET || myManager->getSession()->getGameType() == Session::GAME_TYPE_NETWORK) {
        GameInfo info(myManager->getSession()->getClientGameInfo(myManager->getSession()->getClientCurrentGameId()));
        titleString = QString::fromUtf8(info.name.c_str())+" - ";
    }
    settitle(QString(titleString + tr("PokerTH %1 - The Open-Source Texas Holdem Engine").arg(POKERTH_BETA_RELEASE_STRING)));

    //set speeds for local game and for first network game
    if( !myManager->getSession()->isNetworkClientRunning() || (myManager->getSession()->isNetworkClientRunning() && !myManager->getSession()->getCurrentGame()) ) {
        setgameSpeed(speed);
    }

    emit guiInitiated();

}

void QmlGame::refreshSet()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        myPlayerModel->at((*it_c)->getMyID())->setSet((*it_c)->getMySet());
    }
}

void QmlGame::refreshCash()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        QmlPlayer *tmp = myPlayerModel->at((*it_c)->getMyID());
        tmp->setActiveStatus((*it_c)->getMyActiveStatus());
        tmp->setStayOnTableStatus((*it_c)->getMyStayOnTableStatus());
        tmp->setIsSessionActive((*it_c)->isSessionActive());
        tmp->setIsKicked((*it_c)->isKicked());
        tmp->setIsMuted((*it_c)->isMuted());
        tmp->setCash((*it_c)->getMyCash());
    }
}

void QmlGame::refreshAction(int playerID, int playerAction)
{
    myPlayerModel->at(playerID)->setAction(static_cast<PlayerAction>(playerAction));
}

void QmlGame::refreshActions()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        refreshAction( (*it_c)->getMyID(), (*it_c)->getMyAction());
    }
}

void QmlGame::refreshPlayerAvatar()
{
    qDebug()<<"refreshPlayerAvatar()";
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        setPlayerAvatar((*it_c)->getMyUniqueID(), QString::fromUtf8((*it_c)->getMyAvatar().c_str()));
    }
}

void QmlGame::refreshPot()
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    setboardSet(currentHand->getBoard()->getSets());
    setpot(currentHand->getBoard()->getPot());
}

void QmlGame::refreshGroupbox(int playerID, int status)
{
    qDebug() << "refreshGroupbox: " << playerID << ","<<status;
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();
    PlayerList seatsList = currentGame->getSeatsList();
    if(playerID == -1 || status == -1) {
        for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
            QmlPlayer *tmp = myPlayerModel->at((*it_c)->getMyID());

            tmp->setTurn((*it_c)->getMyTurn());
            tmp->setActiveStatus((*it_c)->getMyActiveStatus());
        }
    } else {
        QmlPlayer *tmp = myPlayerModel->at(playerID);
        switch(status) {
        case 0:
            //inactive
            tmp->setActiveStatus(false);
            break;
        case 1:
        {
            //active but fold
            PlayerListConstIterator it_c;
            for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
                if(playerID == (*it_c)->getMyID())
                    break;
            }
            tmp->setActiveStatus(true);
            tmp->setTurn(false);
            tmp->setAction((*it_c)->getMyAction());
        }
            break;
        case 2:
            //active in action
            tmp->setActiveStatus(true);
            tmp->setTurn(true);
            break;
        case 3:
            //active not in action
            tmp->setActiveStatus(true);
            tmp->setTurn(false);
            break;
        }
    }
}

void QmlGame::refreshPlayerName()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        QmlPlayer *tmp = myPlayerModel->at((*it_c)->getMyID());
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

void QmlGame::refreshButton()
{
    boost::shared_ptr<Game> currentGame = myManager->getSession()->getCurrentGame();

    PlayerList seatsList = currentGame->getSeatsList();
    for (PlayerListConstIterator it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        myPlayerModel->at((*it_c)->getMyID())->setButton((*it_c)->getMyButton());
    }
}

void QmlGame::refreshGameLabels(int gameState)
{
    setgameState(static_cast<QmlGameState>(gameState));

    sethandNr(myManager->getSession()->getCurrentGame()->getCurrentHand()->getMyID());
    setgameNr(myManager->getSession()->getCurrentGame()->getMyGameID());

}

void QmlGame::setPlayerAvatar(int myUniqueID, QString myAvatar)
{
    boost::shared_ptr<PlayerInterface> tmpPlayer = myManager->getSession()->getCurrentGame()->getPlayerByUniqueId(myUniqueID);
    qDebug()<<"Avatar: "<<myAvatar;
    if (tmpPlayer.get()) {
        QString countryString(QString(myManager->getSession()->getClientPlayerInfo(myUniqueID).countryCode.c_str()).toLower());
        QmlPlayer* tmp = myPlayerModel->at(tmpPlayer->getMyID());
        tmp->setCountry(countryString);

        QFile myAvatarFile(myAvatar);
        if(myAvatarFile.exists()) {
            tmp->setAvatar(myAvatar);
        } else {
            tmp->setAvatar(QString(""));
        }
    }
}

void QmlGame::disableMyButtons()
{
    setbuttonsCheckable(false);
}

void QmlGame::guiUpdateDone()
{
    //guiUpdateSemaphore.release();
}

void QmlGame::waitForGuiUpdateDone()
{
    //guiUpdateSemaphore.acquire();
}

void QmlGame::showMyCards()
{
    if(showMyCardsButton()) {
        myManager->getSession()->showMyCards();
        setShowMyCardsButton(false);
    }
}

void QmlGame::showHoleCards(unsigned playerId, bool allIn)
{
    PlayerListConstIterator it_c;
    PlayerList activePlayerList = myManager->getSession()->getCurrentGame()->getActivePlayerList();
    for (it_c=activePlayerList->begin(); it_c!=activePlayerList->end(); ++it_c) {
        if((*it_c)->getMyID() == playerId) {
            int tempCardsIntArray[2];
            (*it_c)->getMyHoleCards(tempCardsIntArray);
            myPlayerModel->at((*it_c)->getMyID())->setCards(tempCardsIntArray);
            myPlayerModel->at((*it_c)->getMyID())->setFlip(true);
            //set Player value (logging)
            if(myManager->getSession()->getCurrentGame()->getCurrentHand()->getCurrentRound() < GAME_STATE_RIVER || allIn) {
                (*it_c)->setMyHoleCardsFlip(1,2); //for bero before postriver or allin just log the hole cards
            } else {
                (*it_c)->setMyHoleCardsFlip(1,1); //for postriver log the value
            }
        }
    }
}
