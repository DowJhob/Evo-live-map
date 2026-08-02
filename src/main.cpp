#include <QApplication>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlApplicationEngine>
#include <QQuickView>

// #include "deviceNativeFilter.h"
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


   // //    QQmlApplicationEngine engine;

   // // engine.load(QUrl(QStringLiteral("../src/widgets/mapWidget/fff.qml")));
   // //    engine.load(QUrl(QStringLiteral("../src/widgets/mapWidget/rrr.qml")));

//     QQuickView view;
//     view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
//     view.setSource(QUrl(QStringLiteral("qrc:/fff.qml")));
// view.show();



    //    if (engine.rootObjects().isEmpty())
    //        return -1;

    qDebug() << "=========== main:: ================ QThread:" << QThread::currentThread();

    //========================================================================================
    ecu *ECU = new ecu();

    MainWindow mainWindow(nullptr, ECU);

    commDevicesController commDevCtrl(ECU);
    ecuManagerWidget *_ecuManager = new ecuManagerWidget(&mainWindow, ECU, &commDevCtrl);
    // _ecuManager->fillECU_Models(ECU->getAvailModels());

    mainWindow.setECUmanager(_ecuManager);

    //========================================================================================
    gaugeWidget wbWgt{"           = Wideband2 =           ", 4, &mainWindow};
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
}
