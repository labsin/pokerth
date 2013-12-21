#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QAbstractItemModel>
#include <QList>

class ServerItem
{
public:
    ServerItem(QString name, unsigned id, QString country) : m_name(name), m_serverId(id), m_country(country) { }
    ServerItem() {}

    void setData(QString name, unsigned id, QString country) {
        m_name = name;
        m_serverId = id;
        m_country = country;
    }

    void setName(QString name) { m_name = name; }
    void setId(unsigned id) { m_serverId = id; }
    void setCountry(QString cy) { m_country = cy; }

    QString name() const { return m_name; }
    unsigned id() const { return m_serverId; }
    QString country() const { return m_country; }

private:
    QString m_name;
    unsigned m_serverId;
    QString m_country;
};

class ServerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ServerModel(QObject *parent = 0);

    bool removeRows(int position, int rows, const QModelIndex &index);
    bool insertRows(int position, int rows, const QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool addServer(ServerItem &server) ;

signals:

public slots:

private:
    QList<ServerItem> serverList;

};

#endif // SERVERMODEL_H
