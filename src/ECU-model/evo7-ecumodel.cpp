#include "evo7-ecumodel.h"
#include "qendian.h"
#include <QDebug>

evo7_ECUmodel::evo7_ECUmodel()
{

}

bool evo7_ECUmodel::connectECU()
{
    qDebug() << "=========== ecuDefinition::connectECU ================" << devComm;
    if (ECUproto->connect())
    {
        QByteArray a = ECUproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки
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
    (*ECUproto->devComm)->close();
    return false;
}

