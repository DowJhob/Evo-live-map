#include "ecu.h"
#include "src/DMA-proto/pollhelper.h"

ecu::ecu()
{
    readThread = new QThread();
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &ecu::destroyed, readThread, &QThread::quit);
    connect(readThread, &QThread::finished, readThread, &QThread::deleteLater);
    moveToThread(readThread);
    readThread->start();
    qDebug() << "=========== ecu:: ================ QThread:" << readThread;

    writer* _writer = new writer(this);
    connect(this, &ecu::destroyed, _writer, &writer::deleteLater);
    connect(this, &ecu::RAMreset, _writer, &writer::RAMreset, Qt::QueuedConnection);
    connect(this, &ecu::updateRAM, _writer, &writer::updateRAM, Qt::QueuedConnection);
}

ecu::~ecu()
{
    //qDebug() << "=========== ~ecu ================";
    //pollTimer->deleteLater();
}

void ecu::setComDev(comm_device_interface *_devComm)
{
    //    devComm->setParent(nullptr);
    //    devComm->moveToThread(thread());
    if (_devComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        if(DMAproto != nullptr)
        {
            DMAproto->stopLog();
        }
        QThread::msleep(devComm->_readTimeout + 200);   // это что бы вывалиться из цикла ожидания стартового сообщения
        emit removeDevice(devComm);

        emit ecuConnected(false);
    }
    devComm = _devComm;
}

void ecu::setECUmodel(ECU_model *_ECUmodel)
{
    this->ecu_model = _ECUmodel;
    _ECUmodel->setCommDev(&devComm);
    DMAproto->ecu_model = &ecu_model;
}

void ecu::setDMAproto(DMA_proto *_DMAproto)
{
    DMAproto = _DMAproto;
    _DMAproto->setCommDev(&devComm);
}

bool ecu::connectDMA(bool state)
{
    //qDebug() << "=========== ecu::connectDMA ================ devComm:" << devComm;
    if (state)
    {
        if (DMAproto->connect_())
        {
            QByteArray a = DMAproto->directDMAread( ecu_model->ptr_calID, ecu_model->calIDsize);                        //читаем номер калибровки
            if ( !a.isEmpty() )
            {
                QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
                qDebug() << "=========== ecu::connectDMA ================ romID:" << romID;
                emit Log("romID: " + romID);
                if (ecuDef.fromROMID(romID))
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
            emit Log("failure get ECU DMA connect - BaudRate" + QString::number( devComm->getBaudRate()));
    }
    else
    {
        qDebug() << "=========== ecu::disconnectDMA ================";
        DMAproto->stopLog();
        QThread::msleep(1000);               // костыль
        ecuDef.reset();
        emit ecuConnected(false);
    }
    (*DMAproto->devComm)->close();
    return false;
}

void ecu::startLog()
{
    DMAproto->startLog(&ecuDef.ramMut);
}

void ecu::stopLog()
{
    DMAproto->stopLog();
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

writer::writer(ecu *parent) : parent(parent)
{
    writeThread = new QThread();
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &ecu::destroyed, writeThread, &QThread::quit);
    connect(writeThread, &QThread::finished, writeThread, &QThread::deleteLater);
    moveToThread(writeThread);
    writeThread->start();
    qDebug() << "=========== writer::writer ================ QThread:" << writeThread;
}

writer::~writer()
{

}

void writer::RAMreset()
{
//    parent->DMAproto->stopLog();
//    qDebug() << "writer::RAMreset(addr::" << parent->ecuDef.ramMut.DEAD_var << ");";
    quint16 r = 0x0000;
    parent->DMAproto->directDMAwrite(parent->ecuDef.ramMut.DEAD_var, (char*)&r, 2);
//    parent->DMAproto->startLog();
}

void writer::updateRAM(offsetMemory memory)
{
//    parent->DMAproto->stopLog();
    //qDebug()<< "writer::updateRAM" << memory.toHex(':');
    parent->DMAproto->directDMAwrite(memory.addr, memory.data(), memory.size());
//    parent->DMAproto->startLog(&parent->ecuDef.ramMut);
}
