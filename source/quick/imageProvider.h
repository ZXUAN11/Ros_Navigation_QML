#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class RosImageProvider : public QQuickImageProvider
{
public:
    RosImageProvider();

    static RosImageProvider* getInstance() {
        static RosImageProvider* instance = nullptr;
        if (!instance) instance = new RosImageProvider();
        return instance;
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void updateImage(const QImage &image);

private:
    QImage mImage;
    QMutex mMutex;
};

#endif // IMAGEPROVIDER_H
