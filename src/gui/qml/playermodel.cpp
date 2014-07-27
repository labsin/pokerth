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
        QmlPlayer *returnObj = m_storePlayers->activeAt(index.row());
        if(returnObj)
            return QVariant::fromValue<QmlPlayer*>(returnObj);
    }
    return QVariant();
}

void PlayerModel::addPlayer(int index)
{
    qDebug()<<"add player at "<<index;
    QmlPlayer* player = new QmlPlayer(m_storePlayers);
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
}

void PlayerModel::addPlayer()
{
    qDebug()<<"add player";
    QmlPlayer* player = new QmlPlayer(m_storePlayers);
    int existing_count = m_storePlayers->count();
    player->setId(existing_count);
    beginInsertRows(QModelIndex(), existing_count, existing_count);
    m_storePlayers->add(player);
    endInsertRows();
}

QmlPlayer * const &PlayerModel::at(int index)
{
    if(!m_storePlayers->validIndex(index)) {
        addPlayer(index);
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
