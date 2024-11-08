#ifndef WB_OP20_H
#define WB_OP20_H

#include <QObject>
#include "WB_I_ComDev.h"
#include "../comm-device-interface/op20.h"

class op20wb : public commDeviceWB
{
    Q_OBJECT
public:
    explicit op20wb(OP20 *op20);
    virtual ~op20wb();

    bool openWB(uint baudRate);

    bool connectWB(uint baudRate);

    bool closeWB();

    QByteArray readWB();

    void startLog(int baudRate);

    void stopLog();

private slots:
    void poll();

private:
    OP20 *op20;
    //unsigned long devID = 0;
    QTimer *shootTimer = nullptr;
    unsigned long chanID_INNO = 0;

    Message rxmsg;
    Message msgMask, msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg = 1;

    int pollRate = 50;

signals:
    void _poll();

};

#endif // WB_OP20_H
