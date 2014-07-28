#ifndef QMLENUMS_H
#define QMLENUMS_H

#include <QObject>

class QmlEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(GameEntry)
    Q_ENUMS(NickEntry)
    Q_ENUMS(ServerEntry)

public:
    explicit QmlEnums(QObject *parent=0);

    static void registerType();

    enum GameEntry  {
        GameIdRole = Qt::UserRole + 1,
        GameNameRole,
        GameMeInThisGameRole,
        GameGameInfoRole,
        GamePlayerModelRole,
        GameSpectatorModelRole,
        //Used by sort filter

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
};

#endif // QMLENUMS_H
