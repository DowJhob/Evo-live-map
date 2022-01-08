#ifndef OP20_H
#define OP20_H

//#include <QObject>
//#include <QDebug>

//#include "src/libs/J2534.h"
#include "j2534-interface.h"
#include "src/serial/tactrix-serial.h"

class OP20 : public j2534_interface
{
    Q_OBJECT
public:
    //common_serial_interface *tactrix_serial = nullptr;
    tactrix_serial *_tactrix_serial = nullptr;
    OP20(QString dllName = nullptr, QString DeviceUniqueID = "") : j2534_interface(dllName, DeviceUniqueID)
    {
        devType = deviceType::OP20;
        qDebug() << "OP20";
    }
    ~OP20()
    {
        if(_tactrix_serial != nullptr)
     //       _tactrix_serial->deleteLater();
        qDebug() << "~OP20";
    }

public slots:
    void _start()
    {
   //     j2534_interface::open();

   //     if (j2534->valid())
        {
     //       _tactrix_serial = new tactrix_serial( j2534, &devID);
      //      _tactrix_serial->start();
        }
    }


private slots:

private:

signals:
};


#endif // OP20_H
