#include "devicemanager.h"

commDeviceManager::commDeviceManager(QWidget *parent):QGroupBox(parent)
{
    setTitle("Communication devices");
    setLayout(&layout);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    /**/
    availCommDev.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    bd.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    el_baudRate.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout.addWidget(&availCommDev, 0, 0);
    layout.addWidget(&bd, 0, 1);
    layout.addWidget(&el_baudRate, 0, 2);

    connect(&availCommDev,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commDeviceManager::_deviceSelected);
    connect(&el_baudRate,  &QLineEdit::editingFinished, this, &commDeviceManager::_baudRateChanged);
}

void commDeviceManager::deviceEvent(device dev)
{
    switch (dev.direction)
    {
    case dir::arrive : addDevice(dev); break;
    case dir::remove : removeDevice(dev); break;
    }
}

void commDeviceManager::addDevice(device dev)
{
    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;
    comm_device_interface *devComm = nullptr;  // это важно если бы мы пытались добавить не инициализированную, тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
    switch (dev.type)
    {
    case deviceType::SERIAL  : devComm = new serial_comm(this, dev.PortName); break;
    //case deviceType::SERIAL  : devComm = new FTDI_comm(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;        // Костыль пока нету  серийного интерфейса
    case deviceType::FTDI    : devComm = new FTDI_comm(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::OP20    : devComm = new OP20(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID);
        emit tactrixArrived(new op20wb(static_cast<OP20*>(devComm)));
        break;

    case deviceType::J2534 : devComm = new j2534_comm(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }

    devComm->setBaudRate(el_baudRate.text().toUInt());

    availCommDev.addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
}

void commDeviceManager::removeDevice(device dev)
{
    int index = availCommDev.findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(availCommDev.itemData(index));
    if( devComm != nullptr)
    {
        if(dev.type == deviceType::OP20)
            emit tactrixRemoved(devComm);
        delete devComm;
    }
        //devComm->deleteLater();

    if( index < availCommDev.count())
        availCommDev.removeItem(index);
    else
        qDebug() << "Error deleting item";

}

void commDeviceManager::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected";
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(availCommDev.itemData(index));
    if(devComm ==nullptr)
        return;
    baudRate = el_baudRate.text().toUInt();
    devComm->setBaudRate(baudRate);             // вдруг она изменилась с момента генерациии devComm
    emit deviceSelected(devComm);
}

void commDeviceManager::_baudRateChanged()   // Обновляем скорость обмена
{
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(availCommDev.currentData());
    baudRate = el_baudRate.text().toUInt();
    devComm->setBaudRate(baudRate);
    qDebug() << "=========== deviceManager::_baudRateChanged ================" << baudRate;
}
