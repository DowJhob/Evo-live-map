#include "mainwindow.h"
#include "enumdev.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    enumerator Enumerator;
    MainWindow w;

    //=============================================================================
    QObject::connect(&Enumerator, SIGNAL(InterfaceActive(int, TCHAR*, bool)), &w, SLOT(dll_connect(int, TCHAR*, bool)));
    QObject::connect(&Enumerator, SIGNAL(disconnectInterface()), &w, SLOT(dll_disconnect()));
    QObject::connect(&Enumerator, SIGNAL(Log(QString)), (QObject*)w.statusBar(), SLOT(showMessage(QString)));
    //Подписываемся на события нет нужды в подписке WM_change broadcast!
    Enumerator.NotifyRegister((HWND)w.winId());
    Enumerator.enumerateUSB_Device_by_guid();
    //=============================================================================

    app.installNativeEventFilter(&Enumerator);
    w.show();
    return app.exec();
}
