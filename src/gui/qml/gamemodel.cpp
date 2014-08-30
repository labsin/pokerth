#include "gamemodel.h"
#include "qmlenums.h"
#include "gamedataclass.h"

GameModel::GameModel(QObject *parent) :
    RoleItemModel(parent)
{
}

GameModel::GameModel(const QHash<int, QByteArray> &roleNames, QObject *parent):
    RoleItemModel(roleNames, parent)
{

}

QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if(role >= QmlEnums::GamePlayersStringRole && role < QmlEnums::GameInfoRole) {
        QVariant data = RoleItemModel::data(index,QmlEnums::GameInfoRole);
        if(!data.canConvert<QObject*>()) {
            return QVariant();
        }
        GameInfoClass* gi = qobject_cast<GameInfoClass*>(data.value<QObject*>());
        if(!gi) {
            return QVariant();
        }
        GameDataClass* gd = gi->gameData();
        switch(role) {
        case QmlEnums::GamePlayersStringRole: {
            int playerCount = qobject_cast<QAbstractItemModel*>(RoleItemModel::data(index,QmlEnums::GamePlayerModelRole).value<QObject*>())->rowCount();
            return QVariant::fromValue(QString::number(playerCount)+"/"+QString::number(gd->maxNumberOfPlayers()));
        }
        case QmlEnums::GameStatusRole:
            return QVariant::fromValue((int) gi->mode());
        case QmlEnums::GameTypeRole:
            return QVariant::fromValue((int) gd->gameType());
        case QmlEnums::GamePwProtectRole:
            return QVariant::fromValue(gi->isPasswordProtected());
        case QmlEnums::GameTimeoutRole:
            return QVariant::fromValue(QString::number(gd->playerActionTimeoutSec())+"/"+QString::number(gd->delayBetweenHandsSec()));
        }
    }
    else if(role > QmlEnums::GameInfoRole && role != QmlEnums::GameDataRole) {
        QStandardItem* item = itemFromIndex(index);
        QVariant data = item->data(QmlEnums::GameInfoRole);
        if(!data.canConvert<QObject*>()) {
            return QVariant();
        }
        GameInfoClass* gi = qobject_cast<GameInfoClass*>(data.value<QObject*>());
        if(!gi) {
            return QVariant();
        }
        if(role > QmlEnums::GameDataRole) {
            GameDataClass* gd = gi->gameData();
            switch(role-QmlEnums::GameDataRole) {
            case QmlEnums::DataGameType:
                return QVariant::fromValue((int) gd->gameType());
            case QmlEnums::DataAllowSpectators:
                return QVariant::fromValue(gd->allowSpectators());
            case QmlEnums::DataMaxNumberOfPlayers:
                return QVariant::fromValue( gd->maxNumberOfPlayers());
            case QmlEnums::DataStartMoney:
                return QVariant::fromValue(gd->startMoney());
            case QmlEnums::DataFirstSmallBlind:
                return QVariant::fromValue(gd->firstSmallBlind());
            case QmlEnums::DataRaiseIntervalMode:
                return QVariant::fromValue((int) gd->raiseIntervalMode());
            case QmlEnums::DataRaiseSmallBlindEveryHandsValue:
                return QVariant::fromValue(gd->raiseSmallBlindEveryHandsValue());
            case QmlEnums::DataRaiseSmallBlindEveryMinutesValue:
                return QVariant::fromValue(gd->raiseSmallBlindEveryMinutesValue());
            case QmlEnums::DataRaiseMode:
                return QVariant::fromValue((int) gd->raiseMode());
            case QmlEnums::DataManualBlindsList:
                return QVariant::fromValue(gd->manualBlindsList());
            case QmlEnums::DataAfterManualBlindsMode:
                return QVariant::fromValue((int) gd->afterManualBlindsMode());
            case QmlEnums::DataAfterMBAlwaysRaiseValue:
                return QVariant::fromValue(gd->afterMBAlwaysRaiseValue());
            case QmlEnums::DataGuiSpeed:
                return QVariant::fromValue(gd->guiSpeed());
            case QmlEnums::DataDelayBetweenHandsSec:
                return QVariant::fromValue(gd->delayBetweenHandsSec());
            case QmlEnums::DataPlayerActionTimeoutSec:
                return QVariant::fromValue(gd->playerActionTimeoutSec());
            case QmlEnums::DataDefaultBlinds:
                return QVariant::fromValue(gd->defaultBlinds());
            case QmlEnums::DataGameName:
                return QVariant::fromValue(gd->gameName());
            }
        }
        else {
            switch(role-QmlEnums::GameInfoRole) {
            case QmlEnums::InfoGameName:
                return QVariant::fromValue(gi->name());
            case QmlEnums::InfoGameData:
                return QVariant::fromValue(qobject_cast<QObject*>(gi->gameData()));
            case QmlEnums::InfoGameMode:
                return QVariant::fromValue((int) gi->mode());
            case QmlEnums::InfoAdmin:
                return QVariant::fromValue(gi->adminId());
            case QmlEnums::InfoIsPwProt:
                return QVariant::fromValue(gi->isPasswordProtected());
            }
        }
    }
    return RoleItemModel::data(index, role);
}
