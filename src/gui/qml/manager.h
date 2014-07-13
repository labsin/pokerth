#ifndef MANAGER_H
#define MANAGER_H

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QUrl>

#include "../../singleton.h"
#include "gamedataclass.h"
#include "gamedata.h"

#define DEBUG_MODE true

class GuiInterface;
class GuiWrapper;
class Session;
class Game;
class QmlGame;
class QmlServer;
class QAbstractTableModel;
class RoleItemModel;

class ConfigFile;

class Log;

class Manager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* servers READ getServerModel NOTIFY serversChanged)
    Q_PROPERTY(QmlServer* server READ getServer NOTIFY serverChanged)
    Q_PROPERTY(QmlGame* game READ getGame NOTIFY gameChanged)
public:
    Manager();
    ~Manager();

    void Init(ConfigFile *c, Log *l);

    void setSession(boost::shared_ptr<Session> session) {
        mySession = session;
    }
    boost::shared_ptr<Session> getSession() {
        assert(mySession.get());
        return mySession;
    }
    boost::shared_ptr<GuiInterface> getMyServerGuiInterface() const {
        return myServerGuiInterface;
    }

    ConfigFile* getConfig() {
        return myConfig;
    }

    Q_INVOKABLE void startGame(QObject *obj);
    Q_INVOKABLE void joinGame(unsigned gameId, QString password = QString());
    Q_INVOKABLE void joinGameLobby();
    Q_INVOKABLE void cancelConnect();

    GuiWrapper* getGui();

public slots:
    QmlGame *getGame();
    QmlServer *getServer();
    QObject *getServerModel();
    void serverListAdd(unsigned serverId);
    void serverListClear();
    void setPlayerAvatar(int myUniqueID, QString myAvatar);

signals:
    void guiInitiated();
    void serversChanged();
    void gameChanged();
    void serverChanged();
    void afterInit();
    void SignalNetClientServerListAdd(unsigned serverId);
    void SignalNetClientServerListClear();
    void signalSetPlayerAvatar(int myUniqueID, QString myAvatar);

private:
    ConfigFile *myConfig;
    Log *myLog;
    QmlGame *myGame;
    QmlServer *myServer;
    RoleItemModel *myServerModel;

    boost::shared_ptr<GuiInterface> myGuiInterface;
    boost::shared_ptr<Session> mySession;
    boost::shared_ptr<GuiInterface> myServerGuiInterface;
};

typedef Singleton<Manager> ManagerSingleton;

#endif // MANAGER_H
