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

#include "ecu-proto/ECU-interface.h"
#include "abstract-memory.h"
#include "log-reader.h"

class dataLogger:public QObject
{
    Q_OBJECT
public:
    ecu_definition **_ecu_definition;
    QMap<int, float> log_param;
    QVector<float> scaledRAM_MUTvalue;
    ECU_interface **ECUproto;

    dataLogger(ecu_definition **_ecu = nullptr, comm_device_interface **devComm = nullptr, ECU_interface **ECUproto = nullptr);
    ~dataLogger();

    void setECU(ecu_definition **_ecu);

private:
    comm_device_interface **devComm = nullptr;
    j2534listener *listener;

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
    void setLogRate(int pollInt);

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
