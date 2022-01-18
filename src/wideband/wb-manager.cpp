#include "wb-manager.h"

wbManager::wbManager(QWidget *parent):QGroupBox(parent)
{
    setTitle("Available wideband");
    setLayout(&layout);

    availWB.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //lgrt.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //el_lograte.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout.addWidget(&availWB, 0, 0);
    layout.addWidget(&protoWB, 0, 1);
    layout.addWidget(&startBtn, 0, 2);

    connect(&availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManager::_wbSelected);
    connect(&protoWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManager::_protoSelected);
    connect(&startBtn, &QPushButton::clicked, this, [this](){



        commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(availWB.currentData());
        wbProto *_protoWB = qvariant_cast<wbProto*>(protoWB.currentData());
        qDebug() << "=========== wbManager::startBtn clicked ================" << cdWB << _protoWB;

        if(startBtn.text() == "Start")
        {
            if(cdWB->isClosed())
                if(cdWB->openWB(_protoWB->baudRate))
                {
                    emit wbStart(true);
                    startBtn.setText("Stop");
                }
        }
        else
        {
            if(!cdWB->isClosed())
                if(cdWB->closeWB())
                {
                    emit wbStart(false);
                    startBtn.setText("Start");
                }
        }
    });

    //fillSerial();

    //fillProto();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void wbManager::fillProto()
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

    protoWB.addItem("Innovate", QVariant::fromValue<wbProto*>(new innoProto()));
    protoWB.addItem("AEM", QVariant::fromValue<wbProto*>(new aemProto()));
    protoWB.addItem("PLX", QVariant::fromValue<wbProto*>(new plxProto()));
}

void wbManager::addTactrix(comm_device_interface *cdWB)
{
    commDeviceWB *ss = static_cast<OP20*>(cdWB);

    //wbLogger *wblog = new wbLogger(&cdWB);
    //emit deviceSelected(wblog);
    availWB.addItem(cdWB->DeviceDesc + " / " + cdWB->DeviceUniqueID, QVariant::fromValue<commDeviceWB*>(ss));
    //    addItem("tactrix", QVariant::fromValue<wbLogger*>(wblog));
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
        availWB.addItem(portInfo.description() + " / " + portInfo.serialNumber(), QVariant::fromValue<commDeviceWB*>(new serialWB(portInfo, this)));
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
    //comm_device_interface *devComm = commDeviceStore.take(dev.DeviceDesc+dev.DeviceUniqueID);
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
    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(availWB.itemData(index));

    wbProto *_protoWB = qvariant_cast<wbProto*>(protoWB.currentData());
    //emit wbSelected(cdWB);

    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);

        //qDebug() << "=========== wbLogger::readyRead ================";

}

void wbManager::_protoSelected(int index)
{
    wbProto *_protoWB = qvariant_cast<wbProto*>(protoWB.itemData(index));


    commDeviceWB *cdWB = qvariant_cast<commDeviceWB*>(availWB.currentData());

    disconnect(wbToProto);
    disconnect(ProtoToLog);
    wbToProto = connect(cdWB, &commDeviceWB::readyRead, _protoWB, &wbProto::handleWB);
    ProtoToLog = connect(_protoWB, &wbProto::logReady, this, &wbManager::logReady);


    //emit protoSelected(_protoWB);
}
