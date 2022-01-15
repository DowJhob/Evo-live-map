#ifndef WB_MANAGER_H
#define WB_MANAGER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

//#include "../deviceNativeFilter.h"
//#include "../comm-device-interface/comm-device-interface.h"
#include "../comm-device-interface/op20.h"
#include "commdevicewb-interface.h"
//#include "wblogger.h"

Q_DECLARE_METATYPE( commDeviceWB* )

class wbManager : public QGroupBox
{
    Q_OBJECT
public:
    wbManager(QWidget *parent = nullptr);

public slots:
    void addTactrix(comm_device_interface *cdWB);
    void deviceEvent();

private:
    QGridLayout layout;
    QComboBox availWB;
    QComboBox protoWB;
    QPushButton startBtn{"Start"};
    //QLabel lgrt{"Logging rate, Hz:"};
    //QLineEdit el_lograte{"20"};

    void addDevice();
    void removeDevice();

private slots:
    void _wbSelected(int index);

signals:
    void wbSelected(commDeviceWB*);
    void protoSelected(int);
    void wbStart(bool);

};

#endif // WB_MANAGER_H
