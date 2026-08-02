// import QtQml 2.15
// import QtQuick 2.15
// import QtQuick.Window 2.15
import QtQuick.Controls 1.4
// import QtQuick.Layouts 2.15
// import mapModel
// import Qt.labs.qmlmodels 1.0
//! [tableview]
TableView {
    TableViewColumn { role: "title"; title: "Title"; width: 100 }
    TableViewColumn { role: "author"; title: "Author"; width: 200 }
    // model: libraryModel
}
