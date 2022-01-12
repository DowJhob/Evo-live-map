#include "devicemanager.h"

deviceManager::deviceManager(QWidget *parent):QComboBox(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(this,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &deviceManager::_deviceSelected);
}

void deviceManager::deviceEvent(device dev)
{
    switch (dev.direction)
    {
    case dir::arrive : addDevice(dev); break;
    case dir::remove : removeDevice(dev); break;
    }
}

void deviceManager::addDevice(device dev)
{
    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;
    comm_device_interface *devComm = nullptr;                                  // это важно если бы мы пытались добавить не инициализированную, тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
    switch (dev.type)
    {
    case deviceType::OP13  : devComm = new OP13(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    case deviceType::OP20  : devComm = new OP20(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    case deviceType::J2534 : devComm = new j2534_interface(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }
    //commDeviceStore.insert(dev.DeviceDesc+dev.DeviceUniqueID, devComm);

    addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
}

void deviceManager::removeDevice(device dev)
{
    //comm_device_interface *devComm = commDeviceStore.take(dev.DeviceDesc+dev.DeviceUniqueID);
    //devComm->deleteLater();
    //qDebug()<< "deviceManager::removeDevice count" << count();
    //qDebug()<< "deviceManager::removeDevice start" << dev.DeviceDesc + " / " + dev.DeviceUniqueID;
    int index = findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
    //qDebug()<< "deviceManager::removeDevice start" << index;
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(itemData(index));
    if( devComm != nullptr)
        devComm->deleteLater();

    if( index < count())
        removeItem(index);
    else
        qDebug() << "Error deleting item";

    //emit deviceSelected(nullptr);  // не нужен потому что будет селект с нулем
}

void deviceManager::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected start";
    //qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(itemData(index));
    qDebug()<< "deviceManager::_deviceSelected finish" << devComm;
    //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!
    //if( devComm != nullptr)
    emit deviceSelected(devComm);
}
