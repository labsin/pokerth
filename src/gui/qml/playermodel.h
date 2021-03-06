#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QAbstractListModel>

class QmlPlayer;
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

    void addPlayer(int index);
    void addPlayer();

    QmlPlayer * const &at(int index);

    StorePlayers* getStorePlayer() {
        return m_storePlayers;
    }

    void nextRound();

signals:

public slots:
    void resetModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    StorePlayers* m_storePlayers;

};

#endif // PLAYERMODEL_H
