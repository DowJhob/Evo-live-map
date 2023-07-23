#include "ecu.h"
#include "src/DMA-proto/pollhelper.h"

ecu::ecu()
{
    QThread *this_thread = new QThread();
    //    QObject::connect(this_thread, &QThread::started, this, [this](){
    //        //pollTimer = new QTimer(this);
    //        //pollTimer->setInterval(50);
    //    }
    //    );
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
    //pollTimer->deleteLater();
}

void ecu::setComDev(comm_device_interface *_devComm)
{
    if (devComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        DMAproto->stopLog();
    }
    else
        devComm = _devComm;
}

void ecu::setECUmodel(ECU_FLASH_model *_ECUmodel)
{
    this->_ECU_FLASH_model = _ECUmodel;
    _ECUmodel->setCommDev(&devComm);
}

void ecu::setDMAproto(DMA_proto *_DMAproto)
{
    DMAproto = _DMAproto;
    _DMAproto->setCommDev(&devComm);

}

void ecu::connectDMA(bool state)
{
    if (state)
        _connectDMA();
    else
        disconnectDMA();
}

bool ecu::_connectDMA()
{
    qDebug() << "=========== ecuDefinition::connectECU ================" << devComm;
    if (DMAproto->connect())
    {
        QByteArray a = DMAproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки
        if ( !a.isEmpty() )
        {
            QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
            emit Log("romID: " + romID);
            if (!ecuDef.fromROMID(romID))
            {
                emit ecuConnected(true);
                return true;
            }
            else
            {
                qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
                emit Log("xml not found");
            }
        }
        else
            emit Log("failure get ECU rom id");
    }
    else
        emit Log("failure get ECU connect " + QString::number( devComm->getBaudRate()));
    (*DMAproto->devComm)->close();
    return false;
}

void ecu::disconnectDMA()
{
    qDebug() << "=========== ecuDefinition::disConnectECU ================";
    //pollTimer->stop();
    DMAproto->stopLog();
    QThread::msleep(1000);               // костыль
    devComm->close();
    ecuDef.reset();
//    emit ecuDisconnected();

    emit ecuConnected(false);
}

void ecu::startLog()
{
    DMAproto->startLog(&ecuDef.ramMut);
}

void ecu::stopLog()
{
    DMAproto->stopLog();
}

void ecu::RAMreset()
{
    qDebug() << "ecuDefinition::RAMreset(addr::" << ecuDef.ramMut.DEAD_var << ");";
    quint16 r = 0x0000;
    DMAproto->directDMAwrite(ecuDef.ramMut.DEAD_var, (char*)&r, 2);
    //QMetaObject::invokeMethod(ECUproto, "directDMAwrite", Q_ARG(quint32, ecuDef.ramMut.DEAD_var), Q_ARG(char*, (char*)&r), Q_ARG(int, 2));
}

void ecu::updateRAM(offsetMemory memory)
{
    qDebug()<< "ecuDefinition::updateRAM" << memory.toHex(':');
//    ECUproto->updateRAM(memory);
    DMAproto->directDMAwrite(memory.addr, memory.data(), memory.size());
    //QMetaObject::invokeMethod(ECUproto, "updateRAM", Q_ARG(offsetMemory, memory));
}

mapDefinition *ecu::getMap(Map *declMap)
{
    //qDebug()<<"ecuDefinition::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = DMAproto->directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = DMAproto->directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = DMAproto->directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void ecu::setLogRate(int freqRate)
{
    //pollTimer->setInterval(1/freqRate);
    ((pollHelper*)DMAproto)->setLogRate(1/freqRate);
}

void ecu::test()
{
    //===================================================================================================
    if (!ecuDef.fromROMID("90550001"))
    {
        (*DMAproto->devComm)->close();
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        emit Log("xml not found");
    }
    //==================================================================================================
    emit s_test();
}
