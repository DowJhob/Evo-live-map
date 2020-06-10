#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>

#include "ecu.h"

class Logger:public QObject
{
    Q_OBJECT
public:
    ecu *_ecu;
    QMap<int, float> log_param;
    Logger(ecu *_ecu = nullptr):_ecu(_ecu)
    {
        //QString filename = startDate.toString("'log_'dd_MM_yyyy'.txt'");

    }
    ~Logger()
    {
    log_file.close();
    }
    void setECU(ecu *_ecu)
    {
        this->_ecu = _ecu;
        log_param.clear();
        newFile();
    }
private:
    QDate startDate;
    QString path = QCoreApplication::applicationDirPath();
    QFile log_file;
    QTextCodec *codec = QTextCodec::codecForName("CP866");

    void newFile(  )
    {
        log_file.close();
        startDate = QDate::currentDate();
        log_file.setFileName( path + "/" + startDate.toString("'log_'dd_MM_yyyy'.txt'") );
        if ( !log_file.open(QIODevice::Append) )
            qDebug() <<  "Лог файл не открыт!\n";


        QString log_str{};
        foreach (mutParam param, _ecu->RAM_MUT)
        {

            log_str.append(param.ram_mut_param_scaling.name + ";");
        }
        log_str.append("\n");
        log_file.write(log_str.toUtf8() + "\n");
        log_file.flush();
    }

public slots:
    void logger( QString str )
    {
        if ( startDate != QDate::currentDate() )
            newFile();
        str = QTime::currentTime().toString() + " | " + str;
        log_file.write(str.toUtf8() + "\n");
        //qDebug().noquote() << QString::fromLocal8Bit( codec->fromUnicode( str ) );
        log_file.flush();
    }
    void poll_data_calc(QByteArray in)
    {
        QString log_str{};
        foreach (mutParam param, _ecu->RAM_MUT)
        {
            qint64 value = _ecu->mut_cast( (uchar*)in.data(), &param.ram_mut_param_scaling, param.ram_mut_offset);
            log_param.insert( param.ram_mut_offset, value );
            log_str.append(QString::number(value) + ";");
        }
        log_str.append("\n");
        log_file.write(log_str.toUtf8());
        log_file.flush();
        emit log_data_ready();
    }
signals:
    void log_data_ready();
};

#endif // LOGGER_H
