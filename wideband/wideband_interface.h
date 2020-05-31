#ifndef WIDEBAND_INTERFACE_H
#define WIDEBAND_INTERFACE_H

#include <QObject>
#include <QDebug>

class wideband_interface:public QObject
{
    Q_OBJECT
public:
    bool flag = true;
    wideband_interface()
    {
        //this->polling_interval = polling_interval;
    }
    ~wideband_interface()
    {
        emit AFR("----");
    }
public slots:
    virtual void _dump(uchar* data, ulong DataSize) = 0;
signals:
    void AFR(QString);
};

#endif // WIDEBAND_INTERFACE_H
