import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "CameraControls"
import "Pages"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: "Ограбление"

    readonly property int paddingValue: 12

    property bool videoVisible: false;

    MainPage {
        id: mainPage
        //visible: false
        font.pixelSize: 140
        font.capitalization: Font.AllUppercase

        anchors{
            right: parent.right;
            top: parent.top;
            bottom: parent.bottom;//parent.verticalCenter;
            left: parent.left;
        }
        MouseArea {
            onClicked: {
                window.videoVisible = !window.videoVisible;
            }
            anchors.fill: parent
        }
    }
    CamerasPage {
        id: camerasPage
        height: parent.height*0.4
        visible: window.videoVisible
        anchors {
            right: parent.right;
            //top: parent.verticalCenter;
            bottom: parent.bottom;
            left: parent.left;
        }
    }
}
