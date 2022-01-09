#include "log-reader.h"

j2534listener::j2534listener(QObject *parent, comm_device_interface **devComm, DMA_proto **ECUproto) : QThread{parent}, devComm(devComm), ECUproto(ECUproto)
{

    //qDebug() << "j2534listener::j2534listener thread: " << thread();
    //connect(this, &QThread::started, this, &j2534listener::loop);
    //connect(this, &j2534listener::loop, this, &j2534listener::read);
}

void j2534listener::run()
{
    while (1) {
        read();
    }
    exec();
}

void j2534listener::read()
{
    //tt.start();
    abstractMemoryScaled a;
    //qDebug() << "j2534listener::read";
    if(*devComm != nullptr)
    {
         a = (*devComm)->read();
        //qDebug() << "j2534listener::read" << a.toHex(':');
        if(a.size() >0)
        {
            emit logReady(a);
            QCoreApplication::processEvents();
        }
    }
    //qDebug() << "j2534listener::read: " << QString::number( tt.nsecsElapsed()/1000000.0) << a.toHex(':');
}
