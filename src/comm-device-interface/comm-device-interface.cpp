#include "comm-device-interface.h"

//#include "src/test-map.h"


comm_device_interface::comm_device_interface(QObject *parent, QString dllName, QString DeviceDesc, QString DeviceUniqueID) : QObject(parent),
    dllName(dllName), DeviceUniqueID(DeviceUniqueID), DeviceDesc(DeviceDesc)
{
    //qDebug() << "comm_device_interface";
}

comm_device_interface::~comm_device_interface()
{
    //qDebug() << "~comm_device_interface";
}

bool comm_device_interface::connect()
{
    qDebug() << "=========== comm_device_interface::connect ================";
    bool status = false;

    switch (protocol) {
    case Protocol::ISO9141 :
    case Protocol::ISO9141_CH1 :
    case Protocol::ISO9141_CH2 :
    case Protocol::ISO9141_CH3 :  status = ISO9141(); break;
    case Protocol::ISO15765 :  status = ISO15765(); break;
    case Protocol::ISO14230 :
    default : qDebug() << "comm_device_interface::connect :: not realize protocol" << (ulong)protocol; break;
    }

    return status;
}

void comm_device_interface::setBaudRate(uint BaudRate)
{
    baudRate = BaudRate;
}

uint comm_device_interface::getBaudRate()
{
    return baudRate;
}
