#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H

#include <QObject>

#include <QList>
#include <QStringList>
#include <QString>
#include <QVariantList>

class ConfigFile;

class ConfigWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWrapper(ConfigFile *c, QObject *parent = 0);

    Q_INVOKABLE QString readConfigString(QString varName) const;
    Q_INVOKABLE QString readConfigBase64String(QString varName) const;
    Q_INVOKABLE QStringList readConfigStringList(QString varName) const;
    Q_INVOKABLE int readConfigInt(QString varName) const;
    Q_INVOKABLE QList<int> readConfigIntList(QString varName) const;
    Q_INVOKABLE QVariantList readConfigIntListAsQVariantList(QString varName) const;
    Q_INVOKABLE void writeConfigInt(QString varName, int varCont);
    Q_INVOKABLE void writeConfigString(QString varName, QString varCont);

    static void registerType();

signals:

public slots:

private:
    ConfigFile* myConfigFile;

};

#endif // CONFIGWRAPPER_H
