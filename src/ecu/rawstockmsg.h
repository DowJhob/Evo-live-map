#ifndef RAWSTOCKMSG_H
#define RAWSTOCKMSG_H

#include <QtGlobal>
#include <QByteArray>

enum class eLogMsgType : char
{
    LOG_24x2B //= 0x81
    ,
    LOG_24x1B,
    LOG_5x2B,
    LOG_24x2B_LT,
    LOG_2x2B_6x1B_LT,
    LOG_5x2B_LT,
    LOG_COPY,
    LOG_INV,
    LOG_LT
};

struct rawMsg_24x1B
{
    quint8 command;
    quint8 msg[48];
    quint8 checkSum;
    quint8 EOM = 0x0D;
};

class stockMsg:public QByteArray
{
public:
    stockMsg():QByteArray()
    {

    }
    void fromRaw(QByteArray rawMsg)
    {
        if(rawMsg.size() < 4) // header + 1 byte data (without data this ..) +checksum + EOM(0x0D)
            return;
        char *data = rawMsg.data();
        switch((eLogMsgType)data[0])
        {
        case eLogMsgType::LOG_24x2B: fromRaw(rawMsg); break;
        default: break;
        }
    }
};

#endif // RAWSTOCKMSG_H
