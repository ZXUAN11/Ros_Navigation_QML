#include <QObject>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <QTimer>

class RosBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(double x READ x NOTIFY poseChanged)
    Q_PROPERTY(double y READ y NOTIFY poseChanged)
public:
    explicit RosBridge(QObject *parent = nullptr);

    double x() const { return mX; }
    double y() const { return mY; }

signals:
    void poseChanged();

private:
    std::shared_ptr<rclcpp::Node> mNode;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr mSub;
    double mX = 0;
    double mY = 0;

};
