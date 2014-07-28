#ifndef QMLROLES_H
#define QMLROLES_H
#include <Qt>

namespace GameEntry {

    enum {
        IdRole = Qt::UserRole + 1,
        NameRole,
        MeInThisGameRole,
        GameInfoRole,
        PlayerModelRole,
        SpectatorModelRole
    };
}

namespace ServerEntry {

    enum {
        IdRole = Qt::UserRole + 1,
        NameRole,
        CountryRole
    };
}

namespace NickEntry {

    enum {
        IdRole = Qt::UserRole + 1,
        NameRole,
        CountryRole,
        IsGuestRole,
        IdleRole,
        IsAdminRole
    };
}

#endif // QMLROLES_H
