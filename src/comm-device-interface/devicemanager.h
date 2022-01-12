#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QComboBox>
#include <QObject>

#include "../deviceNativeFilter.h"
#include "comm-device-interface.h"
#include "j2534-interface.h"
#include "op20.h"
#include "op13.h"

Q_DECLARE_METATYPE( comm_device_interface* )

class deviceManager : public QComboBox
{
    Q_OBJECT
public:
    deviceManager(QWidget *parent = nullptr);

public slots:
    void deviceEvent(device dev);

private:
    void addDevice(device dev);
    void removeDevice(device dev);

private slots:
    void _deviceSelected(int index);

signals:
    void deviceSelected(comm_device_interface*);

};

#endif // DEVICEMANAGER_H
