#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "roleitemmodel.h"

class GameModel : public RoleItemModel
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = 0);
    GameModel (const QHash<int, QByteArray> &roleNames, QObject * parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

#endif // GAMEMODEL_H
