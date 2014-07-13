#include "configwrapper.h"

#include <QtQml>
#include <QQmlEngine>
#include <QJSEngine>

#include "configfile.h"
#include "manager.h"

ConfigWrapper::ConfigWrapper(ConfigFile *c, QObject *parent) :
    QObject(parent), myConfigFile(c)
{
}

QString ConfigWrapper::readConfigString(QString varName) const
{
    return QString::fromStdString(myConfigFile->readConfigString(varName.toStdString()));
}

QString ConfigWrapper::readConfigBase64String(QString varName) const
{
    return QString::fromUtf8(QByteArray::fromBase64(myConfigFile->readConfigString(varName.toStdString()).c_str()));
}

QStringList ConfigWrapper::readConfigStringList(QString varName) const
{
    std::list<std::string>  stringList = myConfigFile->readConfigStringList(varName.toStdString());
    QStringList qStringList;
    for (std::list<std::string>::iterator it=stringList.begin(); it != stringList.end(); ++it)
        qStringList.append(QString::fromStdString(*it));
    return qStringList;
}

int ConfigWrapper::readConfigInt(QString varName) const
{
    return myConfigFile->readConfigInt(varName.toStdString());
}

QList<int> ConfigWrapper::readConfigIntList(QString varName) const
{
    std::list<int>  intList = myConfigFile->readConfigIntList(varName.toStdString());
    QList<int> qIntList;
    for (std::list<int>::iterator it=intList.begin(); it != intList.end(); ++it)
        qIntList.append(*it);
    return qIntList;
}

QVariantList ConfigWrapper::readConfigIntListAsQVariantList(QString varName) const
{
    std::list<int>  intList = myConfigFile->readConfigIntList(varName.toStdString());
    QVariantList list;
    for (std::list<int>::iterator it=intList.begin(); it != intList.end(); ++it)
        list.append(*it);
    return list;

}

void ConfigWrapper::writeConfigInt(QString varName, int varCont)
{
    myConfigFile->writeConfigInt(varName.toStdString(), varCont);
}

void ConfigWrapper::writeConfigString(QString varName, QString varCont)
{
    myConfigFile->writeConfigString(varName.toStdString(), varCont.toStdString());
}

// define the singleton type provider function (callback).
static QObject *config_wrapper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    ConfigFile* cf = ManagerSingleton::Instance()->getConfig();

    ConfigWrapper* example = new ConfigWrapper(cf);
    return example;
}

void ConfigWrapper::registerType()
{
    qmlRegisterSingletonType<ConfigWrapper>("PokerTH", 1, 0, "Config", config_wrapper_singletontype_provider);
}
