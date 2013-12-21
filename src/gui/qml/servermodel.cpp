#include "servermodel.h"

ServerModel::ServerModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ServerModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return serverList.count();
}

int ServerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

QVariant ServerModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(!index.isValid())
        return QVariant();
    if(0>index.row() || index.row() >= serverList.count())
        return QVariant();

    switch(index.column()) {
    case 0:
        return serverList.at(index.row()).name();
    case 1:
        return serverList.at(index.row()).country();
    case 2:
        return serverList.at(index.row()).id();
    default:
        return QVariant();
    }

}

QVariant ServerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(section >= 3 || section < 0)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Name";

        case 1:
            return "Country";

        case 2:
            return "Id";

        default:
            return QVariant();
        }
    }

    return QVariant();
}

bool ServerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role != Qt::EditRole)
        return false;

    int row = index.row();

    ServerItem s = serverList.at(row);

    switch(index.column()) {
    case 0:
        s.setName(value.toString());
    case 1:
        s.setCountry(value.toString());
    case 2:
        s.setId(value.toUInt());
    default:
        return false;
    }

    serverList.replace(row, s);
    emit(dataChanged(index, index));

    return true;
}

bool ServerModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    if(0>position || position >= serverList.count())
        return false;

    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        serverList.insert( position, ServerItem(" ",0," ") );
    }

    endInsertRows();
    return true;
}

bool ServerModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    if(0>position || position >= serverList.count())
        return false;

    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        serverList.removeAt(position);
    }

    endRemoveRows();
    return true;
}

Qt::ItemFlags ServerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ServerModel::addServer(ServerItem &server)
{
    QList<ServerItem>::iterator it;
    int index = 0;
    bool replace = false;
    for(it  = serverList.begin(); it != serverList.end(); ++it) {
        if((*it).id()==server.id()) {
            //replace
            replace = true;
            break;
        }
        else if((*it).id()>server.id()) {
            //insert at index
            break;
        }
        index++;
    }
    if(replace) {
        if(!removeRow(index))
            return false;
    }
    if(!insertRow(index))
        return false;
    serverList.replace(index,server);
    emit(dataChanged(this->index(index,0), this->index(index,2)));
}
