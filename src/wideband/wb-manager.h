#ifndef WB_MANAGER_H
#define WB_MANAGER_H

#include <QComboBox>
#include <QObject>

//#include "../deviceNativeFilter.h"
//#include "../comm-device-interface/comm-device-interface.h"
#include "commdevicewb-interface.h"
#include "wblogger.h"

Q_DECLARE_METATYPE( commDeviceWB* )

class wbManager : public QComboBox
{
    Q_OBJECT
public:
    wbManager(QWidget *parent = nullptr);

public slots:
    void tactrixEvent(commDeviceWB *cdWB);
    void deviceEvent();

private:
    void addDevice();
    void removeDevice();

private slots:
    void _deviceSelected(int index);

signals:
    void deviceSelected(wbLogger*);

};

#endif // WB_MANAGER_H
