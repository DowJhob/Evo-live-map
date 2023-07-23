#include "wb-manager.h"
#include "ui_wb-manager.h"

wbManager::wbManager(QWidget *parent):QGroupBox(parent), ui(new Ui::wbManager)
{
    ui->setupUi(this);
    connect(ui->availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManager::_wbSelected);
    connect(ui->protoWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManager::_protoSelected);
    connect(ui->startBtn, &QPushButton::clicked, this, &wbManager::startStop);
}

void wbManager::fillProto()
{
    ui->protoWB->addItem("Innovate", QVariant::fromValue<wbProto*>(new innoProto()));
    ui->protoWB->addItem("AEM", QVariant::fromValue<wbProto*>(new aemProto()));
    ui->protoWB->addItem("PLX", QVariant::fromValue<wbProto*>(new plxProto()));
}

void wbManager::addTactrix(commDeviceWB *cdWB)
{
    qDebug()<< "wbManager::addTactrix";
    ui->availWB->addItem(cdWB->DeviceDesc, QVariant::fromValue<commDeviceWB*>(cdWB));
    connect(cdWB, &commDeviceWB::destroyed, this, [this](QObject *o){
        qDebug()<< "commDeviceWB::destroyed::Tactrix";
        int index = ui->availWB->findData(QVariant::fromValue<commDeviceWB*>(static_cast<commDeviceWB*>(o)));
        ui->availWB->removeItem(index);
    });
}

void wbManager::deviceEvent()
{
    // dir::arrive
    //    switch (dev.direction)
    //    {
    //    case  : addDevice(dev); break;
    //    case dir::remove : removeDevice(dev); break;
    //    }
}

void wbManager::fillSerial()
{
    for( const auto &portInfo : QSerialPortInfo::availablePorts())
    {
        ui->availWB->addItem(portInfo.description() + " / " + portInfo.serialNumber(), QVariant::fromValue<commDeviceWB*>(new serialWB(portInfo, this)));
    }
}

void wbManager::addDevice()
{
    //    //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;
    //    comm_device_interface *devComm = nullptr;                                  // это важно если бы мы пытались добавить не инициализированную, тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
    //    switch (dev.type)
    //    {
    //    case deviceType::OP13  : devComm = new OP13(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    //    case deviceType::OP20  : devComm = new OP20(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    //    case deviceType::J2534 : devComm = new j2534_interface(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    //    default                : return;                                           //  но поскольку тут вылетим без добавления то вроде и не важно
    //    }
    //    //commDeviceStore.insert(dev.DeviceDesc+dev.DeviceUniqueID, devComm);

    //    addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
}

void wbManager::removeDevice()
{
    //devComm->deleteLater();
    //qDebug()<< "deviceManager::removeDevice count" << count();
    //qDebug()<< "deviceManager::removeDevice start" << dev.DeviceDesc + " / " + dev.DeviceUniqueID;
    ////    int index = findText("");
    //qDebug()<< "deviceManager::removeDevice start" << index;
    ////    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(itemData(index));
    ////    if( cdWB != nullptr)
    {
        ////        delete cdWB;
        //cdWB->deleteLater();
    }

    ////    if( index < count())
    ////        removeItem(index);
    ////    else
    ///        qDebug() << "Error deleting item";

    //emit deviceSelected(nullptr);  // не нужен потому что будет селект с нулем
}

void wbManager::_wbSelected(int index)
{
    qDebug()<< "wbManager::_wbSelected" ;
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->itemData(index));
    if(cdWB == nullptr)
        return;
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    if(_protoWB == nullptr)
        return;
    //emit wbSelected(cdWB);

    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);

    //qDebug() << "=========== wbLogger::readyRead ================";

}

void wbManager::_protoSelected(int index)
{
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->itemData(index));
    if(_protoWB == nullptr)
        return;
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    if(cdWB == nullptr)
        return;
    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);


    //emit protoSelected(_protoWB);
}

void wbManager::startStop()
{
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    qDebug() << "=========== wbManager::startBtn clicked ================" << cdWB << _protoWB;

    if(ui->startBtn->text() == "Start")
    {
        if(cdWB->isClosed())
            if(cdWB->openWB(_protoWB->baudRate))
            {
                emit wbStart(true);
                ui->startBtn->setText("Stop");
            }
    }
    else
    {
        if(!cdWB->isClosed())
            if(cdWB->closeWB())
            {
                emit wbStart(false);
                ui->startBtn->setText("Start");
            }
    }
}
