#include "ECUcommDevicesManager.h"
#include "ui_ECUcommDevicesManager.h"

ECUcommDeviceManagerWidget::ECUcommDeviceManagerWidget(QWidget *parent):QGroupBox(parent), ui(new Ui::commDeviceManagerWidget)
{
    ui->setupUi(this);

    connect(ui->availCommDev, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ECUcommDeviceManagerWidget::_deviceSelected);
    connect(ui->el_baudRate,  &QLineEdit::editingFinished, this, &ECUcommDeviceManagerWidget::_baudRateChanged);
}

ECUcommDeviceManagerWidget::~ECUcommDeviceManagerWidget()
{
    delete ui;
}

void ECUcommDeviceManagerWidget::deviceEvent(device dev)
{
    switch (dev.direction)
    {
    case dir::arrive :
        // devComm->setBaudRate(ui->el_baudRate->text().toUInt());

        ui->availCommDev->addItem(dev.PortName /*+ " / " + dev.DeviceUniqueID*/, QVariant::fromValue<device>(dev));

        break;
    case dir::remove : removeDevice(dev); break;
    }
}

void ECUcommDeviceManagerWidget::addDevice(device dev)
{
    // qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;

    // это важно если бы мы пытались добавить не инициализированную,
    // тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
    comm_device_interface *devComm = nullptr;
    switch (dev.type)
    {
    case deviceType::SERIAL : devComm = new serial_comm(nullptr, dev.PortName); break;
    case deviceType::FTDI   : devComm = new FTDI_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::J2534  : devComm = new j2534_comm(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID); break;
    case deviceType::OP20   : devComm = new OP20(nullptr, dev.FunctionLibrary, dev.DeviceDesc, dev.DeviceUniqueID);
        ((OP20*)devComm)->tactrixWBinstance = new op20wb(static_cast<OP20*>(devComm));
        emit tactrixArrived(((OP20*)devComm)->tactrixWBinstance);
        break;

    default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
    }

    devComm->setBaudRate(ui->el_baudRate->text().toUInt());

    ui->availCommDev->addItem(dev.PortName /*+ " / " + dev.DeviceUniqueID*/, QVariant::fromValue<comm_device_interface*>(devComm));
    // setDisabled(false);
}

void ECUcommDeviceManagerWidget::removeDevice(device dev)
{
    int index = ui->availCommDev->findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);

    if( index < ui->availCommDev->count())
        ui->availCommDev->removeItem(index);
    else
        qDebug() << "Error deleting item";
    if( index < 0 )
        ;
}

void ECUcommDeviceManagerWidget::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected";

    auto dev = qvariant_cast<device>(ui->availCommDev->itemData(index));

    dev.baudRate = ui->el_baudRate->text().toUInt();

    emit deviceSelected(dev);
}

void ECUcommDeviceManagerWidget::_baudRateChanged()   // Обновляем скорость обмена
{
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->currentData());
    baudRate = ui->el_baudRate->text().toUInt();
    devComm->setBaudRate(baudRate);
    qDebug() << "=========== deviceManager::_baudRateChanged ================" << baudRate;
}
