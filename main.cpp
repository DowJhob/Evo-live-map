#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
       MainWindow w;
       app.installNativeEventFilter(&w.Enumerator);
       w.show();
    return app.exec();
}
