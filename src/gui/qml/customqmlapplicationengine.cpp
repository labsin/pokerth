#include "customqmlapplicationengine.h"
#include <QDebug>
#include <QTimer>

CustomQmlApplicationEngine::CustomQmlApplicationEngine()
    : QQmlApplicationEngine()
{
    connect(this,SIGNAL(objectCreated(QObject*,QUrl)), this, SLOT(onObjectCreated(QObject*,QUrl)));
}

void CustomQmlApplicationEngine::onObjectCreated(QObject * object, const QUrl & url)
{
    if(object == NULL) {
        qDebug() << "QmlApplication: QmlObject is NULL";
        // Use a timer to wait for entering the event loop
        QTimer::singleShot(0, this, SLOT(quitApp()));
    }
}

void CustomQmlApplicationEngine::quitApp()
{
    this->quit();
}
