#ifndef CUSTOMQMLAPPLICATIONENGINE_H
#define CUSTOMQMLAPPLICATIONENGINE_H

#include <QQmlApplicationEngine>

class CustomQmlApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
signals:
    void destroy();
public:
    CustomQmlApplicationEngine();
public slots:
    void onObjectCreated(QObject * object, const QUrl & url);
    void quitApp();
};

#endif // CUSTOMQMLAPPLICATIONENGINE_H
