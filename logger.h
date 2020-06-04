#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
//#include <QMap>
//#include <mathparser2.h>
#include "ecu.h"

class Logger:public QObject
{
    Q_OBJECT
public:
    ecu *_ecu;
    QMap<int, float> log_param;
    Logger(ecu *_ecu = nullptr):_ecu(_ecu)
    {
    }
    void setECU(ecu *_ecu)
    {
        this->_ecu = _ecu;
        log_param.clear();
    }
public slots:
    void poll_data_calc(QByteArray in)
    {
        foreach (mutParam param, _ecu->RAM_MUT)
        {
            log_param.insert(
                        param.ram_mut_offset,
                        _ecu->mut_cast( (uchar*)in.data(), &param.ram_mut_param_scaling, param.ram_mut_offset));

        }
        emit log_data_ready();
    }
signals:
    void log_data_ready();
};

#endif // LOGGER_H
