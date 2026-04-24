import QtQuick
import RosBridge 1.0

Window {
    width: 800
    height: 600
    visible: true

    CVBridge {
        id: cvBridge
    }

    Image {
        id: cameraDisplay
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        
        source: "image://rosImage/camera_feed?id=" + cvBridge.frameCount
        
        cache: false // 禁用本地缓存

        Text {
            RosBridge { id: rosbidge }
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: "坐标: " + rosbidge.x + ", " + rosbidge.y
        }
    }
}
