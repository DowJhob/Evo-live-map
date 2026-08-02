import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
//window containing the application
ApplicationWindow {
    width: 640
    height: 480
    visible: true
    //title of the application
    title: qsTr("Hello World")

    //menu containing two menu items
    header: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                onTriggered: console.log("Open action triggered")
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Exit")
                onTriggered: Qt.quit()
            }
        }
    }

    //Content Area

    //a button in the middle of the content area
    Button {
        text: qsTr("Hello World")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
