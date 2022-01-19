#include "op20.h"

OP20::OP20(QObject *parent, QString dllName, QString DeviceDesc, QString DeviceUniqueID) : j2534_interface(parent, dllName, DeviceDesc, DeviceUniqueID)
{
    //devType = deviceType::OP20;
//    //qDebug() << "OP20" << DeviceUniqueID;
//    QThread *this_thread = new QThread();
//    QObject::connect(this_thread, &QThread::started, this, [this](){
//        pollTimer = new QTimer(this);
//        QObject::connect(pollTimer, &QTimer::timeout, this, [this](){
//            QByteArray a = readWB();
//            if (a.size() > 0)
//                emit readyRead(a);
//        });
//    }
//    );
//    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
//    QObject::connect(this, &OP20::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
//    QObject::connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
//    moveToThread(this_thread);
//    this_thread->start();
}

OP20::~OP20()
{
}

//bool OP20::isClosed()
//{
//    if (chanID_INNO == 0)
//        return true;
//    else
//        return false;
//}

//bool OP20::openWB(uint baudRate)
//{
//    //qDebug() << "==================== OP20:openWB ==================================" << j2534->lastErrorString();
//    if (isNotUse())
//        if (j2534->PassThruOpen(nullptr, &devID))         // Get devID
//        {
//            qDebug() << "==================== OP20:openWB::PassThruOpen ==================================" << j2534->lastErrorString();
//            return false;
//        }
//    setUse();

//    //qDebug() << "==================== OP20:openWB3 ==================================" << j2534->lastErrorString();

//    // try to connect to the specific channel we would like to use
//    //
//    // in this case, it is the 2.5mm jack on the Openport 2.0 which can be used as
//    // a RS-232 voltage level receive only input. the Innovate MTS bus
//    // can be used this way, as it is 19200 baud, N,8,1 serial and transmits continuously
//    // without any polling needed.
//    //
//    // note that the ISO9141_NO_CHECKSUM connection flag is used to avoid requiring the data
//    // to have valid ISO9141 checksums (it doesn't)
//    qDebug() << "==================== OP20:openWB::PassThruConnect before ==================================" << j2534->lastErrorString() << "devID" << devID << "chanID_INNO" << chanID_INNO;
//    if (chanID_INNO == 0)
//        if (j2534->PassThruConnect(devID, Protocol::ISO9141_INNO, ConnectFlag::ISO9141NoChecksum, baudRate, &chanID_INNO))
//        {
//            qDebug() << "==================== OP20:openWB::PassThruConnect after ==================================" << j2534->lastErrorString() << "devID" << devID << "chanID_INNO" << chanID_INNO;

//            return false;
//        }
//    qDebug() << "==================== OP20:openWB::PassThruConnect2 ==================================" << "devID" << devID << "chanID_INNO" << chanID_INNO;
//    // all J2534 channels need filters in order to receive anything at all
//    //
//    // in this case, we simply create a "pass all" filter so that we can see
//    // everything unfiltered in the raw stream

//    rxmsg.setProtocolId(Protocol::ISO9141_INNO);
//    rxmsg.m_rxStatus = 0;
//    rxmsg.m_txFlags = 0;
//    rxmsg.m_timestamp = 0;
//    rxmsg.m_dataSize = 1;
//    rxmsg.m_extraDataIndex = 0;
//    msgMask = msgPattern  = rxmsg;
//    msgMask.m_data[0] = 0; // mask the first byte to 0
//    msgPattern.m_data[0] = 0; // match it with 0 (i.e. pass everything)
//    if (j2534->PassThruStartMsgFilter(chanID_INNO, PassThru::PassFilter, &msgMask, &msgPattern, NULL, &msgId))
//    {
//        qDebug() << "==================== OP20:openWB::PassThruStartMsgFilter ==================================" << j2534->lastErrorString();
//        return false;
//    }

//    return true;
//}

//bool OP20::connectWB(uint baudRate)
//{
//    return true;
//}

//bool OP20::closeWB()
//{
//    qDebug() << "\n==================== OP20::closeWB ==================================" << "chanID_INNO" << chanID_INNO;
//    if (j2534->PassThruDisconnect(chanID_INNO))
//    {
//        qDebug() << "==================== OP20:closeWB::PassThruDisconnect ==================================" << j2534->lastErrorString();
//    }

//    resetUse();
//    if(countUSE <= 0)  // если последний то закрываем
//    {
//        countUSE = 0;   // на всякий случай
//        j2534->PassThruClose(devID);
//        chanID_INNO = 0;
//    }

//    chanID_INNO = 0;
//    return true;
//}

//QByteArray OP20::readWB()
//{
//    QByteArray a;
//    numRxMsg = 1;
//    do
//    {
//        j2534->PassThruReadMsgs(chanID_INNO, &rxmsg, &numRxMsg, 140);
//        qDebug() << "==================== OP20:readWB ==================================" << j2534->lastErrorString() << chanID_INNO;
//        a.append(QByteArray((char*)rx_msg.m_data, rx_msg.m_dataSize));
//        //        qDebug()<< "j2534_interface::read " << "rx_msg.m_rxStatus" << rx_msg.m_rxStatus
//        //                << "rx_msg.m_dataSize" << rx_msg.m_dataSize
//        //                << "time" << QString::number( tt.nsecsElapsed()/1000000.0)
//        //                << "NumMsgs=" << NumMsgs
//        //                   //<< "rx_msg[0].data" << QByteArray((char*)rx_msg[0].m_data, 4).toHex(':')
//        //                   ;
//    }
//    while(rx_msg.m_rxStatus == Message::RxStatusBit::InStartOfMessage);

//    //qDebug() << "OP20::read: readWB" << a << "\n\n";

//    //
//    return a;
//}
