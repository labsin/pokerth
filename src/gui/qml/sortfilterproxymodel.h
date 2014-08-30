#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* sourceModel READ sourceModel WRITE setsourceModel NOTIFY sourceModelChanged)
    Q_PROPERTY(QString wildcardExpression READ wildcardExpression WRITE setwildcardExpression NOTIFY wildcardExpressionChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setsortOrder NOTIFY sortOrderChanged)

public:
    explicit SortFilterProxyModel(QObject *parent = 0);
    static void registerType();

    QObject* sourceModel() const;

    QString wildcardExpression() const
    {
        return m_wildcardExpression;
    }

public slots:
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
    void setsortOrder(Qt::SortOrder arg)
    {
        if (sortOrder() != arg) {
            sort(sortColumn(), arg);
            emit sortOrderChanged(arg);
        }
    }

signals:
    void sourceModelChanged(QObject* arg);
    void wildcardExpressionChanged(QString arg);

    void sortOrderChanged(Qt::SortOrder arg);

private:
    QString m_wildcardExpression;

};

#endif // SORTFILTERPROXYMODEL_H
