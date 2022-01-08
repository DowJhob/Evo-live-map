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

#include "widgets/custom_tablewidget.h"

#include "qhexedit/qhexedit.h"

#include "comm-device-interface/comm-device-interface.h"
#include "comm-device-interface/op13.h"
#include "comm-device-interface/op20.h"

#include "enumdev.h"




#include "widgets/gauge_widget.h"
#include "widgets/commParamWidget.h"
#include "widgets/mapWidget/mapwidget.h"

//#include "widgets/mapWidget/mapview.h"
//#include "widgets/mapWidget/mapmodel.h"


//#include "test-map.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QHash<QString, mapWidget*> ptrRAMtables;

    QStringList listFiles;
    QString FirstFile_by_Name = {};
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void deviceEvent(device dev);

    void readyInterface(bool lockFlag);
    void ecu_connected(//QHash<QString, Map *> m
                       );

    void createMap(mapDefinition *dMap);

    void Log(QString str);

protected :

private slots:
    void commDeviceSelected(device dev);
    void _protoSelected(int proto);


    void StartButton_slot();
    void logger_slot();


    void debugButton_slot();


    void on_start_addr_lineEdit_returnPressed();
    void on_count_lineEdit_returnPressed();
    void itemChecks(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    commParamWidget *cpW;

    void gaugeDelete();

    void createMapTree(Map *tab);
    void freeMapTree();

    void create_gauge(QString name, mutParam *param);

    QAction *start_action;
    QAction *ram_reset;

    QAction *logger;

    QAction *debug_action;

    QThread logger_thread;

    ecu_definition *_ecu;
    comm_device_interface *vehicle_ecu_comm = nullptr;

    bool  debug = false;

    QToolBar *loggerWidgetBar = nullptr;

    //======================== widget's =================================
    QHexEdit *hexEdit;
    gauge_widget *tactrix_afr_lcd = nullptr;
    //======================== widget lists =================================
    QSet<gauge_widget*> gauge_widget_set;

    QVector<QColor> colormap;
    void colorFromFile(QString filename);
    void test();

signals:
    void devSelected(device);
    void interfaceRemoved(device);
    void protoSelected(int proto);
    void baudChanged(int);
    void logChanged(int);


    void getECUconnectMainWindow();
    void ecu_test_connect();
    void getECUdisconnectMainWindow();

    void startLogger();
    void stopLogger();

    void _exit();

    void updateRAM(abstractMemoryScaled);
    void RAM_reset();

    void start_WB();
    void setLoggingInterval(int);

    void logReady(QVector<float>);

};
#endif // MAINWINDOW_H
