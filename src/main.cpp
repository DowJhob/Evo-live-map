#include <QApplication>
#include <QObject>
#include <QElapsedTimer>
//#include <QQmlApplicationEngine>

#include "deviceNativeFilter.h"
#include "mainwindow.h"

#include "src/commDevicesController.h"
#include "src/wideband/WB_Obj.h"
#include "widgets/ecuManagerWidget.h"
#include "widgets/mapManager/mapmanager.h"

QElapsedTimer t;

Q_DECLARE_METATYPE( QVector<float> )

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationDisplayName(QString("livemap by eulle@ya.ru ver %1").arg(GIT_VERSION));
    qDebug() << QString("Version: %1").arg(GIT_VERSION);
    //========================================================================================

    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    //    QQmlApplicationEngine engine;

    //    engine.load(QUrl(QStringLiteral("../src/widgets/mapWidget/fff.qml")));
    //    if (engine.rootObjects().isEmpty())
    //        return -1;

    qDebug() << "=========== main:: ================ QThread:" << QThread::currentThread();

    MainWindow mainWindow;


    //========================================================================================
    ecu *ECU = new ecu();
    commDevicesController commDevCtrl(ECU);
    ecuManagerWidget *_ecuManager = new ecuManagerWidget(&mainWindow, ECU, &commDevCtrl);
    _ecuManager->fillECU_Models(ECU->getAvailModels());

    QObject::connect(_ecuManager, &ecuManagerWidget::ECU_DeviceSelected, &commDevCtrl, &commDevicesController::setSelectedECUcommDevice);
    QObject::connect(_ecuManager, &ecuManagerWidget::ecuConnect,          ECU, &ecu::connectDMA, Qt::QueuedConnection);
    QObject::connect(_ecuManager, &ecuManagerWidget::ECU_ModelSelected,   ECU, &ecu::setECUmodelType);
    QObject::connect(_ecuManager, &ecuManagerWidget::ECU_ProtoSelected,   ECU, &ecu::setDMAproto);
    QObject::connect(_ecuManager, &ecuManagerWidget::ECU_deviceHasLeft,   ECU, &ecu::deviceHasLeft);
    QObject::connect(_ecuManager, &ecuManagerWidget::logRateChanged,      ECU, &ecu::setLogRate);


    QObject::connect(ECU, &ecu::getAvailProtos, _ecuManager, &ecuManagerWidget::fillAvailECU_Protos);
    QObject::connect(ECU, &ecu::ecuConnected, _ecuManager, &ecuManagerWidget::ECUconnected, Qt::QueuedConnection);






    mainWindow.setECU(ECU);
    mainWindow.setECUmanager(_ecuManager);


    gaugeWidget wbWgt{"           = Wideband2 =           ", 4};

    //========================================================================================
    // wbManagerWidget *_wbManager = &_ecuManager->commDevsMngrWgt._wbManagerWidget;
    // _wbManager->wb_thread    = wb->thread;
    WB *wb = new WB;


    QObject::connect(&commDevCtrl, &commDevicesController::tactrixArrived,       _ecuManager, &ecuManagerWidget::addTactrix);
    QObject::connect(&commDevCtrl, &commDevicesController::tactrixRemoved,       _ecuManager, &ecuManagerWidget::removeTactrix);

    QObject::connect(_ecuManager, &ecuManagerWidget::wbSelected,       wb, &WB::setWBDev);
    QObject::connect(_ecuManager, &ecuManagerWidget::WB_ProtoSelected, wb, &WB::setWBproto);
    QObject::connect(_ecuManager, &ecuManagerWidget::wbStart,          wb, &WB::start);
    QObject::connect(wb, &WB::lambdaValue,                         &wbWgt, &gaugeWidget::display);

    //========================================================================================

    mainWindow.setWBGUAGE(&wbWgt);

    mainWindow.show();
    return app.exec();
    delete ECU;
}
