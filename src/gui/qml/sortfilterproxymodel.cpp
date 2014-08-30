#include "sortfilterproxymodel.h"
#include "roleitemmodel.h"
#include <QtQml>

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), m_wildcardExpression("*")
{
}

void SortFilterProxyModel::registerType()
{
    qmlRegisterType<SortFilterProxyModel>();
}

QObject *SortFilterProxyModel::sourceModel() const
{
    return qobject_cast<QObject*>(QSortFilterProxyModel::sourceModel());
}

void SortFilterProxyModel::setsourceModel(QObject *arg)
{
    if (QSortFilterProxyModel::sourceModel() != arg) {
        if(QAbstractItemModel* model = qobject_cast<QAbstractItemModel*>(arg)) {
            if(QSortFilterProxyModel::sourceModel())
                if(RoleItemModel* tmp = qobject_cast<RoleItemModel*>(QSortFilterProxyModel::sourceModel()))
                    disconnect(tmp,SIGNAL(invalidate()), this, SLOT(invalidate()));
            QSortFilterProxyModel::setSourceModel(model);
            if(RoleItemModel* tmp = qobject_cast<RoleItemModel*>(arg)) {
                connect(tmp,SIGNAL(invalidate()), this, SLOT(invalidate()));
            }
            sort(sortColumn(), sortOrder());
            emit sourceModelChanged(arg);
        }
    }
}

void SortFilterProxyModel::sort(int column, Qt::SortOrder order)
{
    int tmp_column = column < 0 ? 0 : column;
    QSortFilterProxyModel::sort(tmp_column, order);
}
