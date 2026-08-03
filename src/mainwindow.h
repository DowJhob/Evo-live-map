#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <time.h>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include <QLCDNumber>
#include <QTreeWidgetItem>

#include <QStatusBar>

//#include "src/widgets/3D/s3dplot.h"
//#include "src/widgets/3D/s3dscatter.h"
#include "qlistwidget.h"
#include "src/widgets/Patcher/patcher.h"
//#include "src/widgets/analizer.h"
#include "src/widgets/mapManager/mapmanager.h"
// #include "types.h"

#include "widgets/mapWidget/mapwidget.h"
#include "widgets/hexEditor/hexeditor.h"
#include "widgets/gauge_widget.h"

// #include "widgets/ecuManagerWidget.h"/

#include "widgets/loggermanager.h"


namespace Ui {
class MainWindow;
}

//using namespace Qwt3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QHash<QString, mapWidget*> ptrRAMtables;
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr, ecu *ECU = nullptr);
    void setECUmanager(QToolBar *_ecuManager);
    static QListWidget *systemLog;

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void deviceEventLog(QString msg, int pos);
    void setWBGUAGE(gaugeWidget *wbWgt);

private slots:

private:

    //    s3Dplot plot;
    //s3Dscatter scat;
    //    Analizer an;
    ecu *ECU = nullptr;
    Ui::MainWindow *ui;

    //======================== widget lists =================================
    // QSet<gaugeWidget*> gauge_widget_set;
    //======================== widget's =================================
    // ecuManagerWidget *_ecuManager = nullptr;
    mapManager *_mapManager = nullptr;
    loggerManager _loggerManager;
    hexEditor hexEdit;
    Patcher patcher;
    QToolBar *loggerWidgetBar = nullptr;

signals:
    void _exit();

};
#endif // MAINWINDOW_H
