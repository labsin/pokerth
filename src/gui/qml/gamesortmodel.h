#ifndef GAMESORTMODEL_H
#define GAMESORTMODEL_H
#include "sortfilterproxymodel.h"

class GameSortModel : public SortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool filterFull READ filterFull WRITE setfilterFull NOTIFY filterFullChanged)
    Q_PROPERTY(bool filterStarted READ filterStarted WRITE setfilterStarted NOTIFY filterStartedChanged)
    Q_PROPERTY(bool filterPrivate READ filterPrivate WRITE setfilterPrivate NOTIFY filterPrivateChanged)
    Q_PROPERTY(bool filterNonRanking READ filterNonRanking WRITE setfilterNonRanking NOTIFY filterNonRankingChanged)

public:
    explicit GameSortModel(QObject *parent = 0);
    static void registerType();
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    bool filterFull() const
    {
        return m_filterFull;
    }
    bool filterStarted() const
    {
        return m_filterStarted;
    }
    bool filterPrivate() const
    {
        return m_filterPrivate;
    }
    bool filterNonRanking() const
    {
        return m_filterNonRanking;
    }

signals:
    void filterFullChanged(bool arg);
    void filterStartedChanged(bool arg);
    void filterPrivateChanged(bool arg);
    void filterNonRankingChanged(bool arg);

public slots:
    void setfilterFull(bool arg)
    {
        if (m_filterFull != arg) {
            m_filterFull = arg;
            emit filterFullChanged(arg);
        }
    }
    void setfilterStarted(bool arg)
    {
        if (m_filterStarted != arg) {
            m_filterStarted = arg;
            emit filterStartedChanged(arg);
        }
    }
    void setfilterPrivate(bool arg)
    {
        if (m_filterPrivate != arg) {
            m_filterPrivate = arg;
            emit filterPrivateChanged(arg);
        }
    }
    void setfilterNonRanking(bool arg)
    {
        if (m_filterNonRanking != arg) {
            m_filterNonRanking = arg;
            emit filterNonRankingChanged(arg);
        }
    }

private:
    bool m_filterFull;
    bool m_filterStarted;
    bool m_filterPrivate;
    bool m_filterNonRanking;
};

#endif // GAMESORTMODEL_H
