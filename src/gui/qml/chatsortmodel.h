#ifndef CHATSORTMODEL_H
#define CHATSORTMODEL_H
#include "sortfilterproxymodel.h"

class ChatSortModel : public SortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QVariant filterRoles READ filterRoles WRITE setfilterRoles NOTIFY filterRolesChanged)

public:
    explicit ChatSortModel(QObject *parent = 0);

    static void registerType();
    QVariant filterRoles() const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

signals:

    void filterRolesChanged(QVariant arg);

public slots:
    void setfilterRoles(QVariant arg);
private:
    QList<int> m_filterRoles;
};

#endif // CHATSORTMODEL_H
