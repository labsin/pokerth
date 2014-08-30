#ifndef ROLEITEMMODEL_H
#define ROLEITEMMODEL_H

#include <QStandardItemModel>

/* Convenience class to allow easily exposing
   C++ data as a model for QML View.

   This wouldn't need to exist if setRoleNames was
   a public member of QStandardItemModel
*/

class RoleItemModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ columnCount NOTIFY columnCountChanged)
public:
    /* Ctor. roleNames is a map describing when role id (e.g. Qt::UserRole+1)
      is associated with what name on QML side (e.g. 'bookTitle')
      */
    explicit RoleItemModel(QObject * parent = 0);
    RoleItemModel (const QHash<int, QByteArray> &roleNames, QObject * parent = 0);


    // Extract data from items in model as variant map
    // e.g. { "bookTitle" : QVariant("Bible"), "year" : QVariant(-2000) }
    static QVariantMap getModelData(const QAbstractItemModel *mdl, int row);

    Q_INVOKABLE int role(QString roleName);

public slots:

signals:
    void rowCountChanged();
    void columnCountChanged();
    void invalidate();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void init();

    QHash<int, QByteArray> m_roleNames;

    Qt::SortOrder m_sortOrder;
};

#endif // ROLEITEMMODEL_H
