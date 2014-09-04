#include "roleitemmodel.h"


/* Example usage:


Enumerate the role ID's somewhere
---------------------------------

struct RedditEntry {

    enum RedditRoles {
        UrlRole = Qt::UserRole + 1,
        DescRole,
        ...
    };
    ...
}

Instantiate the class
---------------------


    QHash<int, QByteArray> roleNames;
    roleNames[RedditEntry::UrlRole] =  "url";
    roleNames[RedditEntry::ScoreRole] = "score";
    m_linksmodel = new RoleItemModel(roleNames);



Populate with data:
-------------------

    QStandardItem* it = new QStandardItem();
    it->setData(e.desc, RedditEntry::DescRole);
    it->setData(e.score, RedditEntry::ScoreRole);

    m_linksmodel->appendRow(it);

Expose to QML:
-------------

QDeclarativeContext *ctx = ...

ctx->setContextProperty("mdlLinks", m_linksmodel);

*/


RoleItemModel::RoleItemModel(QObject *parent)
    : QStandardItemModel(parent)
{
    init();
}

RoleItemModel::RoleItemModel(const QHash<int, QByteArray> &roleNames, QObject * parent)
    : QStandardItemModel(parent), m_roleNames(roleNames)
{
    init();
}

QVariantMap RoleItemModel::get(int row)
{
    return getModelData(this, row);
}

QVariantMap RoleItemModel::getModelData(const QAbstractItemModel* mdl, int row)
{
    QHash<int,QByteArray> names = mdl->roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
     while (i.hasNext()) {
        i.next();
        QModelIndex idx = mdl->index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
         //cout << i.key() << ": " << i.value() << endl;
     }
     return res;
}

int RoleItemModel::role(QString roleName)
{
    return m_roleNames.key(roleName.toLatin1());
}

QHash<int, QByteArray> RoleItemModel::roleNames() const
{
    return m_roleNames;
}

void RoleItemModel::init()
{
    connect(this,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SIGNAL(rowCountChanged()));
    connect(this,SIGNAL(rowsRemoved(QModelIndex,int,int)),this,SIGNAL(rowCountChanged()));
    connect(this,SIGNAL(columnsInserted(QModelIndex,int,int)),this,SIGNAL(columnCountChanged()));
    connect(this,SIGNAL(columnsRemoved(QModelIndex,int,int)),this,SIGNAL(columnCountChanged()));

}


