#include "gamesortmodel.h"
#include "qmlenums.h"
#include "gamedataclass.h"
#include <QtQml>

GameSortModel::GameSortModel(QObject *parent) :
    SortFilterProxyModel(parent), m_filterFull(false), m_filterStarted(false)
  , m_filterPrivate(false), m_filterNonRanking(false)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(QmlEnums::GameNameRole);
    setSortRole(QmlEnums::GameNameRole);
    setFilterKeyColumn(0);
    setDynamicSortFilter(true);
}

void GameSortModel::registerType()
{
    qmlRegisterType<GameSortModel>("PokerTH",1, 0, "GameSortModel");
}

bool GameSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool ret = SortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    QModelIndex index = QSortFilterProxyModel::sourceModel()->index(source_row, 0, source_parent);
    if(m_filterFull && ret) {
        int playerCount = qobject_cast<QAbstractItemModel*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GamePlayerModelRole).value<QObject*>())->rowCount();
        int maxNoPlayers = qobject_cast<GameInfoClass*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GameInfoRole).value<QObject*>())->gameData()->maxNumberOfPlayers();
        ret = (playerCount!=maxNoPlayers);
    }
    if(m_filterStarted && ret) {
        ret = qobject_cast<GameInfoClass*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GameInfoRole).value<QObject*>())->mode() == GameInfoClass::GAME_MODE_CREATED;
    }
    if(m_filterPrivate && ret) {
        ret = qobject_cast<GameInfoClass*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GameInfoRole).value<QObject*>())->gameData()->gameType() != GameDataClass::GAME_TYPE_INVITE_ONLY;
        if(ret) {
            ret = !qobject_cast<GameInfoClass*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GameInfoRole).value<QObject*>())->isPasswordProtected();
        }
    }
    if(m_filterNonRanking && ret) {
        ret = qobject_cast<GameInfoClass*>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::GameInfoRole).value<QObject*>())->gameData()->gameType() == GameDataClass::GAME_TYPE_RANKING;
    }
    return ret;
}

bool GameSortModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if(!left.model() || !right.model())
        return false;
    switch((enum QmlEnums::GameEntry) sortRole())
    {
    case QmlEnums::GameIdRole:
    case QmlEnums::GameNameRole:
    case QmlEnums::GameMeInThisGameRole:
        return SortFilterProxyModel::lessThan(left, right);
    }

    GameInfoClass* gil = qobject_cast<GameInfoClass*>(left.model()->data(left,QmlEnums::GameInfoRole).value<QObject*>());
    GameDataClass* gdl = gil->gameData();
    GameInfoClass* gir = qobject_cast<GameInfoClass*>(right.model()->data(right,QmlEnums::GameInfoRole).value<QObject*>());
    GameDataClass* gdr = gir->gameData();

    switch((enum QmlEnums::GameEntry) sortRole())
    {
    case QmlEnums::GamePlayersStringRole:
        if(gil->mode() == gir->mode()) {
            int pcl = qobject_cast<QAbstractItemModel*>(left.model()->data(left,QmlEnums::GamePlayerModelRole).value<QObject*>())->rowCount();
            int pcr = qobject_cast<QAbstractItemModel*>(right.model()->data(right,QmlEnums::GamePlayerModelRole).value<QObject*>())->rowCount();
            return (10*pcl)/gdl->maxNumberOfPlayers() < (10*pcr)/gdr->maxNumberOfPlayers();
        }
        return gil->mode() < gir->mode();
    case QmlEnums::GameStatusRole:
        return gil->mode() < gir->mode();
    case QmlEnums::GameTypeRole:
        return gdl->gameType() < gdr->gameType();
    case QmlEnums::GamePwProtectRole:
        return gil->isPasswordProtected() < gir->isPasswordProtected();
    case QmlEnums::GameTimeoutRole:
        return gdl->playerActionTimeoutSec() < gdr->playerActionTimeoutSec();
    }

    return false;
}
