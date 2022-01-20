#include <QApplication>
#include <QObject>
//#include <QStatusBar>

#include "deviceNativeFilter.h"
#include "comm-device-interface/devicemanager.h"
#include "DMA-proto/proto-manager.h"
#include "mainwindow.h"
#include "ecuManager.h"
//#include "wblogger.h"
#include "widgets/commParamWidget.h"

#include <QElapsedTimer>

QElapsedTimer t;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationDisplayName(QString("livemap by eulle@ya.ru ver %1").arg(GIT_VERSION));
    qDebug() << QString("Version: %1").arg(GIT_VERSION);
    //========================================================================================
    deviceNativeFilter usbFilter;
    //========================================================================================
    MainWindow mainWindow;
    //========================================================================================
    //========================= logger ===============================================================
    //========================================================================================
    mainWindow.setUSBfilter(&usbFilter);
    //========================================================================================
    //Подписываемся на события
    usbFilter.notifyRegister((HWND)mainWindow.winId());
    usbFilter.getPresentCommDevices();
    //=============================================================================
    app.installNativeEventFilter(&usbFilter);

    mainWindow.show();
    return app.exec();
}
