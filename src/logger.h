#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>

#include "ecu/ecu-definition.h"
#include "abstract-memory.h"

#include "DMA-proto/DMA-proto.h"

class dataLogger:public QObject
{
    Q_OBJECT
public:
    ecu_definition **_ecu_definition;
    QMap<int, float> log_param;
    QVector<float> scaledRAM_MUTvalue;
    DMA_proto **ECUproto;

    dataLogger(ecu_definition **_ecu = nullptr, comm_device_interface **devComm = nullptr, DMA_proto **ECUproto = nullptr);
    ~dataLogger();

    void setECU(ecu_definition **_ecu);

private:
    comm_device_interface **devComm = nullptr;

    QTimer pollTimer;
    int pollInt = 20;
    int readSize = 0;
    QDate startDate;
    QString path = QCoreApplication::applicationDirPath();
    QFile log_file;
    QTextCodec *codec = QTextCodec::codecForName("CP866");

    void newFile();

public slots:
    void start();
    void stop();
    void setLogRate(uint pollInt);

    void logger( QString str );
    void writeLogItem(QVector<float> scaledRAM_MUTvalue);

private slots:
    void poll();
    void read(abstractMemoryScaled a);

signals:
    void logReady(QVector<float>);
    void loop();

};

#endif // LOGGER_H
