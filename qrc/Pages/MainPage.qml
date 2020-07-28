import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "../CommonControls"

Control {
    id: mainPage
    Image {
        source: "../wait_background.jpg"
        anchors {
            left: parent.left;
            right: parent.right;
            verticalCenter: parent.verticalCenter
        }
    }
    Label {
        id: timeLabel
        text: robbery.time
        font.family: "UpheavalPro"
        //color: "red"
        anchors.centerIn: parent
    }
//    background: Image {
//        source: "../wait_background.jpg"
//        anchors {
//            left: parent.left;
//            right: parent.right;
//            verticalCenter: parent.verticalCenter
//        }
//    }


}
