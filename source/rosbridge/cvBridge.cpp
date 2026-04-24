#include "cvBridge.h"
#include "imageProvider.h"

CVBridge::CVBridge(QObject *parent) : QObject(parent)
{
    mProvider = RosImageProvider::getInstance();
    mNode = rclcpp::Node::make_shared("cv_bridge_node");
    
    mSub = mNode->create_subscription<sensor_msgs::msg::Image>(
        "/camera_sensor/image_raw", rclcpp::SensorDataQoS(),
        std::bind(&CVBridge::imageCallback, this, std::placeholders::_1));

    // 使用定时器处理 ROS spin
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        rclcpp::spin_some(mNode);
    });
    timer->start(30);
}

void CVBridge::setImageProvider(RosImageProvider *provider)
{
    mProvider = provider;
}

void CVBridge::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        
        cv::Mat bgr_frame = cv_ptr->image;
        cv::Mat rgb_frame;
        cv::cvtColor(bgr_frame, rgb_frame, cv::COLOR_BGR2RGB);

        QImage qimg(rgb_frame.data, rgb_frame.cols, rgb_frame.rows, 
                    rgb_frame.step, QImage::Format_RGB888);
        
        QImage copiedImg = qimg.copy();
        
        if (mProvider) {
            mProvider->updateImage(copiedImg);
        }

        mFrameCount++;
        emit frameCountChanged();
        emit imageReceived(copiedImg);

    }
    catch (cv_bridge::Exception &e) {
        RCLCPP_ERROR(mNode->get_logger(), "cv_bridge exception: %s", e.what());
    }
}
