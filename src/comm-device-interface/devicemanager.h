#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

#include "../deviceNativeFilter.h"
#include "comm-device-interface.h"
#include "j2534-interface.h"
#include "op20.h"
#include "op13.h"

Q_DECLARE_METATYPE( comm_device_interface* )

class deviceManager : public QGroupBox
{
    Q_OBJECT
public:
    uint baudRate;
    deviceManager(QWidget *parent = nullptr);

public slots:
    void deviceEvent(device dev);

private:
    QGridLayout layout;

    QComboBox availCommDev;
    QLabel bd{"Baud rate, Baud:"};
    QLineEdit el_baudRate{"15625"};

    void addDevice(device dev);
    void removeDevice(device dev);

private slots:
    void _deviceSelected(int index);
    void baudRateUpdate();

signals:
    void deviceSelected(comm_device_interface*);
    void baudRateChanged(uint);

    void tactrixArrived(comm_device_interface*);
        void tactrixRemoved(comm_device_interface*);

};

#endif // DEVICEMANAGER_H
