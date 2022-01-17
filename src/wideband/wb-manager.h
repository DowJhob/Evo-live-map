#ifndef WB_MANAGER_H
#define WB_MANAGER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSerialPort>

//#include "../deviceNativeFilter.h"
//#include "../comm-device-interface/comm-device-interface.h"
#include "wb-proto.h"
#include "innoProto.h"
#include "aemProto.h"
#include "plxProto.h"

#include "../comm-device-interface/op20.h"
#include "commdevicewb-interface.h"
#include "serialwb.h"

Q_DECLARE_METATYPE( wbProto* )

class wbManager : public QGroupBox
{
    Q_OBJECT
public:
    wbManager(QWidget *parent = nullptr);

public slots:
    void addTactrix(comm_device_interface *cdWB);
    void deviceEvent();

private:
    void addProto();
    QGridLayout layout;
    QComboBox availWB;
    QComboBox protoWB;
    QPushButton startBtn{"Start", this};
    //QLabel lgrt{"Logging rate, Hz:"};
    //QLineEdit el_lograte{"20"};

    void getAllSerial();
    void addDevice();
    void removeDevice();

private slots:
    void _wbSelected(int index);
    void _protoSelected(int index);

signals:
    void wbSelected(commDeviceWB*);
    void protoSelected(wbProto*);
    void wbStart(bool);

};

#endif // WB_MANAGER_H
