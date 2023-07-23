#include "ecumodel.h"

ECUmodel::ECUmodel()
{

}

bool ECUmodel::connectDMA(quint32 romIDaddr, int length)
{
    qDebug() << "=========== ecuDefinition::connectECU ================" << devComm;
    if (ECUproto->connect())
    {
        QByteArray a = ECUproto->directDMAread( romIDaddr, length);                        //читаем номер калибровки
        if ( !a.isEmpty() )
        {
            QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
            lastError = "romID: " + romID;
//            if (!ecuDef.fromROMID(romID))
            {
                return true;
            }
//            else
            {
                qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
                lastError = "xml not found";
            }
        }
        else
            lastError = "failure get ECU rom id";
    }
    else
        lastError = "failure get ECU connect " + QString::number( (*devComm)->getBaudRate());
    (*ECUproto->devComm)->close();
    return false;
}

void ECUmodel::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== ECUmodel::setCommDev ================" << devComm;

    this->devComm = devComm;
}

