#include <QApplication>
#include <QObject>
#include <QElapsedTimer>
//#include <QQmlApplicationEngine>

#include "deviceNativeFilter.h"
#include "mainwindow.h"

#include "src/wideband/wb.h"
#include "widgets/ecuManager.h"
#include "widgets/mapManager/mapmanager.h"

QElapsedTimer t;

int main(int argc, char *argv[])
{
//    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication app(argc, argv);
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
//    QQmlApplicationEngine engine;

//    engine.load(QUrl(QStringLiteral("../src/widgets/mapWidget/fff.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;




    qDebug() << "=========== main:: ================ QThread:" << QThread::currentThread();

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
    ecuManagerWidget *_ecuManager = new ecuManagerWidget(&mainWindow, ECU);


    _ecuManager->setUSBfilter(&usbFilter);
    QObject::connect(_ecuManager, &ecuManagerWidget::deviceEventLog, &mainWindow, &MainWindow::deviceEventLog);
    QObject::connect(_ecuManager, &ecuManagerWidget::Log,            &mainWindow, &MainWindow::Log);

    //========================================================================================
    mapManager *_mapManager = new mapManager(&mainWindow, ECU);

    //========================================================================================

WB *wb = new WB;
wbManager *_wbManager = &_ecuManager->cpW._wbManager;

QObject::connect(_wbManager, &wbManager::wbSelected, wb, &WB::setWBDev);
QObject::connect(_wbManager, &wbManager::protoSelected, wb, &WB::setWBproto);
QObject::connect(_wbManager, &wbManager::wbStart, wb, &WB::start);


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
