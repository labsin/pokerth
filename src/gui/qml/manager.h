#ifndef MANAGER_H
#define MANAGER_H

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QUrl>

#include "../../singleton.h"
#include "gamedataclass.h"
#include "gamedata.h"

class GuiInterface;
class GuiWrapper;
class Session;
class Game;
class QmlGame;

class ConfigFile;

class Log;

class Manager: public QObject
{
    Q_OBJECT
public:
    Manager() {}

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
    Q_INVOKABLE QmlGame *getGame();

    GuiWrapper* getGui();

signals:
    void guiInitiated();

private:
    ConfigFile *myConfig;
    Log *myLog;

    boost::shared_ptr<GuiInterface> myGuiInterface;
    boost::shared_ptr<Session> mySession;
    boost::shared_ptr<GuiInterface> myServerGuiInterface;
};

typedef Singleton<Manager> ManagerSingleton;

#endif // MANAGER_H
