import QtQml
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import mapModel

//! [tableview]
   TableView {
       id: tableView
       anchors.fill: parent

       rowSpacing: 1
       columnSpacing: 1

       ScrollBar.horizontal: ScrollBar {}
       ScrollBar.vertical: ScrollBar {}

       delegate: Rectangle {
           id: cell
           implicitWidth: 15
           implicitHeight: 15

           required property var model
//           required property bool value

           color: value ? "#f3f3f4" : "#b5b7bf"

           MouseArea {
               anchors.fill: parent
               onClicked: parent.model.value = !parent.value
           }
       }
       //! [tableview]

       //! [model]
       model: GameOfLifeModel {
           id: gameOfLifeModel
       }
       //! [model]

       //! [scroll]
       contentX: (contentWidth - width) / 2;
       contentY: (contentHeight - height) / 2;
       //! [scroll]
   }
