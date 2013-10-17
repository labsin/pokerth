#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QAbstractListModel>

class Player;
class StorePlayers;

class PlayerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PlayerRoles {
        PlayerRole = Qt::UserRole + 1
    };
    explicit PlayerModel(QObject *parent = 0);
    ~PlayerModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    bool addPlayer(int index, Player* player);
    bool addPlayer(Player* player);

    Player * const &at(int index);

    StorePlayers* getStorePlayer() {
        return m_storePlayers;
    }

signals:

public slots:
    void resetModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    StorePlayers* m_storePlayers;

};

#endif // PLAYERMODEL_H
