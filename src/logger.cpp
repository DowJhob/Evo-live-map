#include "logger.h"
#include <QCursor>

dataLogger::dataLogger(ecu_definition **_ecu, comm_device_interface **devComm, ECU_interface **ECUproto):_ecu_definition(_ecu), ECUproto(ECUproto), devComm(devComm)
{
    //QString filename = startDate.toString("'log_'dd_MM_yyyy'.txt'");

    //qDebug() << "dataLogger::dataLogger" << ECUproto;
    //pollTimer = new QTimer();
    connect(&pollTimer, &QTimer::timeout, this, &dataLogger::poll);
    //connect(this, &dataLogger::loop, this, &dataLogger::read);

    //listener = new j2534listener(this, devComm, ECUproto);
    //connect(listener, &j2534listener::logReady, this, &dataLogger::read);
    //qDebug() << "dataLogger::dataLogger thread: " << thread();
}

dataLogger::~dataLogger()
{
    log_file.close();
}

void dataLogger::setECU(ecu_definition **_ecu)
{
    this->_ecu_definition = _ecu;
    scaledRAM_MUTvalue.fill(0, (*_ecu_definition)->RAM_MUT.size());
    log_param.clear();
    newFile();
}

void dataLogger::newFile()
{
    log_file.close();
    startDate = QDate::currentDate();
    log_file.setFileName( path + "/" + startDate.toString("'log_'dd_MM_yyyy'.txt'") );
    if ( !log_file.open(QIODevice::Append) )
        qDebug() <<  "Лог файл не открыт!\n";


    QString log_str{};
    //foreach (mutParam param, _ecu->RAM_MUT)
    for (mutParam param : (*_ecu_definition)->RAM_MUT)
    {

        log_str.append(param.scaling.name + ";");
    }
    log_str.append("\n");
    log_file.write(log_str.toUtf8() + "\n");
    log_file.flush();
}

void dataLogger::start()
{
    for( int i = 0; i < (*_ecu_definition)->RAM_MUT.size() ; ++i  )
    {
        (*_ecu_definition)->RAM_MUT[i].offset = readSize;
        readSize += (*_ecu_definition)->RAM_MUT[i].scaling.getElementSize();
        //qDebug() << "dataLogger::start" << (*_ecu_definition)->RAM_MUT[i].scaling.name << (*_ecu_definition)->RAM_MUT[i].scaling.getElementSize();
    }
    scaledRAM_MUTvalue.resize((*_ecu_definition)->RAM_MUT.size());

    //listener->start();
    pollTimer.start(pollInt);
}

void dataLogger::stop()
{
    //listener->quit();
    pollTimer.stop();
    scaledRAM_MUTvalue.clear();
}

void dataLogger::setLogRate(int pollInt)
{
    this->pollInt = pollInt;
}

void dataLogger::logger(QString str)
{
    if ( startDate != QDate::currentDate() )
        newFile();
    str = QTime::currentTime().toString() + " | " + str;
    log_file.write(str.toUtf8() + "\n");
    //qDebug().noquote() << QString::fromLocal8Bit( codec->fromUnicode( str ) );
    log_file.flush();
}

void dataLogger::writeLogItem(QVector<float> scaledRAM_MUTvalue)
{
    QString log_str{};
    for( int i = 0; i < (*_ecu_definition)->RAM_MUT.size() ; i++  )
    {
        //log_param.insert( param.offset, value );
        log_str.append(QString::number(scaledRAM_MUTvalue[i]) + ";");
    }
    log_str.append("\n");
    log_file.write(log_str.toUtf8());
    log_file.flush();
    //qDebug() << "scaledRAM_MUTvalue: "<<scaledRAM_MUTvalue;
    emit logReady(scaledRAM_MUTvalue);
}

extern QElapsedTimer t;
void dataLogger::poll()
{
    QElapsedTimer tt;
    tt.start();
    //unsigned long addr = (*_ecu_definition)->RAM_MUT_addr;
    //qDebug() << "dataLogger::poll poll: " ;
    abstractMemoryScaled a;
    if(ECUproto != nullptr)
    {
        a = (*ECUproto)->indirectDMAread((*_ecu_definition)->RAM_MUT_addr, readSize);
        //qDebug() << "dataLogger::poll read: " << QString::number( tt.nsecsElapsed()/1000000.0);
        //a[0] = abs(QCursor::pos().x())/10;
        //a[1] = abs(QCursor::pos().y())/6;

        for( int i = 0; i < (*_ecu_definition)->RAM_MUT.size() ; i++  )
        {
            scaledRAM_MUTvalue[i] = a.toFloatOffset( &(*_ecu_definition)->RAM_MUT[i].scaling, (*_ecu_definition)->RAM_MUT[i].offset );
        }

        emit logReady(scaledRAM_MUTvalue);
    }
}

void dataLogger::read(abstractMemoryScaled a)
{
    for( int i = 0; i < (*_ecu_definition)->RAM_MUT.size() ; i++  )
    {
        scaledRAM_MUTvalue[i] = a.toFloatOffset( &(*_ecu_definition)->RAM_MUT[i].scaling, (*_ecu_definition)->RAM_MUT[i].offset );
    }


    //qDebug() << "dataLogger::poll + scalad: " << QString::number( tt.nsecsElapsed()/1000000.0);
    //t.start();
    emit logReady(scaledRAM_MUTvalue);
}
