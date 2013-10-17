#include "player.h"
#include "handinterface.h"
#include "cardsvalue.h"
#include "configfile.h"
#include "qmlcard.h"
#include <QtQml>

Player::Player(QObject *parent) :
    QObject(parent), myCardsValueInt(0), myOdds(0), logHoleCardsDone(false), myCash(0), mySet(0), myStars(0), myAction(NoAction), myButton(NoButton), myActiveStatus(0), myStayOnTableStatus(0), myTurn(0), myRoundStartCash(0), m_isSessionActive(false), m_isKicked(false), m_isMuted(false)
{
    myCards << dynamic_cast<QObject *>(new QmlCard()) << dynamic_cast<QObject *>(new QmlCard());
    emit cardsChanged(myCards);
}

Player::~Player()
{
    delete myCards[0];
    delete myCards[1];
}

void Player::registerType()
{
    qmlRegisterType<Player>("PokerTH",1,0,"Player");
}

void Player::setHand(HandInterface *br)
{
    currentHand = br;
}

QmlCard *Player::getCard(int index) {
    return dynamic_cast<QmlCard *>(myCards[index]);
}

QList<QObject *> Player::getCards() const {
    return myCards;
}

void Player::setFlip(bool flipped) {
    for(int i=0; i<2; i++) {
        getCard(i)->setflipped(flipped);
    }
}


void Player::setCards(int *theValue) {
    for(int i=0; i<2; i++) {
        getCard(i)->setcard(theValue[i]);
    }
}

void Player::setcardsDealt(bool arg)
{
    for(int i=0; i<2; i++) {
        getCard(i)->setdealt(arg);
    }
}
