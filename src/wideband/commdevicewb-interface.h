#ifndef COMMDEVICEWB_INTERFACE_H
#define COMMDEVICEWB_INTERFACE_H

class commDeviceWB
{

public:
    explicit commDeviceWB();

    virtual bool openWB() = 0;

    virtual bool connectWB(unsigned int baudRate) = 0;
    virtual bool closeWB() = 0;
    virtual bool readWB() = 0;

};

#endif // COMMDEVICEWB_INTERFACE_H
