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
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::devSelected, &controller, &controller::commDeviceSelected);

    QObject::connect(&mainWindow, &MainWindow::interfaceRemoved, &controller, &controller::commDeviceRemoved);

    QObject::connect(&mainWindow, &MainWindow::protoSelected, &controller, &controller::setProto);
    //========================================================================================
    QObject::connect(&controller, &controller::interfaceReady, &mainWindow, &MainWindow::readyInterface);
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::getECUconnectMainWindow, &controller, &controller::getECUconnect);
    QObject::connect(&mainWindow, &MainWindow::getECUdisconnectMainWindow, &controller, &controller::getECUdisconnect);
    //========================================================================================

     QObject::connect(&mainWindow,  &MainWindow::baudChanged, &controller, &controller::baudChanged);
     QObject::connect(&mainWindow,  &MainWindow::logChanged, &controller, &controller::logChanged);


    QObject::connect(&controller, &controller::ecu_connected, &mainWindow, &MainWindow::ecu_connected);
    //========================================================================================
    QObject::connect(&controller, &controller::create_table, &mainWindow, &MainWindow::create_table//, Qt::QueuedConnection
                     );
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::RAM_reset, &controller, &controller::RAMreset);
    QObject::connect(&mainWindow, &MainWindow::updateRAM, &controller, &controller::_updateRAM//, Qt::QueuedConnection
                     );
    //========================================================================================
    QObject::connect(&mainWindow, &MainWindow::startLogger, &controller, &controller::startLogger);
    QObject::connect(&mainWindow, &MainWindow::stopLogger, &controller, &controller::stopLogger);
    QObject::connect(&mainWindow, &MainWindow::setLoggingInterval, &controller, &controller::setLoggingInterval);
    //========================================================================================
    QObject::connect(&controller, &controller::Log, &mainWindow, &MainWindow::Log);



    QObject::connect(&controller, &controller::logReady, &mainWindow, &MainWindow::logReady);

    //Подписываемся на события
    Enumerator.NotifyRegister((HWND)mainWindow.winId());
    Enumerator.getPresentCommDevices();
    //=============================================================================

    app.installNativeEventFilter(&Enumerator);
    controller.start();
    mainWindow.show();
    return app.exec();
}
