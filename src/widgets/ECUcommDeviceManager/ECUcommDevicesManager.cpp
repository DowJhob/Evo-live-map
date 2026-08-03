#include "ECUcommDevicesManager.h"
#include "ui_ECUcommDevicesManager.h"

ECUcommDeviceManagerWidget::ECUcommDeviceManagerWidget(QWidget *parent):QGroupBox(parent), ui(new Ui::commDeviceManagerWidget)
{
    ui->setupUi(this);

    connect(ui->availCommDev, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ECUcommDeviceManagerWidget::_deviceSelected);
    connect(ui->el_baudRate,  &QLineEdit::editingFinished, this, &ECUcommDeviceManagerWidget::_baudRateChanged);
    // connect(ui->el_baudRate,  &QLineEdit::editingFinished, this, &ECUcommDeviceManagerWidget::ECU_SetBaudRate);
}

ECUcommDeviceManagerWidget::~ECUcommDeviceManagerWidget()
{
    delete ui;
}

void ECUcommDeviceManagerWidget::insertECU_Device(device dev)
{
        ui->availCommDev->addItem(dev.PortName /*+ " / " + dev.DeviceUniqueID*/, QVariant::fromValue<device>(dev));
}

void ECUcommDeviceManagerWidget::removeECU_Device(device dev)
{
    qDebug() << Q_FUNC_INFO;
    // int index = ui->availCommDev->findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
    int index = ui->availCommDev->findText(dev.PortName);
    if( index < ui->availCommDev->count() && index > 0)
    {
        if(ui->availCommDev->currentIndex() == index)
            emit ECU_deviceHasLeft();
        ui->availCommDev->removeItem(index);
    }
    else
        qDebug() << Q_FUNC_INFO << "Error deleting item, item not in range";
}

void ECUcommDeviceManagerWidget::_deviceSelected(int index)
{
    qDebug()<< Q_FUNC_INFO;

    device dev = qvariant_cast<device>(ui->availCommDev->itemData(index));

    dev.baudRate = ui->el_baudRate->text().toUInt();

    emit deviceSelected(dev);
}

void ECUcommDeviceManagerWidget::_baudRateChanged()   // Обновляем скорость обмена
{
    // comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(ui->availCommDev->currentData());
    // baudRate = ui->el_baudRate->text().toUInt();
    // devComm->setBaudRate(baudRate);
    qDebug() << "=========== deviceManager::_baudRateChanged ================" << baudRate;
    emit ECU_SetBaudRate(ui->el_baudRate->text().toUInt());
}
