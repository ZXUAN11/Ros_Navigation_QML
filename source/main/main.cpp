#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <rclcpp/rclcpp.hpp>
#include "rosBridge.h"
#include "cvBridge.h"
#include "imageProvider.h"

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImageProvider("rosImage", RosImageProvider::getInstance());

    qmlRegisterType<RosBridge>("RosBridge", 1, 0, "RosBridge");
    qmlRegisterType<CVBridge>("RosBridge", 1, 0, "CVBridge");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Ros_Navigation_QML", "Main");

    int ret = app.exec();

    rclcpp::shutdown();

    return ret;
}
