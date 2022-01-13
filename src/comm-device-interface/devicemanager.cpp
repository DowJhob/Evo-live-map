#include "devicemanager.h"

deviceManager::deviceManager(QWidget *parent):QGroupBox(parent)
{
    setTitle("Communication devices");
    setLayout(&layout);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    /*
    availCommDev.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    bd.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    el_baudRate.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);*/

    layout.addWidget(&availCommDev, 0, 0);
    layout.addWidget(&bd, 0, 1);
    layout.addWidget(&el_baudRate, 0, 2);

    connect(&availCommDev,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &deviceManager::_deviceSelected);
    connect(&el_baudRate,  &QLineEdit::editingFinished, this, &deviceManager::baudRateUpdate);
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
    devComm->setBaudRate(el_baudRate.text().toUInt());
    connect(this,  &deviceManager::baudRateChanged, devComm, &comm_device_interface::setBaudRate);

    availCommDev.addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
}

void deviceManager::removeDevice(device dev)
{
    //comm_device_interface *devComm = commDeviceStore.take(dev.DeviceDesc+dev.DeviceUniqueID);
    //devComm->deleteLater();
    //qDebug()<< "deviceManager::removeDevice count" << count();
    //qDebug()<< "deviceManager::removeDevice start" << dev.DeviceDesc + " / " + dev.DeviceUniqueID;
    int index = availCommDev.findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
    //qDebug()<< "deviceManager::removeDevice start" << index;
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(availCommDev.itemData(index));
    if( devComm != nullptr)
        devComm->deleteLater();

    if( index < availCommDev.count())
        availCommDev.removeItem(index);
    else
        qDebug() << "Error deleting item";

    //emit deviceSelected(nullptr);  // не нужен потому что будет селект с нулем
}

void deviceManager::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected start";
    //qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(availCommDev.itemData(index));
    qDebug()<< "deviceManager::_deviceSelected finish" << devComm;
    //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!
    //if( devComm != nullptr)
    emit deviceSelected(devComm);
}

void deviceManager::baudRateUpdate()   // Обновляем скорость обмена
{
    baudRate = el_baudRate.text().toUInt();
    emit baudRateChanged(el_baudRate.text().toUInt());
    qDebug() << "=========== deviceManager::baudRateUpdate ================" << baudRate;
}
