import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "Pages"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: "Flash detector"

    readonly property int paddingValue: 12

    CamerasPage {
        id: camerasPage
        anchors {
            right: parent.right;
            bottom: parent.bottom;
            top: parent.top;
            left: parent.left;
        }
    }
}
