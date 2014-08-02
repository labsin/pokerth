#ifndef GAMEDATACLASS_H
#define GAMEDATACLASS_H

#include <QObject>
#include <QVariant>
#include <QString>
#include "gamedata.h"
#include "QQmlListProperty"

class GameDataClass : public QObject
{
    Q_OBJECT
    Q_ENUMS(GameType)
    Q_ENUMS(RaiseIntervalMode)
    Q_ENUMS(RaiseMode)
    Q_ENUMS(AfterManualBlindsMode)
    Q_PROPERTY(GameType gameType READ gameType WRITE setgameType NOTIFY gameTypeChanged)
    Q_PROPERTY(bool allowSpectators READ allowSpectators WRITE setallowSpectators NOTIFY allowSpectatorsChanged)
    Q_PROPERTY(int maxNumberOfPlayers READ maxNumberOfPlayers WRITE setmaxNumberOfPlayers NOTIFY maxNumberOfPlayersChanged)
    Q_PROPERTY(int startMoney READ startMoney WRITE setstartMoney NOTIFY startMoneyChanged)
    Q_PROPERTY(int firstSmallBlind READ firstSmallBlind WRITE setfirstSmallBlind NOTIFY firstSmallBlindChanged)
    Q_PROPERTY(RaiseIntervalMode raiseIntervalMode READ raiseIntervalMode WRITE setraiseIntervalMode NOTIFY raiseIntervalModeChanged)
    Q_PROPERTY(int raiseSmallBlindEveryHandsValue READ raiseSmallBlindEveryHandsValue WRITE setraiseSmallBlindEveryHandsValue NOTIFY raiseSmallBlindEveryHandsValueChanged)
    Q_PROPERTY(int raiseSmallBlindEveryMinutesValue READ raiseSmallBlindEveryMinutesValue WRITE setraiseSmallBlindEveryMinutesValue NOTIFY raiseSmallBlindEveryMinutesValueChanged)
    Q_PROPERTY(RaiseMode raiseMode READ raiseMode WRITE setraiseMode NOTIFY raiseModeChanged)
    Q_PROPERTY(QList<int> manualBlindsList READ manualBlindsList WRITE setmanualBlindsList NOTIFY manualBlindsListChanged)
    Q_PROPERTY(AfterManualBlindsMode afterManualBlindsMode READ afterManualBlindsMode WRITE setafterManualBlindsMode NOTIFY afterManualBlindsModeChanged)
    Q_PROPERTY(int afterMBAlwaysRaiseValue READ afterMBAlwaysRaiseValue WRITE setafterMBAlwaysRaiseValue NOTIFY afterMBAlwaysRaiseValueChanged)
    Q_PROPERTY(int guiSpeed READ guiSpeed WRITE setguiSpeed NOTIFY guiSpeedChanged)
    Q_PROPERTY(int delayBetweenHandsSec READ delayBetweenHandsSec WRITE setdelayBetweenHandsSec NOTIFY delayBetweenHandsSecChanged)
    Q_PROPERTY(int playerActionTimeoutSec READ playerActionTimeoutSec WRITE setplayerActionTimeoutSec NOTIFY playerActionTimeoutSecChanged)
    Q_PROPERTY(bool defaultBlinds READ defaultBlinds WRITE setdefaultBlinds NOTIFY defaultBlindsChanged)
    Q_PROPERTY(QString gameName READ gameName WRITE setGameName NOTIFY gameNameChanged)

public:
    explicit GameDataClass(QObject *parent = 0);
    GameDataClass(GameData gameData, QObject *parent = 0);

    enum GameType {
        GAME_TYPE_NORMAL = 1,
        GAME_TYPE_REGISTERED_ONLY,
        GAME_TYPE_INVITE_ONLY,
        GAME_TYPE_RANKING
    };

    enum RaiseIntervalMode {
        RAISE_ON_HANDNUMBER = 1,
        RAISE_ON_MINUTES
    };

    enum RaiseMode {
        DOUBLE_BLINDS = 1,
        MANUAL_BLINDS_ORDER
    };

    enum AfterManualBlindsMode {
        AFTERMB_DOUBLE_BLINDS = 1,
        AFTERMB_RAISE_ABOUT,
        AFTERMB_STAY_AT_LAST_BLIND
    };

    static void registerType();

    Q_INVOKABLE void setToDefault(int net=0); // 0 = local, 1 = net, 2 = internet

    Q_INVOKABLE void resetBlinds(int net=0);

    GameType gameType() const
    {
        return (enum GameType) gd.gameType;
    }

    bool allowSpectators() const
    {
        return gd.allowSpectators;
    }

    int maxNumberOfPlayers() const
    {
        return gd.maxNumberOfPlayers;
    }

    int startMoney() const
    {
        return gd.startMoney;
    }

    int firstSmallBlind() const
    {
        return gd.firstSmallBlind;
    }

    RaiseIntervalMode raiseIntervalMode() const
    {
        return (enum RaiseIntervalMode) gd.raiseIntervalMode;
    }

    int raiseSmallBlindEveryHandsValue() const
    {
        return gd.raiseSmallBlindEveryHandsValue;
    }

    int raiseSmallBlindEveryMinutesValue() const
    {
        return gd.raiseSmallBlindEveryMinutesValue;
    }

    RaiseMode raiseMode() const
    {
        return (enum RaiseMode)gd.raiseMode;
    }

    QList<int> manualBlindsList() const;

    AfterManualBlindsMode afterManualBlindsMode() const
    {
        return (enum AfterManualBlindsMode) gd.afterManualBlindsMode;
    }

    int afterMBAlwaysRaiseValue() const
    {
        return gd.afterMBAlwaysRaiseValue;
    }

    int guiSpeed() const
    {
        return gd.guiSpeed;
    }

    int delayBetweenHandsSec() const
    {
        return gd.delayBetweenHandsSec;
    }

    int playerActionTimeoutSec() const
    {
        return gd.playerActionTimeoutSec;
    }

    GameData getGameData() const
    {
        return gd;
    }

    bool defaultBlinds() const
    {
        return m_defaultBlinds;
    }

    QString gameName() const
    {
        return m_gameName;
    }

signals:

    void gameTypeChanged(GameType arg);

    void allowSpectatorsChanged(bool arg);

    void maxNumberOfPlayersChanged(int arg);

    void startMoneyChanged(int arg);

    void firstSmallBlindChanged(int arg);

    void raiseIntervalModeChanged(RaiseIntervalMode arg);

    void raiseSmallBlindEveryHandsValueChanged(int arg);

    void raiseSmallBlindEveryMinutesValueChanged(int arg);

    void raiseModeChanged(RaiseMode arg);

    void manualBlindsListChanged(QList<int> arg);

    void afterManualBlindsModeChanged(AfterManualBlindsMode arg);

    void afterMBAlwaysRaiseValueChanged(int arg);

    void guiSpeedChanged(int arg);

    void delayBetweenHandsSecChanged(int arg);

    void playerActionTimeoutSecChanged(int arg);

    void defaultBlindsChanged(bool arg);

    void gameNameChanged(QString arg);

public slots:

    void setgameType(GameType arg);

    void setallowSpectators(bool arg);

    void setmaxNumberOfPlayers(int arg);

    void setstartMoney(int arg);

    void setfirstSmallBlind(int arg);

    void setraiseIntervalMode(RaiseIntervalMode arg);

    void setraiseSmallBlindEveryHandsValue(int arg);

    void setraiseSmallBlindEveryMinutesValue(int arg);

    void setraiseMode(RaiseMode arg);

    void setmanualBlindsList(QList<int> arg);

    void setafterManualBlindsMode(AfterManualBlindsMode arg);

    void setafterMBAlwaysRaiseValue(int arg);

    void setguiSpeed(int arg);

    void setdelayBetweenHandsSec(int arg);

    void setplayerActionTimeoutSec(int arg);

    void setdefaultBlinds(bool arg);

    void setGameName(QString arg)
    {
        if (m_gameName != arg) {
            m_gameName = arg;
            emit gameNameChanged(arg);
        }
    }

private:
    GameData gd;
    bool m_defaultBlinds;
    QString m_gameName;
};

class GameInfoClass : public QObject
{
    Q_OBJECT
    Q_ENUMS(GameMode)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(GameDataClass* data READ gameData NOTIFY gameDataChanged)
    Q_PROPERTY(GameMode mode READ mode NOTIFY gameModeChanged)
    Q_PROPERTY(unsigned admin READ adminId NOTIFY adminIdChanged)
    Q_PROPERTY(bool isPasswordProtected READ isPasswordProtected NOTIFY isPasswordProtectedChanged)
public:
    explicit GameInfoClass(QObject *parent = 0);
    GameInfoClass(GameInfo gameInfo, QObject *parent = 0);
    ~GameInfoClass();

    enum GameMode {
        GAME_MODE_CREATED = 1,
        GAME_MODE_STARTED,
        GAME_MODE_CLOSED
    };

    QString name() const
    {
        return QString::fromUtf8(gi.name.c_str());
    }

    GameDataClass* gameData() const
    {
        return gd;
    }

    GameMode mode() const
    {
        return (enum GameMode) gi.mode;
    }

    unsigned adminId() const
    {
        return gi.adminPlayerId;
    }

    bool isPasswordProtected() const
    {
        return gi.isPasswordProtected;
    }

signals:

    void nameChanged(QString arg);
    void gameDataChanged(GameDataClass* arg);

    void gameModeChanged(GameMode arg);

    void adminIdChanged(unsigned arg);

    void isPasswordProtectedChanged(bool arg);

public slots:

private:
    GameInfo gi;
    GameDataClass* gd;
};

class StartDataClass : public QObject
{
    Q_OBJECT
public:
    explicit StartDataClass(QObject *parent = 0);
    StartDataClass(StartData startData, QObject *parent = 0);

signals:

public slots:

private:
    StartData sd;
};

class VoteKickDataClass : public QObject
{
    Q_OBJECT
public:
    explicit VoteKickDataClass(QObject *parent = 0);
    VoteKickDataClass(VoteKickData voteKickData, QObject *parent = 0);

signals:

public slots:

private:
    VoteKickData vd;
};

#endif // GAMEDATACLASS_H
