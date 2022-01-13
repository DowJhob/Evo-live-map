#include <QApplication>
#include <QObject>
#include <QStatusBar>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "mainwindow.h"
#include "controller.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    deviceNativeFilter Enumerator;
    deviceManager devManager;
    protoManager protoManager;
    MainWindow mainWindow;
    controller controller;

    //========================================================================================
    QObject::connect(&Enumerator,   &deviceNativeFilter::deviceEvent,       &devManager, &deviceManager::deviceEvent);
    //========================================================================================
    QObject::connect(&devManager,   &deviceManager::deviceSelected,         &controller, &controller::setCommDevice);
    QObject::connect(&protoManager, &protoManager::protoSelected,           &controller, &controller::setProto);
    QObject::connect(&protoManager, &protoManager::logRateChanged,          &controller, &controller::setLogRate);
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
    QObject::connect(&mainWindow,   &MainWindow::logChanged,                 &controller, &controller::logChanged);
    QObject::connect(&controller,   &controller::logReady,                   &mainWindow, &MainWindow::logReady//, Qt::DirectConnection
                     );
    //========================================================================================
    QObject::connect(&controller, &controller::Log, &mainWindow, &MainWindow::Log);



    mainWindow.setProtoManager(&protoManager);

    mainWindow.setDeviceManager(&devManager);


    //Подписываемся на события
    Enumerator.notifyRegister((HWND)mainWindow.winId());
    Enumerator.getPresentCommDevices();

    //=============================================================================

    app.installNativeEventFilter(&Enumerator);
    controller.start();
    mainWindow.show();
    return app.exec();
}
