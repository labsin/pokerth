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
  , m_smallBlind(0), m_gameState(QML_GAME_STATE_PREFLOP)
{
    myPlayerModel = new PlayerModel();
    myManager = ManagerSingleton::Instance();

    m_flopCard1 = new QmlCard();
    m_flopCard2 = new QmlCard();
    m_flopCard3 = new QmlCard();
    m_turnCard = new QmlCard();
    m_riverCard = new QmlCard();

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

    connect(this, SIGNAL(nextPlayerAnimationSignal()), this, SLOT(nextPlayerAnimation()));

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

void QmlGame::setflopCard1(QmlCard *arg)
{
    if (m_flopCard1 != arg) {
        delete m_flopCard1;
        m_flopCard1 = arg;
        emit flopCard1Changed(arg);
    }
}

void QmlGame::setflopCard2(QmlCard *arg)
{
    if (m_flopCard2 != arg) {
        delete m_flopCard2;
        m_flopCard2 = arg;
        emit flopCard2Changed(arg);
    }
}

void QmlGame::setflopCard3(QmlCard *arg)
{
    if (m_flopCard3 != arg) {
        delete m_flopCard3;
        m_flopCard3 = arg;
        emit flopCard3Changed(arg);
    }
}

void QmlGame::setturnCard(QmlCard *arg)
{
    if (m_turnCard != arg) {
        delete m_turnCard;
        m_turnCard = arg;
        emit turnCardChanged(arg);
    }
}

void QmlGame::setriverCard(QmlCard *arg)
{
    if (m_riverCard != arg) {
        delete m_riverCard;
        m_riverCard = arg;
        emit riverCardChanged(arg);
    }
}

void QmlGame::nextRoundCleanGui()
{
    myPlayerModel->nextRound();
    setblinkStartButton(false);
    setflopCard1(new QmlCard());
    setflopCard2(new QmlCard());
    setflopCard3(new QmlCard());
    setturnCard(new QmlCard());
    setriverCard(new QmlCard());
    setbuttonsCheckable(false);
}

bool QmlGame::check()
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    boost::shared_ptr<PlayerInterface> currentPlayer = myManager->getSession()->getCurrentGame()->getCurrentPlayer();
    if(humanPlayer == currentPlayer) {
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
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    boost::shared_ptr<PlayerInterface> currentPlayer = myManager->getSession()->getCurrentGame()->getCurrentPlayer();
    if(humanPlayer == currentPlayer) {
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
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    boost::shared_ptr<PlayerInterface> currentPlayer = myManager->getSession()->getCurrentGame()->getCurrentPlayer();
    if(humanPlayer == currentPlayer) {
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
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    boost::shared_ptr<PlayerInterface> currentPlayer = myManager->getSession()->getCurrentGame()->getCurrentPlayer();
    if(humanPlayer == currentPlayer) {
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

bool QmlGame::set()
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    boost::shared_ptr<PlayerInterface> currentPlayer = myManager->getSession()->getCurrentGame()->getCurrentPlayer();
    if(humanPlayer == currentPlayer) {
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
    // If a network client is running, we need
    // to transfer the action to the server.
    myManager->getSession()->sendClientPlayerAction();

    setbuttonsCheckable(false);

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
        (*it_c)->getMyCards(tempCardsIntArray);
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
    // 	QTimer::singleShot(dealCardsSpeed, this, SLOT( dealRiverCards2() ));
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

    myManager->getGui()->refreshSet();

    PlayerListConstIterator it_c;
    PlayerList seatsList = currentHand->getSeatsList();
    for (it_c=seatsList->begin(); it_c!=seatsList->end(); ++it_c) {
        if((*it_c)->getMyID() == currentHand->getPreviousPlayerID()) break;
    }

    if(currentHand->getPreviousPlayerID() != -1) {
        myManager->getGui()->refreshAction(currentHand->getPreviousPlayerID(), (*it_c)->getMyAction());
    }
    myManager->getGui()->refreshCash();
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
                    myPlayerModel->at((*it_c)->getMyID())->setFlip(true);
                }

                //if human player dont need to show cards he gets the button "show cards" in internet or network game
                if( internetOrNetworkGame && (*it_c)->getMyID() == 0 && (*it_c)->getMyAction() != PLAYER_ACTION_FOLD && !(*it_c)->checkIfINeedToShowCards()) {
                    //showShowMyCardsButton();
                }
            }
            //Wenn einmal umgedreht dann fertig!!
            flipHolecardsAllInAlreadyDone = true;
        }
        postRiverRunAnimation2Timer->start(postRiverRunAnimationSpeed);
    } else {

        //display show! button if human player is active and the latest non foldedone
        boost::shared_ptr<PlayerInterface> humanPlayer = myManager->getSession()->getCurrentGame()->getCurrentHand()->getSeatsList()->front();
        if( internetOrNetworkGame && humanPlayer->getMyActiveStatus() && humanPlayer->getMyAction() != PLAYER_ACTION_FOLD) {

            //showShowMyCardsButton();
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
            myPlayerModel->at((*it_c)->getMyID())->setAction(Player::WonAction);

        } else {

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
    switch(m_playingMode) {
    case AutoCheckCallMode:
        break;
    case AutoCheckFoldMode:
        break;
    default:
        if(m_buttonsCheckable) {
            switch(m_checkedButton) {
            case BetRaiseButton:
                set();
                break;
            case CallButton:
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
                break;
            }
        }
        break;
    }

    if(m_checkedButton != NoButton) {

    }
    provideMyActions();
    setbuttonsCheckable(false);
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
                    myPlayerModel->at((*it_c)->getMyID())->setFlip(true);
                }
            }
        }

        //Wenn einmal umgedreht dann fertig!!
        flipHolecardsAllInAlreadyDone = true;
    }
}

void QmlGame::updateMyButtonsState(int mode)
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();

    if(currentHand->getPreviousPlayerID() == 0) {
        setbuttonsCheckable(false);
    } else {
        if(currentHand->getSeatsList()->front()->getMyAction() != PLAYER_ACTION_ALLIN) { // dont show pre-actions after flip cards when allin
            setbuttonsCheckable(true);
            provideMyActions(mode);
        }
    }
}

void QmlGame::provideMyActions(int mode)
{
    boost::shared_ptr<HandInterface> currentHand = myManager->getSession()->getCurrentGame()->getCurrentHand();
    boost::shared_ptr<PlayerInterface> humanPlayer = currentHand->getSeatsList()->front();
    PlayerList activePlayerList = currentHand->getActivePlayerList();

    setmyPlayer(myPlayerModel->at(humanPlayer->getMyID()));
    sethighestSet(currentHand->getCurrentBeRo()->getHighestSet());
    setfullBetRule(currentHand->getCurrentBeRo()->getFullBetRule());
    setminimumRaise(currentHand->getCurrentBeRo()->getMinimumRaise());
    setsmallBlind(currentHand->getSmallBlind());

    //really disabled buttons if human player is fold/all-in or server-autofold... and not called from dealberocards
    if(/*pushButton_BetRaise->isCheckable() && */(mode != 0 && (humanPlayer->getMyAction() == PLAYER_ACTION_ALLIN || humanPlayer->getMyAction() == PLAYER_ACTION_FOLD || (humanPlayer->getMySet() == currentHand->getCurrentBeRo()->getHighestSet() && (humanPlayer->getMyAction() != PLAYER_ACTION_NONE)))) || !humanPlayer->isSessionActive() /*server-autofold*/) {
        setbuttonsCheckable(false);
    } else {
        setbuttonsCheckable(true);

        if(mode == 0) {
            if( humanPlayer->getMyAction() == PLAYER_ACTION_FOLD ) {
                setbuttonsCheckable(false);
            }
        }
    }
}
