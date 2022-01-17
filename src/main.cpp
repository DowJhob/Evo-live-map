#include <QApplication>
#include <QObject>
//#include <QStatusBar>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "wideband/wb-manager.h"
#include "mainwindow.h"
#include "controller.h"
#include "wblogger.h"
#include "widgets/gauge_widget.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    deviceNativeFilter usbFilter;
    deviceManager devManager;
    protoManager protoManager;
    wbManager wbManager;
    MainWindow mainWindow;
    controller controller;
    wbLogger wbLogger;
    gaugeWidget wbWgt("         = Wideband =         ", 4);

    QObject::connect(&devManager,   &deviceManager::tactrixArrived,   &wbManager,  &wbManager::addTactrix);

    QObject::connect(&wbManager,   &wbManager::wbSelected,            &wbLogger,   &wbLogger::setWB);
    QObject::connect(&wbManager,   &wbManager::protoSelected,         &wbLogger,   &wbLogger::setProto);
    QObject::connect(&wbManager,   &wbManager::wbStart,               &wbLogger,   &wbLogger::start_stop);

    QObject::connect(&wbLogger,   &wbLogger::logReady,                &wbWgt,      &gaugeWidget::display);



    //========================================================================================
    QObject::connect(&usbFilter,    &deviceNativeFilter::deviceEvent, &devManager, &deviceManager::deviceEvent);
    //========================================================================================
    QObject::connect(&devManager,   &deviceManager::deviceSelected,   &controller, &controller::setCommDevice);
    QObject::connect(&protoManager, &protoManager::protoSelected,     &controller, &controller::setProto);
    QObject::connect(&protoManager, &protoManager::logRateChanged,    &controller, &controller::setLogRate);
    //========================================================================================
    //========================================================================================
    QObject::connect(&mainWindow,   &MainWindow::connectECU,          &controller, &controller::connectECU);
    QObject::connect(&mainWindow,   &MainWindow::disConnectECU,       &controller, &controller::disConnectECU);
    QObject::connect(&controller,   &controller::ecu_connected,       &mainWindow, &MainWindow::ecu_connected);
    //========================================================================================
    QObject::connect(&controller,   &controller::create_table,        &mainWindow, &MainWindow::createMap);
    //========================================================================================
    QObject::connect(&mainWindow,   &MainWindow::resetRAM,            &controller, &controller::RAMreset);
    QObject::connect(&mainWindow,   &MainWindow::updateRAM,           &controller, &controller::updateRAM);
    //========================= logger ===============================================================
    QObject::connect(&controller,   &controller::logReady,            &mainWindow, &MainWindow::dataLog//, Qt::DirectConnection
                     );
    //========================================================================================
    QObject::connect(&controller,   &controller::Log,                 &mainWindow, &MainWindow::Log);



    mainWindow.setProtoManager(&protoManager);

    mainWindow.setDeviceManager(&devManager);

    mainWindow.setWBManager(&wbManager);
    mainWindow.setWidebandWidge(&wbWgt);

    //Подписываемся на события
    usbFilter.notifyRegister((HWND)mainWindow.winId());
    usbFilter.getPresentCommDevices();

    //=============================================================================

    app.installNativeEventFilter(&usbFilter);
    controller.start();
    mainWindow.show();
    return app.exec();
}
