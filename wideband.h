#ifndef WIDEBAND_H
#define WIDEBAND_H

#include <QObject>

#include "wideband/wideband_input_device.h"
#include "wideband/wideband_interface.h"

#include "wideband/inno_interface.h"
#include "wideband/plx_interface.h"

#include "wideband/tactrix_wideband.h"


class wideband:public QObject
{
    Q_OBJECT
public:
    wideband(int interface_type, int device_type)
    {
        switch (interface_type)
        {
        case 1 : wb_interface = new inno_interface;break;
        case 2 : wb_interface = new plx_interface;
        }

    }
    wideband_input_device *wb_device = nullptr;
    wideband_interface *wb_interface = nullptr;
};

#endif // WIDEBAND_H
