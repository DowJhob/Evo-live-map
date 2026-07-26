#include "devicemanager.h"
#include "ui_devicemanager.h"

commDeviceManagerWidget::commDeviceManagerWidget(QWidget *parent):QGroupBox(parent), ui(new Ui::commDeviceManagerWidget)
{
    ui->setupUi(this);

    connect(ui->availCommDev, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &commDeviceManagerWidget::_deviceSelected);
    connect(ui->el_baudRate,  &QLineEdit::editingFinished, this, &commDeviceManagerWidget::_baudRateChanged);
}

commDeviceManagerWidget::~commDeviceManagerWidget()
{
    delete ui;
}

void commDeviceManagerWidget::deviceEvent(device dev)
{
    switch (dev.direction)
    {
    case dir::arrive : addDevice(dev); break;
    case dir::remove : removeDevice(dev); break;
    }
}

void commDeviceManagerWidget::addDevice(device dev)
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
}

void commDeviceManagerWidget::removeDevice(device dev)
{
    int index = ui->availCommDev->findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);

    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->itemData(index));

    if( devComm != nullptr)
    {
        qDebug() << "commDeviceManagerWidget::removeDevice   deviceType::OP20" << (int)dev.type;
        if(dev.type == deviceType::OP20)
        {
            emit tactrixRemoved(((OP20*)devComm)->tactrixWBinstance);
        }

        if( devComm != selected_devComm)     // если это не выбранный можно его сразу удалить
        {
            qDebug() << "commDeviceManagerWidget::removeDevice   delete devComm";
            delete devComm;
        }
        else                                 // иначе пошлем сигнал эку что бы он остановился и удалил ссылки на этот объект
            emit deviceHasLeft(devComm);     // И пусть он там и удалится
    }

    if( index < ui->availCommDev->count())
        ui->availCommDev->removeItem(index);
    else
        qDebug() << "Error deleting item";
}

void commDeviceManagerWidget::_deviceSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected";
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->itemData(index));
    if(devComm == nullptr)
        return;

    if(selected_devComm != nullptr)
        selected_devComm->close();  // на всякий случай

    selected_devComm = devComm;

    baudRate = ui->el_baudRate->text().toUInt();
    devComm->setBaudRate(baudRate);

    emit deviceSelected(devComm);
}

void commDeviceManagerWidget::_baudRateChanged()   // Обновляем скорость обмена
{
    comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->currentData());
    baudRate = ui->el_baudRate->text().toUInt();
    devComm->setBaudRate(baudRate);
    qDebug() << "=========== deviceManager::_baudRateChanged ================" << baudRate;
}
