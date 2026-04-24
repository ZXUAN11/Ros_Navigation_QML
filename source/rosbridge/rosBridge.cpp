#include "rosBridge.h"

RosBridge::RosBridge(QObject *parent)
{
    mNode = rclcpp::Node::make_shared("simple_qml_bridge");
    mSub = mNode->create_subscription<nav_msgs::msg::Odometry>(
        "/odom", 10, [this](const nav_msgs::msg::Odometry::SharedPtr msg) {
            mX = msg->pose.pose.position.x;
            mY = msg->pose.pose.position.y;
            emit poseChanged();
        });
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        rclcpp::spin_some(mNode);
    });
    timer->start(10);
}

