#ifndef WB_MANAGER_H
#define WB_MANAGER_H

#include <QObject>
#include <QMetaObject>
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
#include "../../wideband/wb-proto.h"
#include "../../wideband/innoProto.h"
#include "../../wideband/aemProto.h"
#include "../../wideband/plxProto.h"

#include "../../comm-device-interface/op20.h"
#include "../../wideband/commdevicewb-interface.h"
#include "../../wideband/serialwb.h"

Q_DECLARE_METATYPE( wbProto* )

namespace Ui {
class wbManagerWidget;
}

class wbManagerWidget : public QGroupBox
{
    Q_OBJECT
public:
    QThread *wb_thread;
    wbManagerWidget(QWidget *parent = nullptr);

public slots:
    void addTactrix(commDeviceWB *cdWB);
    void removeTactrix(comm_device_interface *cdWB);

    void deviceEvent();
    void fillSerial();
    void fillProto();

private:
    Ui::wbManagerWidget *ui;

    QMetaObject::Connection wbToProto;
    QMetaObject::Connection ProtoToLog;

    void addDevice();
    void removeDevice();

private slots:
    void _wbSelected(int index);
    void _protoSelected(int index);
    void startStop();

signals:
    void wbSelected(commDeviceWB*);
    void protoSelected(wbProto*);
    void wbStart(bool);

    void logReady(QString);

};

#endif // WB_MANAGER_H
