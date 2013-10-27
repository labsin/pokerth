#include <QApplication>
#include <QQmlContext>
#include <QQmlComponent>
#include <QDir>
#include <QUrl>
#include <QQuickWindow>
#include <QDebug>
#include "src/gui/qml/customqmlapplicationengine.h"
#include "third_party/qtsingleapplication/qtsingleapplication.h"
#include "configfile.h"
#include "log.h"
#include "gui/qml/manager.h"
#include "gui/qml/guiwrapper.h"
#include "gui/qml/imageprovider.h"
#include "gui/qml/qmlgame.h"
#include "gui/qml/qmlserver.h"

int main(int argc, char *argv[])
{
    /////// can be removed for non-qt-guis ////////////
    SharedTools::QtSingleApplication app( "PokerTH", argc, argv );

    if (app.sendMessage("Wake up!")) {
        return 0;
    }

#ifdef __APPLE__
    // The following needs to be done directly after the application is created.
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("plugins");
    QGuiApplication::setLibraryPaths(QStringList(dir.absolutePath()));
#endif

    //create defaultconfig
    ConfigFile *myConfig = new ConfigFile(argv[0], false);
    Log *myLog = new Log(myConfig);

    ManagerSingleton::Instance()->Init(myConfig, myLog);
//    QString rscPath = QDir::currentPath() + "/data";

    CustomQmlApplicationEngine engine;
    QObject::connect(&engine,SIGNAL(destroyed()), &app, SLOT(quit()));
    QString rscPath = ("file://"+myConfig->readConfigString("AppDataDir")).c_str();
    engine.rootContext()->setContextProperty("rscPath",rscPath);
    engine.rootContext()->setContextProperty("Manager",ManagerSingleton::Instance());
    engine.rootContext()->setContextProperty("CurrentGame",ManagerSingleton::Instance()->getGame());
    engine.rootContext()->setContextProperty("CurrentServer",ManagerSingleton::Instance()->getServer());
    engine.addImageProvider("PokerTH",new ImageProvider);
#ifdef NO_RESOURCES
    QUrl url = QUrl::fromLocalFile("resources/qml/Main.qml");
#else
    QUrl url("qrc:///qml/Main.qml");
#endif
//    QQmlComponent component(&engine, url);
//    QObject *object = component.create();
//    if(object) {
//        QMetaObject::invokeMethod(object,"show");
//    }
    engine.load(url);

    int returnCode = app.exec();

//    delete object;
    return returnCode;
}
