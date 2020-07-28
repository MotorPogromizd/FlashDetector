import QtQuick 2.10
import QtQuick.Controls 2.2
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

import CustomGeometry 1.0

import "../CameraControls"
import "../CommonControls"

Page {
    VideoViewer {
        id: video0
        anchors {
            left: parent.left;
            top: parent.top;
            bottom: parent.bottom;
            right: parent.horizontalCenter;
        }
        source: camera0Viewer
    }
    VideoViewer {
        id: video1
        anchors {
            right: parent.right;
            top: parent.top;
            bottom: parent.bottom;
            left: parent.horizontalCenter;
        }
        source: camera1Viewer
    }
}
