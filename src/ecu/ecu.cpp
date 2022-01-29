#include "ecu.h"

ecu::ecu()
{
    QThread *this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, [this](){
        pollTimer = new QTimer(this);
        pollTimer->setInterval(50);
    }
    );
    //    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &ecu::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    //connect(this, &ecuDefinition::destroyed, pollTimer, &QTimer::deleteLater);            // Когда удалим объект остановим поток
    connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);
    this_thread->start();
}

ecu::~ecu()
{
    qDebug() << "=========== ~ecu ================";
    pollTimer->deleteLater();
}

void ecu::setComDev(comm_device_interface *_devComm)
{
    devComm = _devComm;

    if (devComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        pollTimer->stop();
    }
    else
        ECUproto->setCommDev(&devComm);
}

void ecu::setDMAproto(DMA_proto *_ECUproto)
{
    ECUproto = _ECUproto;
    _ECUproto->setCommDev(&devComm);
    connect(pollTimer, &QTimer::timeout, this, &ecu::poll, Qt::QueuedConnection);
}

bool ecu::connectECU()
{
    qDebug() << "=========== ecuDefinition::connectECU ================" << devComm;
    if (!ECUproto->connect())
    {
        emit Log("failure get ECU connect " + QString::number( devComm->getBaudRate()));
        return false;
    }
    QByteArray a = ECUproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки
    if ( a.isEmpty() )
    {
        emit Log("failure get ECU rom id");
        return false;
    }
    QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
    emit Log("romID: " + romID);
    //emit Log("CurrDir: " + QApplication::applicationDirPath());
    //===================================================================================================
    if (!ecuDef.fromROMID(romID))
    {
        (*ECUproto->devComm)->close();
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        emit Log("xml not found");
        return false;
    }
    //==================================================================================================
    emit ecuConnected();
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(ecuDef.RAMtables) )
    {
        emit createMap( getMap(tab) );
    }
    startLog();
    return true;
}

void ecu::disConnectECU()
{
    qDebug() << "=========== ecuDefinition::disConnectECU ================";
    pollTimer->stop();
    QThread::msleep(1000);               // костыль
    devComm->close();
    ecuDef.reset();
    emit ecuDisconnected();
}

void ecu::RAMreset()
{
    qDebug() << "ecuDefinition::RAMreset(addr::" << ecuDef.DEAD_var << ");";
    quint16 r = 0x0000;
    ECUproto->directDMAwrite(ecuDef.DEAD_var, (char*)&r, 2);
}

void ecu::updateRAM(offsetMemory memory)
{
    qDebug()<< "ecuDefinition::updateRAM" << memory.toHex(':');
    ECUproto->directDMAwrite(memory);
}

mapDefinition *ecu::getMap(Map *declMap)
{
    //qDebug()<<"ecuDefinition::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = ECUproto->directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = ECUproto->directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = ECUproto->directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void ecu::startLog()
{
    qDebug()<<"=========== ecuDefinition::startLog ================" << ecuDef.RAM_MUT.size();
    scaledRAM_MUTvalue.resize(ecuDef.RAM_MUT.size());
    pollTimer->start();
}

void ecu::poll()
{
    //qDebug() << "jcsbanksDMA::poll" ;
    offsetMemory a = ECUproto->indirectDMAread(ecuDef.RAM_MUT_addr, ecuDef.RAM_MUT_size);
    qDebug() << "ecu::poll" << a.toHex(':') << a.size();
    a[0] = abs(QCursor::pos().x())/10;
    a[1] = abs(QCursor::pos().y())/6;
    for( int i = 0; i < ecuDef.RAM_MUT.size() ; i++  )
    {
        scaledRAM_MUTvalue[i] = a.toFloatOffset( &ecuDef.RAM_MUT[i].scaling, ecuDef.RAM_MUT[i].offset );
    }
    emit logReady(scaledRAM_MUTvalue);
}
