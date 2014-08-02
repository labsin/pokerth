#include "chatsortmodel.h"
#include "qmlenums.h"
#include <QtQml>
#include <QVariant>
#include <QRegExp>

ChatSortModel::ChatSortModel(QObject *parent) :
    SortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(QmlEnums::ChatPlayerName);
    setFilterKeyColumn(0);
    setDynamicSortFilter(true);
    m_filterRoles << QmlEnums::ChatOnIgnoreList;
}

void ChatSortModel::registerType()
{
    qmlRegisterType<ChatSortModel>("PokerTH",1, 0, "ChatSortModel");
}

QVariant ChatSortModel::filterRoles() const
{
    return QVariant::fromValue(m_filterRoles);
}

bool ChatSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!SortFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
        return false;
    }
    bool ret = true;
    QModelIndex index = QSortFilterProxyModel::sourceModel()->index(source_row, 0, source_parent);
    if (!index.isValid()) // the column may not exist
        return true;
    foreach (int role, m_filterRoles) {
        QVariant var = QSortFilterProxyModel::sourceModel()->data(index,role);
        switch((enum QmlEnums::ChatEntry) role) {
        case QmlEnums::ChatMessage:
        case QmlEnums::ChatPlayerName:
        {
            QRegExp filter_regexp = filterRegExp();
            if (!filter_regexp.isEmpty()) {
                QString key = var.toString();
                ret = key.contains(filter_regexp);
            }
            break;
        }
        case QmlEnums::ChatBot:
        case QmlEnums::ChatPM:
        case QmlEnums::ChatLobby:
        case QmlEnums::ChatGame:
        case QmlEnums::ChatMeReferred:
        case QmlEnums::ChatMyMessage:
        case QmlEnums::ChatOnIgnoreList:
            ret = !var.toBool();
            break;
        }

        if(!ret) {
            break;
        }
    }
    return ret;
}

void ChatSortModel::setfilterRoles(QVariant arg)
{
    m_filterRoles.clear();
    foreach (QVariant item, arg.toList()) {
        bool ok = false;
        int key = item.toInt(&ok);
        if (ok) {
            m_filterRoles.append(key);
        }
    }
    emit filterRolesChanged(filterRoles());
    sort();
}
