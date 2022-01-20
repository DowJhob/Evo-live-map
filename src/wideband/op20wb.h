#ifndef OP20WB_H
#define OP20WB_H

#include <QObject>
#include "commdevicewb-interface.h"
#include "../comm-device-interface/op20.h"

class op20wb : public commDeviceWB
{
    Q_OBJECT
public:
    explicit op20wb(OP20 *j2534);
    virtual ~op20wb();

    bool isClosed();

    bool openWB(uint baudRate);

    bool connectWB(uint baudRate);

    bool closeWB();

    QByteArray readWB();

private slots:
    void poll();

private:
    OP20 *op20;
    QTimer *pollTimer;

    void pollHelper();

    Message rxmsg;
    Message msgMask, msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg = 1;

signals:

};

#endif // OP20WB_H
