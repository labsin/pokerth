#include "gamedataclass.h"
#include <QtQml>
#include <QList>
#include <QDebug>
#include "configfile.h"
#include "manager.h"
#include "qmlplayer.h"

GameDataClass::GameDataClass(QObject *parent) :
    QObject(parent)
{
}

GameDataClass::GameDataClass(GameData gameData, QObject *parent) :
    QObject(parent), gd(gameData), m_defaultBlinds(true)
{
}

void GameDataClass::registerType()
{
    qmlRegisterType<GameDataClass>("PokerTH.GameData",1,0,"GameData");
    qmlRegisterType<GameInfoClass>("PokerTH.GameData",1,0,"GameInfo");
    qmlRegisterType<StartDataClass>("PokerTH.GameData",1,0,"StartData");
    qmlRegisterType<VoteKickDataClass>("PokerTH.GameData",1,0,"VoteKickData");
}

void GameDataClass::setToDefault(int net)
{
    ConfigFile* myConfig = ManagerSingleton::Instance()->getConfig();
    // Set Game Data
    if(net==2) {
        setgameType((enum GameType)myConfig->readConfigInt("InternetGameType"));
        setGameName(QString::fromUtf8(myConfig->readConfigString("InternetGameName").c_str()));
        setallowSpectators((bool)myConfig->readConfigInt("InternetGameAllowSpectators"));
    }
    if(net>1) {
        setmaxNumberOfPlayers(myConfig->readConfigInt("NetNumberOfPlayers"));
        setstartMoney(myConfig->readConfigInt("NetStartCash"));
        setfirstSmallBlind(myConfig->readConfigInt("NetFirstSmallBlind"));

        //Speeds
        setguiSpeed(myConfig->readConfigInt("NetGameSpeed"));
    }
    else {
        setmaxNumberOfPlayers(myConfig->readConfigInt("NumberOfPlayers"));
        setstartMoney(myConfig->readConfigInt("StartCash"));
        setfirstSmallBlind(myConfig->readConfigInt("FirstSmallBlind"));

        //Speeds
        setguiSpeed(myConfig->readConfigInt("GameSpeed"));
    }

    setdefaultBlinds(true);
    resetBlinds(net);
}

void GameDataClass::resetBlinds(int net)
{
    ConfigFile* myConfig = ManagerSingleton::Instance()->getConfig();

    if(net) {
        if(myConfig->readConfigInt("NetRaiseBlindsAtHands")) {
            setraiseIntervalMode((enum RaiseIntervalMode) ::RAISE_ON_HANDNUMBER);
        } else {
            setraiseIntervalMode((enum RaiseIntervalMode) ::RAISE_ON_MINUTES);
        }
        setraiseSmallBlindEveryHandsValue(myConfig->readConfigInt("NetRaiseSmallBlindEveryHands"));
        setraiseSmallBlindEveryMinutesValue(myConfig->readConfigInt("NetRaiseSmallBlindEveryMinutes"));

        if(myConfig->readConfigInt("NetAlwaysDoubleBlinds")) {
            setraiseMode((enum RaiseMode) ::DOUBLE_BLINDS);
        } else {
            setraiseMode((enum RaiseMode) ::MANUAL_BLINDS_ORDER);
        }
        gd.manualBlindsList = myConfig->readConfigIntList("NetManualBlindsList");
        emit manualBlindsListChanged(manualBlindsList());


        if(myConfig->readConfigInt("NetAfterMBAlwaysDoubleBlinds")) {
            setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_DOUBLE_BLINDS);
        } else {
            if(myConfig->readConfigInt("NetAfterMBAlwaysRaiseAbout")) {
                setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_RAISE_ABOUT);
            } else {
                setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_STAY_AT_LAST_BLIND);
            }
        }
        setafterMBAlwaysRaiseValue(myConfig->readConfigInt("NetAfterMBAlwaysRaiseValue"));
    }
    else {
        if(myConfig->readConfigInt("RaiseBlindsAtHands")) {
            setraiseIntervalMode((enum RaiseIntervalMode) ::RAISE_ON_HANDNUMBER);
        } else {
            setraiseIntervalMode((enum RaiseIntervalMode) ::RAISE_ON_MINUTES);
        }
        setraiseSmallBlindEveryHandsValue(myConfig->readConfigInt("RaiseSmallBlindEveryHands"));
        setraiseSmallBlindEveryMinutesValue(myConfig->readConfigInt("RaiseSmallBlindEveryMinutes"));

        if(myConfig->readConfigInt("AlwaysDoubleBlinds")) {
            setraiseMode((enum RaiseMode) ::DOUBLE_BLINDS);
        } else {
            setraiseMode((enum RaiseMode) ::MANUAL_BLINDS_ORDER);
        }
        gd.manualBlindsList = myConfig->readConfigIntList("ManualBlindsList");
        emit manualBlindsListChanged(manualBlindsList());


        if(myConfig->readConfigInt("AfterMBAlwaysDoubleBlinds")) {
            setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_DOUBLE_BLINDS);
        } else {
            if(myConfig->readConfigInt("AfterMBAlwaysRaiseAbout")) {
                setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_RAISE_ABOUT);
            } else {
                setafterManualBlindsMode((enum AfterManualBlindsMode) ::AFTERMB_STAY_AT_LAST_BLIND);
            }
        }
        setafterMBAlwaysRaiseValue(myConfig->readConfigInt("AfterMBAlwaysRaiseValue"));
    }
}

QList<int> GameDataClass::manualBlindsList() const
{
    std::list<int> list = gd.manualBlindsList;
    QList<int> retList;
    for (std::list<int>::iterator it=list.begin(); it != list.end(); ++it)
        retList.append(*it);
    return retList;
}

void GameDataClass::setgameType(GameDataClass::GameType arg)
{
    if (gameType() != arg) {
        gd.gameType = (enum ::GameType)arg;
        emit gameTypeChanged(arg);
    }
}

void GameDataClass::setallowSpectators(bool arg)
{
    if (gd.allowSpectators != arg) {
        gd.allowSpectators = arg;
        emit allowSpectatorsChanged(arg);
    }
}

void GameDataClass::setmaxNumberOfPlayers(int arg)
{
    if (gd.maxNumberOfPlayers != arg) {
        gd.maxNumberOfPlayers = arg;
        emit maxNumberOfPlayersChanged(arg);
    }
}

void GameDataClass::setstartMoney(int arg)
{
    if (gd.startMoney != arg) {
        gd.startMoney = arg;
        emit startMoneyChanged(arg);
    }
}

void GameDataClass::setfirstSmallBlind(int arg)
{
    if (gd.firstSmallBlind != arg) {
        gd.firstSmallBlind = arg;
        emit firstSmallBlindChanged(arg);
    }
}

void GameDataClass::setraiseIntervalMode(GameDataClass::RaiseIntervalMode arg)
{
    if ((int) gd.raiseIntervalMode != arg) {
        gd.raiseIntervalMode = (enum ::RaiseIntervalMode) arg;
        emit raiseIntervalModeChanged(arg);
    }
}

void GameDataClass::setraiseSmallBlindEveryHandsValue(int arg)
{
    if (gd.raiseSmallBlindEveryHandsValue != arg) {
        gd.raiseSmallBlindEveryHandsValue = arg;
        emit raiseSmallBlindEveryHandsValueChanged(arg);
    }
}

void GameDataClass::setraiseSmallBlindEveryMinutesValue(int arg)
{
    if (gd.raiseSmallBlindEveryMinutesValue != arg) {
        gd.raiseSmallBlindEveryMinutesValue = arg;
        emit raiseSmallBlindEveryMinutesValueChanged(arg);
    }
}

void GameDataClass::setraiseMode(GameDataClass::RaiseMode arg)
{
    if (raiseMode() != arg) {
        gd.raiseMode = (enum ::RaiseMode) arg;
        emit raiseModeChanged(arg);
    }
}

void GameDataClass::setmanualBlindsList(QList<int> arg)
{
    std::list<int> list;
    for (QList<int>::iterator it=arg.begin(); it != arg.end(); ++it)
        list.push_back((*it));
    gd.manualBlindsList = list;
    emit manualBlindsListChanged(arg);
}

void GameDataClass::setafterManualBlindsMode(GameDataClass::AfterManualBlindsMode arg)
{
    if ((int) gd.afterManualBlindsMode != arg) {
        gd.afterManualBlindsMode = (enum ::AfterManualBlindsMode) arg;
        emit afterManualBlindsModeChanged(arg);
    }
}

void GameDataClass::setafterMBAlwaysRaiseValue(int arg)
{
    if (gd.afterMBAlwaysRaiseValue != arg) {
        gd.afterMBAlwaysRaiseValue = arg;
        emit afterMBAlwaysRaiseValueChanged(arg);
    }
}

void GameDataClass::setguiSpeed(int arg)
{
    if (gd.guiSpeed != arg) {
        gd.guiSpeed = arg;
        emit guiSpeedChanged(arg);
    }
}

void GameDataClass::setdelayBetweenHandsSec(int arg)
{
    if (gd.delayBetweenHandsSec != arg) {
        gd.delayBetweenHandsSec = arg;
        emit delayBetweenHandsSecChanged(arg);
    }
}

void GameDataClass::setplayerActionTimeoutSec(int arg)
{
    if (gd.playerActionTimeoutSec != arg) {
        gd.playerActionTimeoutSec = arg;
        emit playerActionTimeoutSecChanged(arg);
    }
}

void GameDataClass::setdefaultBlinds(bool arg)
{
    if(m_defaultBlinds!=arg) {
        m_defaultBlinds = arg;
        emit defaultBlindsChanged(arg);
    }
}

GameInfoClass::GameInfoClass(QObject *parent) :
    QObject(parent), gd(NULL)
{
}

GameInfoClass::GameInfoClass(GameInfo gameInfo, QObject *parent) :
    QObject(parent), gi(gameInfo)
{
    gd = new GameDataClass(gameInfo.data, this);
}

GameInfoClass::~GameInfoClass()
{
    delete gd;
}

StartDataClass::StartDataClass(QObject *parent) :
    QObject(parent)
{
}

StartDataClass::StartDataClass(StartData startData, QObject *parent) :
    QObject(parent), sd(startData)
{
}

VoteKickDataClass::VoteKickDataClass(QObject *parent) :
    QObject(parent), m_player(NULL), m_voteStarterPlayer(NULL)
  , m_numberVotesNeeded(0), m_currentVotes(0), m_timeOut(0), m_shown(false)
{
}

void VoteKickDataClass::reset()
{
    setplayer(NULL);
    setvoteStarterPlayer(NULL);
    setNumberVotesNeeded(0);
    setCurrentVotes(0);
    setTimeOut(0);
    setShown(false);
}

void VoteKickDataClass::setplayer(QmlPlayer *arg)
{
    if (m_player != arg) {
        if(m_player) {
            m_player->settoBeKicked(false);
        }
        m_player = arg;
        m_player->settoBeKicked(true);
        emit playerChanged(arg);
    } else if(m_player) {
        m_player->settoBeKicked(true);
    }
}

void VoteKickDataClass::setvoteStarterPlayer(QmlPlayer *arg)
{
    if (m_voteStarterPlayer != arg) {
        if(m_voteStarterPlayer) {
            m_voteStarterPlayer->setvoteStarter(false);
        }
        m_voteStarterPlayer = arg;
        m_voteStarterPlayer->setvoteStarter(true);
        emit voteStarterPlayerChanged(arg);
    } else if(m_voteStarterPlayer) {
        m_voteStarterPlayer->setvoteStarter(true);
    }
}
