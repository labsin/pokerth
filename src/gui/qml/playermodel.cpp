#include "playermodel.h"
#include <QDebug>
#include "qmlplayer.h"
#include "storeplayers.h"

PlayerModel::PlayerModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_storePlayers = new StorePlayers(this);
    connect(m_storePlayers,SIGNAL(activeListShouldReset()),this,SLOT(resetModel()));
}

PlayerModel::~PlayerModel()
{
    delete m_storePlayers;
}

int PlayerModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "rowCount: " << m_storePlayers->activeCount();
    return m_storePlayers->activeCount();
}

QVariant PlayerModel::data(const QModelIndex &index, int role) const
{
    qDebug() <<"data() row: " << index.row() << " role: " << role;
    if (!m_storePlayers->validActiveIndex(index.row())) {
        return QVariant();
    }

    if(role == PlayerRole) {
        QmlPlayer *returnObj = (*m_storePlayers).activeAt(index.row());
        if(returnObj)
            return QVariant::fromValue<QmlPlayer*>(returnObj);
    }
    return QVariant();
}

bool PlayerModel::addPlayer(int index, QmlPlayer* player)
{
    qDebug()<<"add player at "<<index;
    if(player) {
        player->setId(index);
        if(m_storePlayers->validIndex(index)) {
            beginRemoveRows(QModelIndex(), index, index);
            m_storePlayers->removeAt(index);
            endRemoveRows();
        }
        if(player->getActiveStatus()) {
            beginInsertRows(QModelIndex(), index, index);
            m_storePlayers->add(player);
            endInsertRows();
        }
        else {
            m_storePlayers->add(player);
        }
        return true;
    }
    else {
        qDebug()<<"invalid player";
        return false;
    }
}

bool PlayerModel::addPlayer(QmlPlayer* player)
{
    qDebug()<<"add player";
    if(player) {
        int existing_count = m_storePlayers->count();
        player->setId(existing_count);
        beginInsertRows(QModelIndex(), existing_count, existing_count);
        m_storePlayers->add(player);
        endInsertRows();
        return true;
    }
    else {
        qDebug()<<"invalid player";
        return false;
    }
}

QmlPlayer * const &PlayerModel::at(int index)
{
    if(!m_storePlayers->validIndex(index)) {
        addPlayer(index,new QmlPlayer(m_storePlayers));
    }
    return m_storePlayers->at(index);
}

void PlayerModel::nextRound()
{
    m_storePlayers->cardsToDefault();
}

void PlayerModel::resetModel()
{
    beginResetModel();
    m_storePlayers->resetActiveList();
    endResetModel();
}

QHash<int, QByteArray> PlayerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PlayerRole] = "player";
    return roles;
}
