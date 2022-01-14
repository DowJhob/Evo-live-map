#include "wb-manager.h"

wbManager::wbManager(QWidget *parent):QGroupBox(parent)
{
    setTitle("Available wideband");
    setLayout(&layout);

    availWB.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //lgrt.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //el_lograte.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout.addWidget(&availWB, 0, 0);
    //layout.addWidget(&lgrt, 0, 1);
    //layout.addWidget(&el_lograte, 0, 2);

    connect(&availWB,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &wbManager::wbSelected);
    //connect(&el_lograte,  &QLineEdit::editingFinished, this, &protoManager::_logRateChanged);


    //QGroupBox *grBxWBprt = new QGroupBox("Wideband proto", this);
    //ll->addWidget(grBxWBprt, 0, 1);





    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void wbManager::tactrixEvent(commDeviceWB *cdWB)
{
    wbLogger *wblog = new wbLogger(&cdWB);
    emit deviceSelected(wblog);

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

void wbManager::wbSelected(int index)
{
    qDebug()<< "deviceManager::_deviceSelected start";
    //qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
////    wbLogger *wblog = qvariant_cast<wbLogger*>(itemData(index));
////    qDebug()<< "deviceManager::_deviceSelected finish" << wblog;
    //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!

////    emit deviceSelected(wblog);
}
