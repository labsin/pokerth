#ifndef PLAYERSORTMODEL_H
#define PLAYERSORTMODEL_H

#include "sortfilterproxymodel.h"

class PlayerSortModel : public SortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hideActive READ hideActive WRITE sethideActive NOTIFY hideActiveChanged)
public:
    explicit PlayerSortModel(QObject *parent = 0);
    static void registerType();
    bool hideActive() const
    {
        return m_hideActive;
    }
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

public slots:
    void sortByName();
    void sortByCountry();
    void sethideActive(bool arg)
    {
        if (m_hideActive != arg) {
            m_hideActive = arg;
            emit hideActiveChanged(arg);
        }
    }

signals:
    void hideActiveChanged(bool arg);

private:
    bool m_hideActive;
};

#endif // PLAYERSORTMODEL_H
