#include "sortfilterproxymodel.h"
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
            QSortFilterProxyModel::setSourceModel(model);
            sort();
            emit sourceModelChanged(arg);
        }
    }
}

void SortFilterProxyModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);
}
