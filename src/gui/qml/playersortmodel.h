#ifndef PLAYERSORTMODEL_H
#define PLAYERSORTMODEL_H

#include <QSortFilterProxyModel>

class PlayerSortModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hideActive READ hideActive WRITE sethideActive NOTIFY hideActiveChanged)
    Q_PROPERTY(QObject* sourceModel READ sourceModel WRITE setsourceModel NOTIFY sourceModelChanged)
    Q_PROPERTY(QString wildcardExpression READ wildcardExpression WRITE setwildcardExpression NOTIFY wildcardExpressionChanged)
public:
    explicit PlayerSortModel(QObject *parent = 0);
    static void registerType();
    bool hideActive() const
    {
        return m_hideActive;
    }
    QObject* sourceModel() const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    QString wildcardExpression() const
    {
        return m_wildcardExpression;
    }

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
    void setsourceModel(QObject* arg);
    void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder);

    void setwildcardExpression(QString arg)
    {
        if (m_wildcardExpression != arg) {
            m_wildcardExpression = arg;
            setFilterWildcard(arg);
            emit wildcardExpressionChanged(arg);
        }
    }

signals:
    void hideActiveChanged(bool arg);
    void sourceModelChanged(QObject* arg);

    void wildcardExpressionChanged(QString arg);

private:
    bool m_hideActive;
    QString m_wildcardExpression;
};

#endif // PLAYERSORTMODEL_H
