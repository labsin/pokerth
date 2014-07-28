#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* sourceModel READ sourceModel WRITE setsourceModel NOTIFY sourceModelChanged)
    Q_PROPERTY(QString wildcardExpression READ wildcardExpression WRITE setwildcardExpression NOTIFY wildcardExpressionChanged)
public:
    explicit SortFilterProxyModel(QObject *parent = 0);
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

signals:
    void sourceModelChanged(QObject* arg);
    void wildcardExpressionChanged(QString arg);

private:
    QString m_wildcardExpression;

};

#endif // SORTFILTERPROXYMODEL_H
