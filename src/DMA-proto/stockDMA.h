#ifndef STOCK_DMA_H
#define STOCK_DMA_H

#include <QObject>
#include <QDebug>

#include "DMA-proto.h"

#define DS 0x33

class stockDMA : public DMA_proto
{
    Q_OBJECT
public:
    explicit stockDMA(comm_device_interface **devComm);
    ~stockDMA();

public slots:
    bool connect(uint baudRate);
    QByteArray indirectDMAread(quint32 addr, int lenght);
    QByteArray directDMAread(quint32 addr, int lenght);
    void directDMAwrite(quint32 addr, char *buf, int lenght);

private slots:

private:
    void setHeader(DMAcomand command, uchar count, quint32 addr);

    void getChckSmm();

//    void log0x81()
//    {
//        QByteArray a;
//        uchar packetSize = 0x33;
//        uchar logPacketBodySize = 0x30;
//        //uchar recPacketCount = (lenght + packetBodySize - 1) / packetBodySize; //сколько 44байтных пакетов нужно принять
//        //getRead( recPacketCount, addr);
//        // QByteArray bb = read();
//        //        int lastPacketBodySize = lenght - packetBodySize*(recPacketCount-1);
//        //qDebug() << "logDMAread read for safe" <<bb.toHex(':');

//        p_out_buff[0] = 0x81;
//        //    p_out_buff[1] = 0x17;
//        //    p_out_buff[2] = 0x00;  //sub type?
//        for (int i = 1; i < 0x31; i++)
//        {
//            p_out_buff[i] = 0x17;
//            p_out_buff[i+1] = 0x00;
//        }
//        // checksum
//        getChckSmm();
//        (*devComm)->write( 0x33 );
//    }

//    void stoplog0x81()
//    {
//        QByteArray a;
//        uchar packetSize = 0x33;
//        uchar logPacketBodySize = 0x30;
//        //uchar recPacketCount = (lenght + packetBodySize - 1) / packetBodySize; //сколько 44байтных пакетов нужно принять
//        //getRead( recPacketCount, addr);
//        //

//        p_out_buff[0] = 0x81;
//        //    p_out_buff[1] = 0x17;
//        //    p_out_buff[2] = 0x00;  //sub type?
//        for (int i = 1; i < 0x31; i++)
//        {
//            p_out_buff[i] = 0x00;
//            p_out_buff[i+1] = 0x00;
//        }
//        // checksum
//        getChckSmm();
//        (*devComm)->write( 0x33 );
//        QByteArray bb = (*devComm)->read();
//        qDebug() << "logDMAread read for safe" <<bb.toHex(':');
//    }

signals:
    //void readyInterface(bool);
    //void readyRead(QByteArray);

    //void ECUready();
    //void AFR(float);
    //void Log(QString);
};

#endif // STOCK_DMA_H
