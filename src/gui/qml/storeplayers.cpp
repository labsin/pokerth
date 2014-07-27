#include "storeplayers.h"
#include "qmlplayer.h"
#include "QtCore"
#include "QObject"

StorePlayers::StorePlayers(QObject* parent)
    : QObject(parent)
{
}

StorePlayers::~StorePlayers()
{
    QMap<int, QmlPlayer*>::iterator it;
    for (it = m_players.begin(); it != m_players.end(); ++it){
        delete it.value();
    }
    m_players.clear();
    m_activePlayers.clear();
}

void StorePlayers::add(QmlPlayer *player) {
    QObject::connect(player,SIGNAL(activeStatusChanged(bool)),this,SIGNAL(activeListShouldReset()));
    m_players.insert(player->getId(),player);
    if(player->getActiveStatus()) {
        setActive(player->getId());
    }
}

void StorePlayers::removeAt(int index) {
    delete m_players[index];
    m_players.remove(index);
    setInActive(index);
}

bool StorePlayers::setActive(int id)
{
    if(!m_activePlayers.contains(id)) {
        QList<int>::Iterator it;
        int jj;
        for (it = m_activePlayers.begin(), jj = 0; it != m_activePlayers.end(); ++it, ++jj){
            if(*it>id) {
                break;
            }
        }
        m_activePlayers.insert(jj,id);
        return true;
    }
    return false;
}

bool StorePlayers::setInActive(int id)
{
    if(m_activePlayers.contains(id)) {
        m_activePlayers.removeAll(id);
        return true;
    }
    return false;
}

void StorePlayers::resetActiveList()
{
    QMap<int, QmlPlayer*>::iterator it;
    bool changed = false;
    for (it = m_players.begin(); it != m_players.end(); ++it){
        if(it.value()->getActiveStatus()) {
            if(setActive(it.value()->getId())) {
                changed = true;
            }
        }
        else {
            if(setInActive(it.value()->getId())){
                changed = true;
            }
        }
    }
    if(changed)
        emit activeListChanged();
}

void StorePlayers::cardsToDefault()
{
    QMap<int, QmlPlayer*>::iterator it;
    bool changed = false;
    for (it = m_players.begin(); it != m_players.end(); ++it){
        it.value()->initCards();
    }
}
