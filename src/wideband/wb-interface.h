#ifndef WBINTERFACE_H
#define WBINTERFACE_H

#include "commdevicewb-interface.h"

class wbInterface //:public commDeviceWB
{
public:
    wbInterface(commDeviceWB **cdWB);

    virtual void handleWB() = 0;

private:
    commDeviceWB **cdWB;
};

#endif // WBINTERFACE_H
