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

void wbManager::_wbSelected(int index)
{
    qDebug()<< "wbManager::_wbSelected" ;
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->itemData(index));
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    if(cdWB == nullptr)
    {
        ui->startBtn->setDisabled(true);
        return;
    }

    ui->startBtn->setDisabled(false);

    //emit wbSelected(cdWB);

    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);

    //qDebug() << "=========== wbLogger::readyRead ================";
    qDebug()<< "deviceManager::_deviceSelected";
    commDeviceWB *devComm = qvariant_cast<commDeviceWB*>(ui->availWB->itemData(index));
    if(devComm ==nullptr)
        return;

    emit wbSelected(devComm);
}

void wbManager::_protoSelected(int index)
{
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->itemData(index));

    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    if(cdWB == nullptr)
        return;
    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);


    qDebug()<< "deviceManager::_deviceSelected";
    wbProto *proto = qvariant_cast<wbProto*>(ui->protoWB->itemData(index));
    if(proto ==nullptr)
        return;

    emit protoSelected(proto);
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

}

void wbManager::removeDevice()
{

}

void wbManager::startStop()
{
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    qDebug() << "=========== wbManager::startBtn clicked ================" << cdWB << _protoWB;

    if(ui->startBtn->text() == "Start")
    {
        ui->availWB->setDisabled(true);
        {
            emit wbStart(true);
            ui->startBtn->setText("Stop");
        }
    }
    else
    {
        ui->availWB->setDisabled(false);
        {
            emit wbStart(false);
            ui->startBtn->setText("Start");
        }
    }
}
