#include "qmlplayer.h"
#include "handinterface.h"
#include "cardsvalue.h"
#include "configfile.h"
#include "qmlcard.h"
#include <QtQml>

QmlPlayer::QmlPlayer(QObject *parent) :
    QObject(parent), myCardsValueInt(0), myOdds(0), logHoleCardsDone(false), myCash(0), mySet(0), myStars(0), myAction(NoAction), myButton(NoButton), myActiveStatus(0), myStayOnTableStatus(0), myTurn(0), myRoundStartCash(0), m_isSessionActive(false), m_isKicked(false), m_isMuted(false)
{
    QmlCard* c1 = new QmlCard(this);
    QmlCard* c2 = new QmlCard(this);
    myCards << c1 << c2;
}

QmlPlayer::~QmlPlayer()
{
    delete myCards[0];
    delete myCards[1];
    myCards.clear();
}

void QmlPlayer::registerType()
{
    qmlRegisterType<QmlPlayer>("PokerTH",1,0,"Player");
}

void QmlPlayer::setHand(HandInterface *br)
{
    currentHand = br;
}

QmlCard *QmlPlayer::getCard(int index) {
    return myCards[index];
}

QQmlListProperty<QmlCard> QmlPlayer::getCards() {
    return QQmlListProperty<QmlCard>(this, myCards);
}

void QmlPlayer::initCards()
{
    getCard(0)->init();
    getCard(1)->init();
}

void QmlPlayer::setFlip(bool flipped) {
    for(int i=0; i<2; i++) {
        getCard(i)->setflipped(flipped);
    }
}


void QmlPlayer::setCards(int *theValue) {
    for(int i=0; i<2; i++) {
        getCard(i)->setcard(theValue[i]);
    }
}

void QmlPlayer::setcardsDealt(bool arg)
{
    for(int i=0; i<2; i++) {
        getCard(i)->setdealt(arg);
    }
}
