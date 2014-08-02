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

    enum GameEntry  {
        GameIdRole = Qt::UserRole + 1,
        GameNameRole,
        GameMeInThisGameRole,
        GameInfoRole,
        GamePlayerModelRole,
        GameSpectatorModelRole,

        //Used by sort filter
        GameSortPlayers = 0x1000,
        GameSortStatus,
        GameSortGameType,
        GameSortPwProtect,
        GameSortActionTimeout
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
