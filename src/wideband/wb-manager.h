#ifndef WB_MANAGER_H
#define WB_MANAGER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

//#include "../deviceNativeFilter.h"
//#include "../comm-device-interface/comm-device-interface.h"
#include "commdevicewb-interface.h"
#include "wblogger.h"

Q_DECLARE_METATYPE( commDeviceWB* )

class wbManager : public QGroupBox
{
    Q_OBJECT
public:
    wbManager(QWidget *parent = nullptr);

public slots:
    void tactrixEvent(commDeviceWB *cdWB);
    void deviceEvent();

private:
    QGridLayout layout;
    QComboBox availWB;
    //QLabel lgrt{"Logging rate, Hz:"};
    //QLineEdit el_lograte{"20"};

    void addDevice();
    void removeDevice();

private slots:
    void wbSelected(int index);

signals:
    void deviceSelected(wbLogger*);

};

#endif // WB_MANAGER_H
