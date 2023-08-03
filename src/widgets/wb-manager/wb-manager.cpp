#include "wb-manager.h"
#include "ui_wb-manager.h"

wbManagerWidget::wbManagerWidget(QWidget *parent):QGroupBox(parent), ui(new Ui::wbManagerWidget)
{
    ui->setupUi(this);
    connect(ui->availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManagerWidget::_wbSelected);
    connect(ui->protoWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManagerWidget::_protoSelected);
    connect(ui->startBtn, &QPushButton::clicked, this, &wbManagerWidget::startStop);
}

void wbManagerWidget::fillProto()
{
    ui->protoWB->addItem("Innovate", QVariant::fromValue<wbProto*>(new innoProto()));
    ui->protoWB->addItem("AEM", QVariant::fromValue<wbProto*>(new aemProto()));
    ui->protoWB->addItem("PLX", QVariant::fromValue<wbProto*>(new plxProto()));
}

void wbManagerWidget::_wbSelected(int index)
{
    qDebug()<< "wbManager::_wbSelected" ;
    //    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->itemData(index));
    //    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    //    if(cdWB == nullptr)
    //    {
    //        ui->startBtn->setDisabled(true);
    //        return;
    //    }


    //emit wbSelected(cdWB);

    //    disconnect(wbToProto);
    //    disconnect(ProtoToLog);
    //    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    //    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManagerWidget::logReady);

    //qDebug() << "=========== wbLogger::readyRead ================";
    //    qDebug()<< "deviceManager::_deviceSelected";
    commDeviceWB *wbdevComm = qvariant_cast<commDeviceWB*>(ui->availWB->itemData(index));
    emit wbSelected(wbdevComm);
    if(wbdevComm == nullptr)
        ui->startBtn->setDisabled(true);
    else
    {
        ui->startBtn->setDisabled(false);
        wbdevComm->moveToThread(wb_thread);
    }
}

void wbManagerWidget::_protoSelected(int index)
{
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->itemData(index));

    //    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    qDebug()<< "wbManagerWidget::_protoSelected";
    //    if(cdWB == nullptr)
    //        return;
    //    disconnect(wbToProto);
    //    disconnect(ProtoToLog);
    //    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    //    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManagerWidget::logReady);


    //    wbProto *proto = qvariant_cast<wbProto*>(ui->protoWB->itemData(index));
    //    if(_protoWB ==nullptr)
    //        return;

    emit protoSelected(_protoWB);
}

void wbManagerWidget::addTactrix(commDeviceWB *cdWB)
{
    qDebug()<< "wbManager::addTactrix";
    ui->availWB->addItem(cdWB->DeviceDesc, QVariant::fromValue<commDeviceWB*>(cdWB));
    connect(cdWB, &commDeviceWB::destroyed, this, [this](QObject *o){
        qDebug()<< "commDeviceWB::destroyed::Tactrix";
        int index = ui->availWB->findData(QVariant::fromValue<commDeviceWB*>(static_cast<commDeviceWB*>(o)));
        ui->availWB->removeItem(index);
    });
}

void wbManagerWidget::removeTactrix(comm_device_interface *cdWB)
{
    qDebug()<< "wbManager::removeTactrix";

}

void wbManagerWidget::removeDevice()
{

}

void wbManagerWidget::deviceEvent()
{
    // dir::arrive
    //    switch (dev.direction)
    //    {
    //    case  : addDevice(dev); break;
    //    case dir::remove : removeDevice(dev); break;
    //    }
}

void wbManagerWidget::fillSerial()
{
    for( const auto &portInfo : QSerialPortInfo::availablePorts())
    {
        ui->availWB->addItem(portInfo.description() + " / " + portInfo.serialNumber(), QVariant::fromValue<commDeviceWB*>(new serialWB(portInfo, this)));
    }
}

void wbManagerWidget::addDevice()
{

}

void wbManagerWidget::startStop()
{
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(ui->availWB->currentData());
    wbProto *_protoWB = qvariant_cast<wbProto*>(ui->protoWB->currentData());
    qDebug() << "=========== wbManager::startBtn clicked ================" << cdWB << _protoWB;

    if(ui->startBtn->text() == "Start")
    {
        ui->availWB->setDisabled(true);
        ui->startBtn->setText("Stop");
        emit wbStart(true);
    }
    else
    {
        ui->availWB->setDisabled(false);
        ui->startBtn->setText("Start");
        emit wbStart(false);
    }
}
