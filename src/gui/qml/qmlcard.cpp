#include "qmlcard.h"
#include <QtQml>

QmlCard::QmlCard(QObject *parent) :
    QObject(parent), m_card(-1), m_flipped(false)
  , m_dealt(false), m_blur(false)
{
}

void QmlCard::init()
{
    setcard(-1);
    setdealt(false);
    setblur(false);
    setflipped(false);
}

void QmlCard::registerType()
{
    qmlRegisterType<QmlCard>("PokerTH",1,0,"Card");
}
