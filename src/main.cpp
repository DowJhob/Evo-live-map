#include <QApplication>
#include <QObject>
//#include <QStatusBar>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "mainwindow.h"
#include "controller.h"
//#include "wblogger.h"
#include "widgets/commParamWidget.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationDisplayName(QString("livemap by eulle@ya.ru ver %1").arg(GIT_VERSION));
    qDebug() << QString("Version: %1").arg(GIT_VERSION);

    deviceNativeFilter usbFilter;
    //========================================================================================
    commParamWidget cpW;
    //========================================================================================
    MainWindow mainWindow;
    controller controller;
    //========================================================================================
    QObject::connect(&cpW._protoManager, &protoManager::protoSelected,     &controller,     &controller::setProto);
    QObject::connect(&cpW.devManager,    &deviceManager::deviceSelected,   &controller,     &controller::setCommDevice);
    QObject::connect(&cpW._protoManager, &protoManager::logRateChanged,    &controller,     &controller::setLogRate);
    QObject::connect(&usbFilter,         &deviceNativeFilter::deviceEvent, &cpW.devManager, &deviceManager::deviceEvent);

cpW._protoManager.addProtos();   // костыль пока

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
    QObject::connect(&controller,   &controller::logReady,            &mainWindow, &MainWindow::dataLog);
    //========================================================================================
    QObject::connect(&controller,   &controller::Log,                 &mainWindow, &MainWindow::Log);

    mainWindow.setCPW(&cpW);

    //Подписываемся на события
    usbFilter.notifyRegister((HWND)mainWindow.winId());
    usbFilter.getPresentCommDevices();

    //=============================================================================

    app.installNativeEventFilter(&usbFilter);
    controller.start();
    mainWindow.show();
    return app.exec();
}
