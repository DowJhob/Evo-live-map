#include "evo7-ecu-flash-model.h"
#include "qendian.h"
#include <QDebug>

evo7_ECUmodel::evo7_ECUmodel()
{
    ptr_calID = 0x0F52;
    calIDsize = 4;
}

bool evo7_ECUmodel::connect()
{
    if( (*devComm)->ISO9141() )
    {
        qDebug() << "=========== evo7_ECUmodel::connect ================";
        return true;
    }
    (*devComm)->close();
    return false;
}

bool evo7_ECUmodel::MUTconnect()
{
    if( (*devComm)->ISO9141() )
        if ( (*devComm)->five_baud_init() )
        {
            qDebug() << "=========== evo7_ECUmodel::MUTconnect ================";
            return true;
        }
    (*devComm)->close();
    return false;
}

//void evo7_ECUmodel::disconnect()
//{

//}

//int evo7_ECUmodel::writeFLASH(quint32 addr, int lenght)
//{

//}

//QByteArray evo7_ECUmodel::readFLASH(quint32 addr, int len)
//{

//}

