#ifndef POLLHELPER_H
#define POLLHELPER_H

#include <QObject>
#include <QThread>
#include <QVector>

#include <QTimer>

//#include "comm-device-interface.h"

class pollHelper : public QObject
{
    Q_OBJECT
public:
    explicit pollHelper();

    QVector<float> scaledRAM_MUTvalue;
    int readSize = 0;
    QTimer *pollTimer;

public slots:
    virtual void startLog() = 0;
    virtual void stopLog() = 0;

    virtual void poll() = 0;

private:

private slots:

signals:
    void logReady(QVector<float>);

};

#endif // POLLHELPER_H
