#include "imageprovider.h"
#include "manager.h"
#include "configfile.h"

ImageProvider::ImageProvider(QQmlImageProviderBase::ImageType type, QQmlImageProviderBase::Flags flags)
    : QQuickImageProvider(type, flags), m_type(type)
{
    // Needs to be kept in sync with ProviderTypes
    options << "avatars" << "cards" << "table" << "misc";

    dataDir = QString::fromUtf8(ManagerSingleton::Instance()->getConfig()->readConfigString("AppDataDir").c_str());
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage returnImage;
    QString provider(id.left(id.indexOf("/")));
    QString request(id.mid(id.indexOf("/")+1));
    ProviderTypes providerType(static_cast<ProviderTypes>(options.indexOf(provider)));
    switch(providerType) {
    case AVATARS:
        returnImage = QImage(dataDir + +"gfx/avatars/" + request);
        break;
    case CARDS:
        returnImage = QImage(dataDir + +"gfx/cards/default/" + request);
        break;
    case TABLE:
        returnImage = QImage(dataDir + +"gfx/gui/table/default/" + request);
        break;
    case MISC:
        returnImage = QImage(dataDir + +"gfx/gui/misc/" + request);
        break;
    }
    if(size && !returnImage.isNull()) {
        *size = returnImage.size();
    }
    return returnImage;
}
