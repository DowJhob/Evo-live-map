#include "ECU-interface.h"

ECU_interface::ECU_interface(comm_device_interface **devComm) : devComm(devComm)
{
    qDebug() << "ECU_interface";
}

ECU_interface::~ECU_interface()
{
    qDebug() << "~ECU_interface";
}

mapDefinition *ECU_interface::getMap(Map *declMap)
{
    //qDebug()<<"ECU_interface::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void ECU_interface::updateRAM(abstractMemoryScaled memory)
{
    directDMAwrite(memory.addr, memory.data(), memory.size());
    QThread::msleep(50);
}

void ECU_interface::RAMreset(quint32 addr)
{

    qDebug() << "comm_device_interface::RAMreset(addr::" << addr << ");";
    quint16 r = 0x0000;
    directDMAwrite(addr, (char*)&r, 2);
}

void ECU_interface::DMApoll()
{
    //QElapsedTimer t;
    //t.start();
    //sendDMAcomand(DMAcomand::indirectRead, RAM_MUT_addr, RAM_MUT_len);
    auto a = (*devComm)->read();
    emit readyRead(QByteArray(a));
   // qDebug()<<"poll"<<a.toHex(':');
}
