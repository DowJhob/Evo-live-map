#include <QApplication>
#include <QObject>
#include <QStatusBar>

#include "mainwindow.h"
#include "enumdev.h"
#include "controller.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    controller controller;
    enumerator Enumerator;
    MainWindow mainWindow;

    //========================================================================================
    QObject::connect(&Enumerator, &enumerator::commDeviceEvent, &mainWindow, &MainWindow::deviceEvent);
    QObject::connect(&Enumerator, &enumerator::Log, &mainWindow, &MainWindow::Log);
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::devSelected, &controller, &controller::commDeviceSelected);
    QObject::connect(&mainWindow, &MainWindow::interfaceRemoved, &controller, &controller::commDeviceRemoved);
    QObject::connect(&mainWindow, &MainWindow::protoSelected, &controller, &controller::setProto);
    QObject::connect(&controller, &controller::getWB, &mainWindow, &MainWindow::createWB);
    //========================================================================================
    QObject::connect(&controller, &controller::interfaceReady, &mainWindow, &MainWindow::lockInterface);
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
    QObject::connect(&controller, &controller::logReady,    &mainWindow, &MainWindow::logReady);
    //========================================================================================
    QObject::connect(&controller, &controller::Log, &mainWindow, &MainWindow::Log);




    //Подписываемся на события
    Enumerator.NotifyRegister((HWND)mainWindow.winId());
    Enumerator.getPresentCommDevices();
    //=============================================================================

    app.installNativeEventFilter(&Enumerator);
    controller.start();
    mainWindow.show();
    return app.exec();
}
