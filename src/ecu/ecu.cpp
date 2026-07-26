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

    // for(auto _ECUmodel : qAsConst(AvailModels))
    // {
    //     _ECUmodel->moveToThread(readThread);
    // }

    // for(auto proto : qAsConst(availProtos))
    // {
    //     proto->moveToThread(readThread);
    // }

    qDebug() << "=========== ecu:: ================ QThread::readThread" << readThread << "  \\  QThread::thread" << thread();
}

ecu::~ecu()
{
    //qDebug() << "=========== ~ecu ================";
    //pollTimer->deleteLater();
}

// QMap<ecuModelType, ECU_model *> *ecu::getAvailModels()
// {
//     return &AvailModels;
// }

// QMap<DMA_ProtoType, DMA_proto *> *ecu::getAvailProtos()
// {
//     return &availProtos;
// }

void ecu::setComDev(comm_device_interface *_devComm)
{
    if (selectedDMAproto != nullptr  )
        selectedDMAproto->stopLog();

    if (selectedDevComm != nullptr  )
    {
        selectedDMAproto->disconnect_();
    }

    emit ecuConnected(false);

    selectedDevComm = _devComm;
}

void ecu::setECUmodel(ECU_model *_ECUmodel)
{
    if (selectedDMAproto != nullptr  )
    {
        selectedDMAproto->stopLog();
        selectedDMAproto->disconnect_();
    }
    selectedECUmodel = _ECUmodel;
}

void ecu::setECUmodelType(ecuModelType _ECUmodelType)
{
    if (selectedECUmodel != nullptr  )
    {
        if (selectedECUmodel->type == _ECUmodelType  )
        {
            return;
        }
        selectedECUmodel->deleteLater();
    }


    switch (_ECUmodelType) {
    case ecuModelType::EVO7_9_ECU_Model:
        selectedECUmodel = new evo7_ECUmodel(this);
        break;
    case ecuModelType::EVO_X_ECU_Model:
        selectedECUmodel = new evoX_ECUmodel(this);
        break;
    default:
        break;

    }

    auto a = selectedECUmodel->availProtos;
    auto b = &a;

    qDebug() << "=========== ecu::setECUmodelType ================" << b;

    emit getAvailProtos(&(selectedECUmodel->availProtos));
}

void ecu::setDMAproto(DMA_ProtoType _DMAprotoType)
{
    if (selectedDMAproto != nullptr  )
    {
        selectedDMAproto->stopLog();
        selectedDMAproto->disconnect_();

        if (selectedDMAproto->type == _DMAprotoType  )
        {
            return;
        }
        selectedDMAproto->deleteLater();
    }

    switch (_DMAprotoType) {
    case DMA_ProtoType::jcsbanks:
        selectedDMAproto = new jcsbanksDMA(&selectedDevComm);
        break;
    case DMA_ProtoType::nanner55:
        selectedDMAproto = new stockDMA(&selectedDevComm);
        break;
    case DMA_ProtoType::tephraX:
        selectedDMAproto = new stockDMA(&selectedDevComm);
        break;
    default:
        break;
    }

    selectedDMAprotoType = _DMAprotoType;

    // qDebug() << "=========== ecu::setDMAproto ================ _DMAproto" << _DMAproto << "  /  _DMAproto::thread" << _DMAproto->thread();
}

void ecu::deviceHasLeft(comm_device_interface *_devComm)
{
    selectedDMAproto->stopLog();

    delete _devComm;
}

bool ecu::connectDMA(bool state)
{
    //qDebug() << "=========== ecu::connectDMA ================ devComm:" << devComm;
    if (state)
    {
        if (selectedDMAproto->connect_())
        {
            QByteArray a = selectedDMAproto->directDMAread( selectedECUmodel->ptr_calID, selectedECUmodel->calIDsize);                        //читаем номер калибровки
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
            emit Log("failure get ECU DMA connect");
    }
    else
    {
        qDebug() << "=========== ecu::disconnectDMA ================";
        selectedDMAproto->stopLog();
        QThread::msleep(1000);               // костыль
        ecuDef.reset();
        // ecu_model->ecuDef.reset();
        emit ecuConnected(false);
    }
    selectedDMAproto->disconnect_();
    return false;
}

void ecu::startLog()
{
    selectedDMAproto->startLog(&ecuDef.ramMut);
    // DMAproto->startLog(&ecu_model->ecuDef.ramMut);
}

void ecu::stopLog()
{
    selectedDMAproto->stopLog();
}

void ecu::updateRAM(offsetMemory memory)
{
    //    qDebug() << "=========== ecu::updateRAM ================ sender()->thread:" << sender()->thread();

    selectedDMAproto->updateRAM(memory);
}

void ecu::RAMreset()
{
    selectedDMAproto->RAMreset(ecuDef.ramMut.DEAD_var, 0);
    // DMAproto->RAMreset(ecu_model->ecuDef.ramMut.DEAD_var, 0);
}

mapDefinition *ecu::getMap(Map *declMap)
{
    //qDebug()<<"ecuDefinition::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = selectedDMAproto->directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = selectedDMAproto->directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = selectedDMAproto->directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void ecu::setLogRate(int freqRate)
{
    //pollTimer->setInterval(1/freqRate);
    ((pollHelper*)selectedDMAproto)->setLogRate(1/freqRate);
}

void ecu::test()
{
    //===================================================================================================
    if (!ecuDef.fromROMID("90550001"))
    {
        selectedDMAproto->disconnect_();
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        emit Log("xml not found");
    }
    //==================================================================================================
    emit s_test();
}
