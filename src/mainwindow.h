#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <time.h>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include <QLCDNumber>
#include <QTreeWidgetItem>

//#include <QMessageBox>
#include <QStatusBar>

//#include "src/widgets/3D/s3dplot.h"
//#include "src/widgets/3D/s3dscatter.h"
#include "src/widgets/Patcher/patcher.h"
#include "src/widgets/mapManager/mapmanager.h"
#include "types.h"

#include "deviceNativeFilter.h"

#include "widgets/mapWidget/mapwidget.h"
#include "widgets/hexEditor/hexeditor.h"
#include "widgets/gauge_widget.h"

#include "widgets/ecuManager.h"

#include "widgets/loggermanager.h"
#include <qwt3d_surfaceplot.h>

namespace Ui {
class MainWindow;
}

using namespace Qwt3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QHash<QString, mapWidget*> ptrRAMtables;

    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void setUSBfilter(deviceNativeFilter *usbFilter);
    void Log(QString str);

private slots:
    void deviceEventLog(QString msg, int pos);
    void ecuConnected(QHash<QString, Map *> *RAMtables);
    void ecuDisconnected();
//    void createMap(mapDefinition *dMap);
//    void itemChecks(QTreeWidgetItem *item, int column);

private:

//    s3Dplot plot;
//s3Dscatter scat;
    SurfacePlot *pw;

    Ui::MainWindow *ui;

    //======================== widget lists =================================
    QSet<gaugeWidget*> gauge_widget_set;
    //======================== widget's =================================
    ecuManager _ecuManager;
    mapManager _mapManager;
    loggerManager _loggerManager;
    hexEditor hexEdit;
    Patcher patcher;
    QToolBar *loggerWidgetBar = nullptr;

//    QVector<QColor> colormap;

//    void createMapTree(Map *tab);
//    void freeMapTree();

//    void colorFromFile(QString filename);

    void create_gauge(QString name, mutParam *param);
    void gaugeDelete();

signals:
    void _exit();

};
#endif // MAINWINDOW_H
