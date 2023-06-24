#include <QApplication>
#include <QObject>
#include <QElapsedTimer>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "mainwindow.h"

#include "widgets/commParamWidget.h"
#include "DMA-proto/proto-manager.h"


QElapsedTimer t;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    app.setApplicationDisplayName(QString("livemap by eulle@ya.ru ver %1").arg(GIT_VERSION));
    qDebug() << QString("Version: %1").arg(GIT_VERSION);
    //========================================================================================

    MainWindow mainWindow;

    deviceNativeFilter usbFilter;

    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<offsetMemory>("offsetMemory");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    ecu *ECU = new ecu();

    //========================================================================================
    ecuManager *_ecuManager = new ecuManager(&mainWindow, ECU);
    _ecuManager->setUSBfilter(&usbFilter);
    QObject::connect(_ecuManager, &ecuManager::deviceEventLog, &mainWindow, &MainWindow::deviceEventLog);
    QObject::connect(_ecuManager, &ecuManager::Log,            &mainWindow, &MainWindow::Log);

    //========================================================================================
    mapManager *_mapManager = new mapManager(&mainWindow, ECU);

    //========================================================================================


    //========================================================================================
    mainWindow.setECUmanager(_ecuManager);
    mainWindow.setMAPmanager(_mapManager);

    //========================================================================================
    usbFilter.notifyRegister((HWND)mainWindow.winId());
    usbFilter.getPresentCommDevices();

    //========================= Подписываемся на события ====================================================
    app.installNativeEventFilter(&usbFilter);

    mainWindow.show();
    return app.exec();
}
