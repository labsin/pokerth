#ifndef QMLENUMS_H
#define QMLENUMS_H

#include <QObject>

class QmlEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(GameEntry)
    Q_ENUMS(NickEntry)
    Q_ENUMS(ServerEntry)
    Q_ENUMS(ChatEntry)

public:
    explicit QmlEnums(QObject *parent=0);

    static void registerType();

    // used as data(GameDataRole + DataGameType)
    enum DataEntry {
        DataGameType = 1,
        DataAllowSpectators,
        DataMaxNumberOfPlayers,
        DataStartMoney,
        DataFirstSmallBlind,
        DataRaiseIntervalMode,
        DataRaiseSmallBlindEveryHandsValue,
        DataRaiseSmallBlindEveryMinutesValue,
        DataRaiseMode,
        DataManualBlindsList,
        DataAfterManualBlindsMode,
        DataAfterMBAlwaysRaiseValue,
        DataGuiSpeed,
        DataDelayBetweenHandsSec,
        DataPlayerActionTimeoutSec,
        DataDefaultBlinds,
        DataGameName
    };

    enum InfoEntry {
        InfoGameName = 1,
        InfoGameData,
        InfoGameMode,
        InfoAdmin,
        InfoIsPwProt
    };

    enum GameEntry  {
        GameIdRole = Qt::UserRole + 1,
        GameNameRole,
        GameMeInThisGameRole,
        GamePlayerModelRole,
        GameSpectatorModelRole,

        //Not set directly, used in table/sort
        GamePlayersStringRole,
        GameStatusRole,
        GameTypeRole,
        GamePwProtectRole,
        GameTimeoutRole,

        GameInfoRole = 0x0300,
        GameDataRole = 0x0400
    };

    enum ServerEntry {
        ServerIdRole = Qt::UserRole + 1,
        ServerNameRole,
        ServerCountryRole
    };

    enum NickEntry {
        NickIdRole = Qt::UserRole + 1,
        NickNameRole,
        NickCountryRole,
        NickIsGuestRole,
        NickIdleRole,
        NickIsAdminRole
    };

    enum ChatEntry {
        ChatPlayerName = Qt::UserRole + 1,
        ChatMessage,
        ChatPM,
        ChatLobby,
        ChatGame,
        ChatBot,
        ChatMeReferred,
        ChatMyMessage,
        ChatOnIgnoreList
    };
};

#endif // QMLENUMS_H
