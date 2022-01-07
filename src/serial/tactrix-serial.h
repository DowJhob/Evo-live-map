#ifndef TACTRIX_SERIAL_H
#define TACTRIX_SERIAL_H

#include <QObject>
#include <QThread>

//#include "src/libs/J2534.h"
#include "common-serial-interface.h"
#include "../libs/j2534passthru.h"

using namespace J2534;
class tactrix_serial: public common_serial_interface
{
    //Q_OBJECT
public:
    tactrix_serial(PassThru *j2534 = nullptr, unsigned long *devID = 0) : common_serial_interface(), j2534(j2534), devID(devID)
    {

    }

    ~tactrix_serial()
    {
        if (j2534->PassThruDisconnect(chanID_INNO))
        {
            //   reportJ2534Error();
        }
    }

private slots:
    void open()
    {
        //emit AFR("20.4");
        // try to connect to the specific channel we would like to use
        //
        // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
        // a RS-232 voltage level receive only input. the Innovate MTS bus
        // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
        // without any polling needed.
        //
        // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
        // to have valid ISO9141 checksums (it doesn't)
//        if (j2534->PassThruConnect(*devID, ISO9141_INNO, ISO9141_NO_CHECKSUM, 19200, &chanID_INNO))
        {
            qDebug() << "wb error PassThruConnect: " ;
            //         return 0;
        }

        // all J2534 channels need filters in order to receive anything at all
        //
        // in this case, we simply create a "pass all" filter so that we can see
        // everything unfiltered in the raw stream

//        rxmsg.ProtocolID = ISO9141_INNO;
//        rxmsg.RxStatus = 0;
//        rxmsg.TxFlags = 0;
//        rxmsg.Timestamp = 0;
//        rxmsg.DataSize = 1;
//        rxmsg.ExtraDataIndex = 0;
//        msgMask = msgPattern  = rxmsg;
//        msgMask.Data[0] = 0; // mask the first byte to 0
//        msgPattern.Data[0] = 0; // match it with 0 (i.e. pass everything)
//        if (j2534->PassThruStartMsgFilter(chanID_INNO, PASS_FILTER, &msgMask, &msgPattern, NULL, &msgId))
        {
            //qDebug() << "wb error PassThruStartMsgFilter: " << reportJ2534Error();
            //            return 0;
        }

    }
    QByteArray read()
    {
        //qDebug() << "hop: " ;
        numRxMsg = 1;
        j2534->PassThruReadMsgs(chanID_INNO, &rxmsg, &numRxMsg, 40);
       // qDebug() << "hop22: " ;
        //if (numRxMsg)
//            return QByteArray::fromRawData((char*)rxmsg.Data, rxmsg.DataSize);
        //return false;
    }

private:
    PassThru *j2534;

    unsigned long* devID;
    unsigned long chanID_INNO;

    Message rxmsg;
    Message msgMask, msgPattern;
    unsigned long msgId;
    unsigned long numRxMsg;
//    unsigned long protocol_inno = ISO9141_INNO;

//    bool tactrix_wb_dump_msg(PASSTHRU_MSG* msg)
//    {
//        if (msg->RxStatus & START_OF_MESSAGE)
//            return false; // skip

//        if (msg->DataSize < 2)
//            return false;
// //       (this->*dump)();
//        return true;
//    }

};


#endif // TACTRIX_SERIAL_H
