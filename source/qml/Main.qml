import QtQuick
import RosBridge 1.0
import App 1.0

Window {
    width: 800
    height: 600
    visible: true

    Component.onCompleted: {
        Translator.setLanguage(1) // 切换到英文
    }

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
            text: qsTr("坐标: %1, %2").arg(rosbidge.x).arg(rosbidge.y)
        }
    }
}
