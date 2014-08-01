#include <QtQml>
#include "playersortmodel.h"
#include "qmlenums.h"
#include "qmlserver.h"

PlayerSortModel::PlayerSortModel(QObject *parent):
    SortFilterProxyModel(parent), m_hideActive(false)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(QmlEnums::NickNameRole);
    setFilterKeyColumn(0);
    setDynamicSortFilter(true);
    sortByName();
}

void PlayerSortModel::registerType()
{
    qmlRegisterType<PlayerSortModel>("PokerTH",1, 0, "PlayerSortModel");
}

bool PlayerSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool ret = SortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    if(m_hideActive && ret) {
        QModelIndex index = QSortFilterProxyModel::sourceModel()->index(source_row, 0, source_parent);
        ret = static_cast<QmlServer::IdleRole>(QSortFilterProxyModel::sourceModel()->data(index,QmlEnums::NickIdleRole).toInt(0))==QmlServer::Idle;
    }
    return ret;
}

void PlayerSortModel::sortByName()
{
    setSortRole(QmlEnums::NickNameRole);
}

void PlayerSortModel::sortByCountry()
{
    setSortRole(QmlEnums::NickCountryRole);
}
