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



#include "types.h"

#include "comm-device-interface/comm-device-interface.h"
#include "comm-device-interface/op13.h"
#include "comm-device-interface/op20.h"

#include "enumdev.h"

#include "widgets/maintoolbar.h"
#include "widgets/gauge_widget.h"
#include "widgets/commParamWidget.h"
#include "widgets/mapWidget/mapwidget.h"
#include "widgets/hexEditor/qhexedit/qhexedit.h"
#include "widgets/hexEditor/hexeditor.h"

namespace Ui {
class MainWindow;
}

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
    void deviceEvent(device dev);

    void lockInterface(bool lockFlag);
    void ecu_connected();

    void createMap(mapDefinition *dMap);

    void Log(QString str);

private slots:
    void commDeviceSelected(device dev);
    void _protoSelected(int proto);

    void StartButton_slot();

    void itemChecks(QTreeWidgetItem *item, int column);

private:
    QString start_action= "Start";
    Ui::MainWindow *ui;
    mainToolBar *_mainToolBar;
    commParamWidget *cpW;

    void createMapTree(Map *tab);
    void freeMapTree();

    void create_gauge(QString name, mutParam *param);
    void gaugeDelete();

    QToolBar *loggerWidgetBar = nullptr;

    //======================== widget's =================================
    hexEditor *hexEdit;
    gauge_widget *tactrix_afr_lcd = nullptr;
    //======================== widget lists =================================
    QSet<gauge_widget*> gauge_widget_set;

    QVector<QColor> colormap;
    void colorFromFile(QString filename);

signals:
    void devSelected(device);
    void interfaceRemoved(device);
    void protoSelected(int proto);
    //void baudChanged(int);
    void logChanged(int);

    void getECUconnectMainWindow(int);
    void getECUdisconnectMainWindow();

    void updateRAM(abstractMemoryScaled);
    void resetRAM();

    void logReady(QVector<float>);

    void _exit();

};
#endif // MAINWINDOW_H
