#include "devicemanager.h"
#include "ui_devicemanager.h"

commDeviceManager::commDeviceManager(QWidget *parent):QGroupBox(parent), ui(new Ui::commDeviceManager)
{
    ui->setupUi(this);

    connect(ui->availCommDev, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commDeviceManager::_deviceSelected);
    connect(ui->el_baudRate,  &QLineEdit::editingFinished, this, &commDeviceManager::_baudRateChanged);
}

commDeviceManager::~commDeviceManager()
{
    delete ui;
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

    // это важно если бы мы пытались добавить не инициализированную,
    // тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
    comm_device_interface *devComm = nullptr;
    switch (dev.type)
    {
    case deviceType::SERIAL  : devComm = new serial_comm(this, dev.PortName); break;
    case deviceType::FTDI    : devComm = new FTDI_comm(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::J2534 : devComm = new j2534_comm(this, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::OP20    : devComm = new OP20(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID);
        emit tactrixArrived(new op20wb(static_cast<OP20*>(devComm)));
        break;

    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }

    devComm->setBaudRate(ui->el_baudRate->text().toUInt());

    ui->availCommDev->addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
}

void commDeviceManager::removeDevice(device dev)
{
    int index = ui->availCommDev->findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->itemData(index));
    if( devComm != nullptr)
    {
        if(dev.type == deviceType::OP20)
            emit tactrixRemoved(devComm);
        //        delete devComm;
    }
    //devComm->deleteLater();

    if( index < ui->availCommDev->count())
        ui->availCommDev->removeItem(index);
    else
        qDebug() << "Error deleting item";

}

void commDeviceManager::_removeDevice(comm_device_interface *devComm)
{
    if( devComm != nullptr)
    {
        delete devComm;
    }
}

void commDeviceManager::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected";
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->itemData(index));
    emit deviceSelected(devComm);
    if(devComm == nullptr)
        return;
    baudRate = ui->el_baudRate->text().toUInt();
    devComm->setBaudRate(baudRate);
}

void commDeviceManager::_baudRateChanged()   // Обновляем скорость обмена
{
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->currentData());
    baudRate = ui->el_baudRate->text().toUInt();
    devComm->setBaudRate(baudRate);
    qDebug() << "=========== deviceManager::_baudRateChanged ================" << baudRate;
}
