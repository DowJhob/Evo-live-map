#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
       MainWindow w;
       qint32 g = 5;
       qDebug() << g;
       qDebug() << &g;

       w.show();
    return app.exec();
}
