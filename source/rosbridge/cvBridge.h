#include <QObject>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QImage>

class RosImageProvider; // 前向声明

class CVBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged)
public:
    explicit CVBridge(QObject *parent = nullptr);
    void setImageProvider(RosImageProvider *provider);

signals:
    void imageReceived(const QImage &image);
    void frameCountChanged();

private:
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg);

public:
    int frameCount() const { return mFrameCount; }

private:
    std::shared_ptr<rclcpp::Node> mNode;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr mSub;
    RosImageProvider *mProvider = nullptr;
    int mFrameCount = 0;
};
