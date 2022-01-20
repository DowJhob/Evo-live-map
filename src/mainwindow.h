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

//#include "comm-device-interface/comm-device-interface.h"
//#include "comm-device-interface/op13.h"
//#include "comm-device-interface/op20.h"

#include "deviceNativeFilter.h"

#include "widgets/maintoolbar.h"
#include "widgets/gauge_widget.h"
#include "widgets/commParamWidget.h"
#include "widgets/mapWidget/mapwidget.h"
#include "widgets/hexEditor/qhexedit/qhexedit.h"
#include "widgets/hexEditor/hexeditor.h"


#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "wideband/wb-manager.h"


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
    void setCPW(commParamWidget *cpW);

    void deviceEvent(comm_device_interface *devComm);

    void ecu_connected();
    void createMap(mapDefinition *dMap);
    void Log(QString str);

private slots:
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
    gaugeWidget wbWgt{"           = Wideband =           ", 4};
    //======================== widget lists =================================
    QSet<gaugeWidget*> gauge_widget_set;

    QVector<QColor> colormap;
    void colorFromFile(QString filename);

signals:
    void connectECU(int);
    void disConnectECU();

    void updateRAM(abstractMemoryScaled);
    void resetRAM();

    void dataLog(QVector<float>);

    void _exit();

};
#endif // MAINWINDOW_H
