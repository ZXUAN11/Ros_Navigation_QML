cmake_minimum_required(VERSION 3.16)

project(Ros_Navigation_QML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Qml
    QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_kdl REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCES
    source/main/main.cpp
    source/rosbridge/rosBridge.h
    source/rosbridge/rosBridge.cpp
    source/rosbridge/cvBridge.h
    source/rosbridge/cvBridge.cpp
    source/quick/imageProvider.h
    source/quick/imageProvider.cpp
)
set(QML_FILES
    source/qml/Main.qml
)
set(RESOURCES resource.qrc)

qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${RESOURCES}
)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
    ${QML_FILES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Qml
    PRIVATE Qt6::QuickControls2
    PRIVATE ${OpenCV_LIBS}
)

ament_target_dependencies(${PROJECT_NAME} PUBLIC
    rclcpp
    rclcpp_action
    nav_msgs
    sensor_msgs
    geometry_msgs
    tf2
    tf2_ros
    tf2_geometry_msgs
    tf2_kdl
    cv_bridge
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main
    ${CMAKE_CURRENT_SOURCE_DIR}/source/rosbridge
    ${CMAKE_CURRENT_SOURCE_DIR}/source/quick)

add_subdirectory(./modules/TranslationHelper)
setup_app_translations(${PROJECT_NAME} ${SOURCES} ${QML_FILES})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
