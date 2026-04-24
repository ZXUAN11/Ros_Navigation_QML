#include "imageProvider.h"

RosImageProvider::RosImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    mImage = QImage(640, 480, QImage::Format_RGB888);
    mImage.fill(Qt::black);
}

QImage RosImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    QMutexLocker locker(&mMutex);
    
    if (size)
        *size = mImage.size();

    if (requestedSize.isValid())
        return mImage.scaled(requestedSize, Qt::KeepAspectRatio);

    return mImage;
}

void RosImageProvider::updateImage(const QImage &image)
{
    QMutexLocker locker(&mMutex);
    mImage = image;
}
