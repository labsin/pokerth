#ifndef STOREPLAYERS_H
#define STOREPLAYERS_H

#include <QList>
#include <QDebug>

class QmlPlayer;

class StorePlayers : public QObject
{
    Q_OBJECT
public:
    StorePlayers(QObject *parent=0);
    ~StorePlayers();

    int count() const {
        if(m_players.isEmpty())
            return 0;
        return m_players.keys().last();
    }

    int activeCount() const {
        return m_activePlayers.count();
    }

    bool validIndex(int const & index) {
        return m_players.contains(index);
    }

    bool validActiveIndex(int const & index) {
        if(index>=0 && index < activeCount())
            return true;
        else
            return false;
    }

    void add(QmlPlayer* player);

    void removeAt(int index);

    QmlPlayer * const &at(int i) {
        Q_ASSERT(validIndex(i));
        return m_players[i];
    }

    QmlPlayer * const &activeAt(int i) {
        Q_ASSERT(validActiveIndex(i));
        int index = m_activePlayers[i];
        Q_ASSERT(validIndex(index));
        return m_players[index];
    }

    QmlPlayer * const &operator[](int i) {
        return at(i);
    }
    void resetActiveList();
    void cardsToDefault();

signals:
    void activeListShouldReset();
    void activeListChanged();

private:
    QMap<int,QmlPlayer*> m_players;
    QList<int> m_activePlayers;

    bool setActive(int id);

    bool setInActive(int id);
};

#endif // STOREPLAYERS_H
