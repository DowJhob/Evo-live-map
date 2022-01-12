#include <QApplication>
#include <QObject>
#include <QStatusBar>

#include "enumdev.h"
#include "comm-device-interface/devicemanager.h"
#include "mainwindow.h"
#include "controller.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    enumerator Enumerator;
    deviceManager devManager;
    MainWindow mainWindow;
    controller controller;
    mainWindow.setDeviceManager(&devManager);

    //========================================================================================
    QObject::connect(&Enumerator, &enumerator::deviceEvent, &devManager, &deviceManager::deviceEvent);
    //QObject::connect(&Enumerator, &enumerator::Log, &mainWindow, &MainWindow::Log);
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::deviceSelected, &controller, &controller::commDeviceSelected);
    QObject::connect(&mainWindow, &MainWindow::protoSelected, &controller, &controller::setProto);
    QObject::connect(&controller, &controller::getWB, &mainWindow, &MainWindow::createWB);
    //========================================================================================
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::getECUconnectMainWindow, &controller, &controller::getECUconnect);
    QObject::connect(&mainWindow, &MainWindow::getECUdisconnectMainWindow, &controller, &controller::getECUdisconnect);
    QObject::connect(&controller, &controller::ecu_connected, &mainWindow, &MainWindow::ecu_connected);
    //========================================================================================
    QObject::connect(&controller, &controller::create_table, &mainWindow, &MainWindow::createMap);
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::resetRAM, &controller, &controller::RAMreset);
    QObject::connect(&mainWindow, &MainWindow::updateRAM, &controller, &controller::updateRAM);
    //========================= logger ===============================================================
    QObject::connect(&mainWindow, &MainWindow::logChanged,  &controller, &controller::logChanged);
    QObject::connect(&controller, &controller::logReady,    &mainWindow, &MainWindow::logReady//, Qt::DirectConnection
                     );
    //========================================================================================
    QObject::connect(&controller, &controller::Log, &mainWindow, &MainWindow::Log);




    //Подписываемся на события
    Enumerator.notifyRegister((HWND)mainWindow.winId());
    Enumerator.getPresentCommDevices();
    //=============================================================================

    app.installNativeEventFilter(&Enumerator);
    controller.start();
    mainWindow.show();
    return app.exec();
}
