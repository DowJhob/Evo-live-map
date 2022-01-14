#include <QApplication>
#include <QObject>
#include <QStatusBar>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "wideband/wb-manager.h"
#include "mainwindow.h"
#include "controller.h"

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

    //========================================================================================
    QObject::connect(&usbFilter,    &deviceNativeFilter::deviceEvent,        &devManager, &deviceManager::deviceEvent);
    //========================================================================================
    QObject::connect(&devManager,   &deviceManager::deviceSelected,          &controller, &controller::setCommDevice);
    QObject::connect(&protoManager, &protoManager::protoSelected,            &controller, &controller::setProto);
    QObject::connect(&protoManager, &protoManager::logRateChanged,           &controller, &controller::setLogRate);
    //========================================================================================
    //========================================================================================
    QObject::connect(&mainWindow,   &MainWindow::getECUconnectMainWindow,    &controller, &controller::getECUconnect);
    QObject::connect(&mainWindow,   &MainWindow::getECUdisconnectMainWindow, &controller, &controller::getECUdisconnect);
    QObject::connect(&controller,   &controller::ecu_connected,              &mainWindow, &MainWindow::ecu_connected);
    //========================================================================================
    QObject::connect(&controller,   &controller::create_table,               &mainWindow, &MainWindow::createMap);
    //========================================================================================
    QObject::connect(&mainWindow,   &MainWindow::resetRAM,                   &controller, &controller::RAMreset);
    QObject::connect(&mainWindow,   &MainWindow::updateRAM,                  &controller, &controller::updateRAM);
    //========================= logger ===============================================================
    QObject::connect(&controller,   &controller::logReady,                   &mainWindow, &MainWindow::logReady//, Qt::DirectConnection
                     );
    //========================================================================================
    QObject::connect(&controller, &controller::Log, &mainWindow, &MainWindow::Log);



    mainWindow.setProtoManager(&protoManager);

    mainWindow.setDeviceManager(&devManager);

    mainWindow.setWBManager(&wbManager);


    //Подписываемся на события
    usbFilter.notifyRegister((HWND)mainWindow.winId());
    usbFilter.getPresentCommDevices();

    //=============================================================================

    app.installNativeEventFilter(&usbFilter);
    controller.start();
    mainWindow.show();
    return app.exec();
}
