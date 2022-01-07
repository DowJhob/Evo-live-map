#include "comm-device-interface.h"

//#include "src/test-map.h"


comm_device_interface::comm_device_interface(QString dllName) : QObject(), dllName(dllName)
{
    qDebug() << "comm_device_interface";
}

comm_device_interface::~comm_device_interface()
{
    qDebug() << "~comm_device_interface";
}
