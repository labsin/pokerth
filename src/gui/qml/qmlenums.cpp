#include "qmlenums.h"
#include <QtQml>


QmlEnums::QmlEnums(QObject *parent) : QObject(parent){}

void QmlEnums::registerType()
{
    qmlRegisterType<QmlEnums>("PokerTH",1, 0, "PlayerSortModel");
}
