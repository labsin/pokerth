#include "timeout.h"
#include "session.h"
#include "manager.h"
#include "QTimer"

Timeout::Timeout(QObject *parent) :
    QObject(parent), m_timeout(0), m_msgID(-1)
{
    m_timer = new QTimer(this);

    connect(this, SIGNAL(msgIDChanged(int)), this, SLOT(refresh()));
    connect(m_timer,SIGNAL(timeout()), this, SLOT(refresh()));
}

int Timeout::seconds() const
{
    int tmp = m_time.elapsed()/1000;
    tmp = m_timeout - tmp;
    return tmp > 0 ? tmp : 0;
}

QString Timeout::reason() const
{
    switch(m_msgID) {
    case NETWORK_TIMEOUT_GAME_ADMIN_IDLE:
        return tr("You are game-admin of an open game which will time out in %1 seconds.").arg(seconds(),0,10);
    case NETWORK_TIMEOUT_KICK_AFTER_AUTOFOLD:
        return tr("You did not act in the game recently. You will be removed from the game in %1 seconds.").arg(seconds(),0,10);
    default:
        return tr("Your connection is about to time out due to inactivity in %1 seconds.").arg(seconds(),0,10);
    }
}

void Timeout::stopTimeout()
{
    ManagerSingleton::Instance()->getSession()->resetNetworkTimeout();
    m_timer->stop();
}

void Timeout::refresh()
{
    emit this->reasonChanged(reason());
    emit this->secondsChanged(seconds());
}

void Timeout::setMsgID(int arg)
{
    if (m_msgID != arg) {
        m_msgID = arg;
        emit msgIDChanged(arg);
    }
}

void Timeout::startTimeout(int seconds)
{
    m_timeout = seconds;
    m_time.start();
    m_timer->start(1000);

    emit this->timeoutStarted(seconds);
}
