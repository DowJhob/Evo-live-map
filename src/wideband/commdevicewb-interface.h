#ifndef COMMDEVICEWB_INTERFACE_H
#define COMMDEVICEWB_INTERFACE_H

#include <QObject>
#include <QString>

class commDeviceWB : public QObject
{    
    Q_OBJECT
public:
    QString DeviceDesc;// + " / " + cdWB->DeviceUniqueID
    explicit commDeviceWB();
    virtual ~commDeviceWB();

    virtual bool isClosed() = 0;
    virtual bool openWB(uint baudRate) = 0;

    virtual bool connectWB(unsigned int baudRate) = 0;
    virtual bool closeWB() = 0;
    virtual QByteArray readWB() = 0;

signals:
    void readyRead(QByteArray);

};

Q_DECLARE_METATYPE( commDeviceWB* )

#endif // COMMDEVICEWB_INTERFACE_H
