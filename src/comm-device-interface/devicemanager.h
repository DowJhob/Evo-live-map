#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QComboBox>
#include <QObject>

#include "../enumdev.h"
#include "comm-device-interface.h"
#include "j2534-interface.h"
#include "op20.h"
#include "op13.h"

Q_DECLARE_METATYPE( comm_device_interface* )

class deviceManager : public QComboBox
{
    Q_OBJECT
public:
    deviceManager(QWidget *parent):QComboBox(parent)
    {
        connect(this,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &deviceManager::_deviceSelected);
    }
    deviceManager()
    {
        connect(this,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &deviceManager::_deviceSelected);
    }
public slots:
    void deviceEvent(device dev)
    {
        switch (dev.direction)
        {
        case dir::arrive : addDevice(dev); break;
        case dir::remove : removeDevice(dev); break;
        }
    }
private:
    void addDevice(device dev)
    {
        //qDebug()<< "deviceManager::addDevice start" << dev.DeviceDesc;
        comm_device_interface *devComm = nullptr;                                  // это важно если бы мы пытались добавить не инициализированную, тогда бы при попытке извлечь девайсСелектед она не прошла проверку кУвариант
        switch (dev.type)
        {
        case deviceType::OP13  : devComm = new OP13(dev.FunctionLibrary, dev.DeviceUniqueID); break;
        case deviceType::OP20  : devComm = new OP20(dev.FunctionLibrary, dev.DeviceUniqueID); break;
        case deviceType::J2534 : devComm = new j2534_interface(dev.FunctionLibrary, dev.DeviceUniqueID); break;
        default                : return;                                            //  но поскольку тут вылетим без добавления то вроде и не важно
        }
        //commDeviceStore.insert(dev.DeviceDesc+dev.DeviceUniqueID, devComm);

        addItem(dev.DeviceDesc + " / " + dev.DeviceUniqueID, QVariant::fromValue<comm_device_interface*>(devComm));
    }
    void removeDevice(device dev)
    {
        //comm_device_interface *devComm = commDeviceStore.take(dev.DeviceDesc+dev.DeviceUniqueID);
        //devComm->deleteLater();
        //qDebug()<< "deviceManager::removeDevice count" << count();
        //qDebug()<< "deviceManager::removeDevice start" << dev.DeviceDesc + " / " + dev.DeviceUniqueID;
        int index = findText(dev.DeviceDesc + " / " + dev.DeviceUniqueID);
        //qDebug()<< "deviceManager::removeDevice start" << index;
        comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(itemData(index));
        if( devComm != nullptr)
            devComm->deleteLater();

        if( index < count())
            removeItem(index);
        else
            qDebug() << "Error deleting item";

        //emit deviceSelected(nullptr);
    }

    //QHash<QString, comm_device_interface*> commDeviceStore;

private slots:
    void _deviceSelected(int index)
    {
        //qDebug()<< "deviceManager::_deviceSelected start";
        qDebug()<< "deviceManager::_deviceSelected start" << itemData(index);
        comm_device_interface *devComm = qvariant_cast<comm_device_interface*>(itemData(index));
        //qDebug()<< "deviceManager::_deviceSelected finish" << devComm;
        //qDebug()<< "deviceManager::_deviceSelected finish" << devComm->DeviceUniqueID; // Не делай так!!! devComm может быть нулл!!!
        if( devComm != nullptr)
            emit deviceSelected(devComm);
    }

signals:
    void deviceSelected(comm_device_interface*);

};

#endif // DEVICEMANAGER_H
