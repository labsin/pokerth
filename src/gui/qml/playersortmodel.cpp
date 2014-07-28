#include <QtQml>
#include "playersortmodel.h"
#include "qmlroles.h"
#include "qmlserver.h"

PlayerSortModel::PlayerSortModel(QObject *parent):
    SortFilterProxyModel(parent), m_hideActive(false)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(NickEntry::NameRole);
    setFilterKeyColumn(0);
    sortByName();
}

void PlayerSortModel::registerType()
{
    qmlRegisterType<PlayerSortModel>("PokerTH",1, 0, "PlayerSortModel");
}

bool PlayerSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool ret = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);;
//    bool ret = true;
    if(m_hideActive && ret) {
        QModelIndex index = QSortFilterProxyModel::sourceModel()->index(source_row, 0, source_parent);
        ret = static_cast<QmlServer::IdleRole>(QSortFilterProxyModel::sourceModel()->data(index,NickEntry::IdleRole).toInt(0))==QmlServer::Idle;
    }
    return ret;
}

void PlayerSortModel::sortByName()
{
    setSortRole(NickEntry::NameRole);
}

void PlayerSortModel::sortByCountry()
{
    setSortRole(NickEntry::CountryRole);
}
