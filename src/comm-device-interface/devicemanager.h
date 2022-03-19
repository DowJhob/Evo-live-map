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
#include "j2534-comm.h"
#include "op20.h"
#include "ftdi-comm.h"
#include "serial-comm.h"

#include "../wideband/op20wb.h"

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
    void _baudRateChanged();

signals:
    void deviceSelected(comm_device_interface*);

    void tactrixArrived(commDeviceWB *);
    void tactrixRemoved(comm_device_interface*);

};

#endif // DEVICEMANAGER_H
