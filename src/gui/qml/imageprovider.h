#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    enum ProviderTypes {
        AVATARS = 0,
        CARDS,
        TABLE,
        MISC
    };

    ImageProvider(ImageType type=QQmlImageProviderBase::Image, Flags flags = 0);

    ImageType imageType() {
        return m_type;
    }

    Flags flags() {
        return 0;
    }

    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

private:
    QStringList options;
    QString dataDir;
    ImageType m_type;
};

#endif // IMAGEPROVIDER_H
