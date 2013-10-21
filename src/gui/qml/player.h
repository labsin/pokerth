#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include "playerdata.h"
#include "game_defs.h"

class ConfigFile;
class HandInterface;
class QmlCard;

class Player : public QObject
{
    Q_OBJECT
    Q_ENUMS(Action)
    Q_ENUMS(PlayerType)
    Q_ENUMS(Button)
    Q_PROPERTY(int id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString guid READ getGuid NOTIFY guidChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(PlayerType type READ getPlayerType NOTIFY typeChanged)
    Q_PROPERTY(QString avatar READ getAvatar NOTIFY avatarChanged)
    Q_PROPERTY(bool guest READ getGuest NOTIFY isGuestChanged)
    Q_PROPERTY(QString country READ getCountry NOTIFY countryChanged)
    Q_PROPERTY(int cash READ getCash NOTIFY cashChanged)
    Q_PROPERTY(int set READ getSet NOTIFY setChanged)
    Q_PROPERTY(int stars READ getStars NOTIFY starsChanged)
    Q_PROPERTY(Action action READ getAction NOTIFY actionChanged)
    Q_PROPERTY(int startCash READ getRoundStartCash NOTIFY startCashChanged)
    Q_PROPERTY(Button button READ getButton NOTIFY buttonChanged)
    Q_PROPERTY(bool activeStatus READ getActiveStatus NOTIFY activeStatusChanged)
    Q_PROPERTY(bool stayOnTableStatus READ getStayOnTableStatus NOTIFY stayOnTableStatusChanged)
    Q_PROPERTY(QList<QObject*> cards READ getCards NOTIFY cardsChanged)
    Q_PROPERTY(bool thisTurn READ getTurn NOTIFY turnChanged)
    Q_PROPERTY(QString cardsValueInt READ getCardsValueInt NOTIFY cardsValueIntChanged)
    Q_PROPERTY(bool sessionActive READ isSessionActive NOTIFY sessionActive)
    Q_PROPERTY(bool kicked READ isKicked NOTIFY kicked)
    Q_PROPERTY(bool muted READ isMuted NOTIFY muted)

public:
    enum Action {
        NoAction = 0,
        FoldAction,
        CheckAction,
        CallAction,
        BetAction,
        RaiseAction,
        AllInAction,
        WonAction
    };

    enum Button {
        NoButton= 0,
        DealerButton,
        SmallBlindButton,
        BigBlindButton
    };

    enum PlayerType {
        PLAYER_TYPE_COMPUTER,
        PLAYER_TYPE_HUMAN
    };

    explicit Player(QObject *parent = 0);
    ~Player();

    static void registerType();

    void setHand(HandInterface *br);

    void setGuid(const QString &theValue) {
        if(QString::compare(theValue, myGuid) != 0){
            myGuid = QString(theValue);
            emit guidChanged(myGuid);
        }
    }

    void setPlayerType(const ::PlayerType &theValue) {
        PlayerType tmpValue = static_cast<PlayerType>(theValue);
        if(myType != tmpValue) {
            myType = tmpValue;
            emit typeChanged(myType);
        }
    }
    PlayerType getPlayerType() {
        return myType;
    }
    ::PlayerType getGlobalPlayerType() {
        return static_cast< ::PlayerType >(myType);;
    }

    QString getGuid() const {
        return myGuid;
    }

    void setName(const QString& theValue) {
        if(QString::compare(myName, theValue) != 0){
            myName = QString(theValue);
            emit nameChanged(myName);
        }
    }
    QString getName() const {
        return myName;
    }

    void setAvatar(const QString& theValue) {
        if(QString::compare(myAvatar, theValue) != 0){
            myAvatar = QString(theValue);
            emit avatarChanged(myAvatar);
        }
    }
    QString getAvatar() const {
        return myAvatar;
    }

    void setGuest(const bool& theValue) {
        if(isGuest != theValue) {
            isGuest = theValue;
            isGuestChanged(isGuest);
        }
    }
    bool getGuest() const {
        return isGuest;
    }

    void setCountry(const QString& theValue) {
        if(QString::compare(myCountry, theValue) != 0){
            myCountry = QString(theValue);
            emit countryChanged(myCountry);
        }
    }
    QString getCountry() {
        return myCountry;
    }

    void setCash(int theValue) {
        if(myCash != theValue){
            myCash = theValue;
            emit cashChanged(myCash);
        }
    }
    int getCash() const {
        return myCash;
    }

    void setSet(int theValue) {
        if(mySet != theValue ){
            mySet = theValue;
            emit setChanged(mySet);
        }
    }
    int getSet() const {
        return mySet;
    }

    void setStars(int theValue) {
        if(myStars !=theValue){
            myStars = theValue;
            emit starsChanged(myStars);
        }
    }
    int getStars() const {
        return myStars;
    }

    void setAction(PlayerAction theValue, bool human = 0) {
        Action tmpAction = static_cast<Action>(theValue);
        if(tmpAction != myAction){
            myAction = tmpAction;
            emit actionChanged(myAction);
        }
    }
    void setAction(Action theValue) {
        if(myAction !=theValue){
            myAction = theValue;
            emit actionChanged(myAction);
        }
    }
    Action getAction() const	{
        return myAction;
    }
    PlayerAction getPlayerAction() const	{
        return static_cast<PlayerAction>(myAction);
    }

    void setRoundStartCash(int theValue) {
        if(myRoundStartCash != theValue){
            myRoundStartCash = theValue;
            emit startCashChanged(myRoundStartCash);
        }
    }
    int getRoundStartCash() const {
        return myRoundStartCash;
    }

    void setButton(int theValue) {
        Button tmp = static_cast<Button>(theValue);
        if(tmp != myButton) {
            myButton = tmp;
            emit buttonChanged(myButton);
        }
    }
    Button getButton() const	{
        return myButton;
    }

    void setActiveStatus(bool theValue) {
        if(myActiveStatus != theValue) {
            myActiveStatus = theValue;
            emit activeStatusChanged(myActiveStatus);
        }
    }
    bool getActiveStatus() const {
        return myActiveStatus;
    }

    void setStayOnTableStatus(bool theValue) {
        if(myStayOnTableStatus != theValue){
            myStayOnTableStatus = theValue;
            emit stayOnTableStatusChanged(myStayOnTableStatus);
        }
    }
    bool getStayOnTableStatus() const {
        return myStayOnTableStatus;
    }

    QmlCard *getCard(int index);

    void setCards(int* theValue);

    QList<QObject *> getCards() const;

    void setTurn(bool theValue) {
        if(myTurn != theValue){
            myTurn = theValue;
            emit turnChanged(myTurn);
        }
    }
    bool getTurn() const {
        return myTurn;
    }

    void setCardsValueInt(int theValue) {
        if(myCardsValueInt !=theValue) {
            myCardsValueInt = theValue;
            emit cardsValueIntChanged(myCardsValueInt);
        }
    }
    int getCardsValueInt() const {
        return myCardsValueInt;
    }

    void setIsSessionActive(bool theValue) {
        if(m_isSessionActive != theValue) {
            m_isSessionActive = theValue;
            emit sessionActive(m_isSessionActive);
        }
    }
    bool isSessionActive() {
        return m_isSessionActive;
    }

    void setIsKicked(bool theValue) {
        if(m_isKicked != theValue) {
            m_isKicked = theValue;
            emit kicked(m_isKicked);
        }
    }
    bool isKicked() {
        return m_isKicked;
    }

    void setIsMuted(bool theValue) {
        if(m_isMuted != theValue) {
            m_isMuted = theValue;
            emit muted(m_isMuted);
        }
    }
    bool isMuted() {
        return m_isMuted;
    }

    int getId() const
    {
        return myId;
    }
    void setId(int arg)
    {
        if (myId != arg) {
            myId = arg;
            emit idChanged(arg);
        }
    }

    void initCards();

signals:

    void guidChanged(QString arg);
    void typeChanged(PlayerType arg);
    void nameChanged(QString arg);
    void avatarChanged(QString arg);
    void countryChanged(QString arg);
    void isGuestChanged(bool arg);
    void cashChanged(int arg);
    void setChanged(int arg);
    void starsChanged(int arg);
    void actionChanged(Action arg);
    void startCashChanged(int arg);
    void buttonChanged(Button arg);
    void activeStatusChanged(bool arg);
    void stayOnTableStatusChanged(bool arg);
    void cardsChanged(QList<QObject *> arg);
    void turnChanged(bool arg);
    void cardsValueIntChanged(int arg);

    void sessionActive(bool arg);
    void kicked(bool arg);
    void muted(bool arg);

    void idChanged(int arg);

public slots:

    void setFlip(bool flipped);

    void setcardsDealt(bool arg);

private:

    ConfigFile *myConfig;
    HandInterface *currentHand;

    // Konstanten
    int myId;
    QString myGuid;
    PlayerType myType;
    QString myName;
    QString myAvatar;
    bool isGuest;
    QString myCountry;


    // Laufvariablen
    int myCardsValueInt;
    int myBestHandPosition[5];
    double myOdds;
    int myNiveau[3];
    bool logHoleCardsDone;

    QList<QObject*> myCards;
    int myCash;
    int mySet;
    int myStars;
    Action myAction;
    Button myButton;
    bool myActiveStatus; // 0 = inactive, 1 = active
    bool myStayOnTableStatus; // 0 = left, 1 = stay
    bool myTurn; // 0 = no, 1 = yes
    int myRoundStartCash;

    bool m_isSessionActive;
    bool m_isKicked;
    bool m_isMuted;
    bool m_cardsDealt;
};

#endif // PLAYER_H
