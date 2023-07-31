#include "op20.h"

OP20::OP20(QObject *parent, QString dllName, QString DeviceDesc, QString DeviceUniqueID) : j2534_comm(parent, dllName, DeviceDesc, DeviceUniqueID)
{
}

OP20::~OP20()
{
}

bool OP20::close()
{
    // shut down the channel
    if (j2534->PassThruDisconnect(chanID) == PassThru::Status::NoError )
    {
        chanID = 0;
        if(!WBinUse)
            if (j2534->PassThruClose(devID) == PassThru::Status::NoError)
            {
                inUse = false;
                devID = 0;
                qDebug() << "==================== OP20::close::PassThruClose closed succefull ==================================" << j2534->lastErrorString();
                return true;
            }
        qDebug() << "==================== OP20::close::PassThruClose only disconnect succefull ==================================" << j2534->lastErrorString();
        return true;
    }
    qDebug() << "==================== OP20::close::PassThruDisconnect ==================================" << j2534->lastErrorString();
    return false;
}

bool OP20::open()
{
    PassThru::Status status;
    qDebug() << "==================== j2534_interface::ISO9141 -  =========================== status" << status << devID;
    if(!WBinUse)
    {
        if ( status = j2534->PassThruOpen(nullptr, &devID); status != PassThru::Status::NoError && status != PassThru::Status::DeviceInUse )
        {
            return false;
        }
    }
    inUse = true;
    return true;
}
