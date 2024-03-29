#include "controller.h"

controller::controller(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Map>("Map");
    qRegisterMetaType<mapDefinition>("mapDefinition");
    qRegisterMetaType<abstractMemoryScaled>("abstractMemoryScaled");
    qRegisterMetaType<QVector<float>>("QVector<float>");

    this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, &controller::init//, Qt::QueuedConnection
                     );
    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    QObject::connect(this, &controller::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    QObject::connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);

    //ecu_polling_timer = new QTimer();

    setProto(0);
}

controller::~controller()
{
//    getECUdisconnect();

//    if (ECUproto != nullptr)
//        ECUproto->deleteLater();


//    if (devComm != nullptr)
//        devComm->deleteLater();
    qDebug() << "~controller";
}

void controller::start()
{
    this_thread->start();  // запустим поток,
}

void controller::setCommDevice(comm_device_interface *dev)
{
    qDebug() << "=========== controller::setCommDevice ================" << dev;
    devComm = dev;
    this->ECUproto->setCommDev(&devComm);
    if (devComm != nullptr  )
    {
        p_in_buff = devComm->p_in_buff;
    }
    else
    {
        // все интерфесы отключены, сделай что нибудь!!!!

        _dataLogger->stop();
    }
}

void controller::setProto(DMA_proto *ECUproto)
{
    qDebug() << "=========== controller::setProto ================" << ECUproto;
    this->ECUproto = ECUproto;

    //this->ECUproto->setCommDev(&devComm);
}

void controller::setLogRate(uint logRate)
{
    _dataLogger->setLogRate(logRate);
}

void controller::connectECU()
{
    qDebug() << "=========== controller::getECUconnect ================" << devComm->getBaudRate();
    if (!ECUproto->connect())
    {
        emit Log("failure get ECU connect " + QString::number( devComm->getBaudRate()));
        return ;
    }

    QByteArray a = ECUproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки

    if ( a.isEmpty() )
    {
        emit Log("failure get ECU rom id");
        return;
    }
    QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
    emit Log("romID: " + romID);

    //emit Log("CurrDir: " + QApplication::applicationDirPath());

    _ecu_definition = new ecu_definition;

    if (!_ecu_definition->fromFile(SearchFiles(QApplication::applicationDirPath() + "/xml/", romID)))
    {
        delete _ecu_definition;
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        emit Log("xml not found");
        return;
    }

    emit ecu_connected();
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(_ecu_definition->RAMtables) )
    {
        emit create_table( getMap(tab) );
    }
    //char f = 0x1f;
    //    devComm->p_out_buff[0] = 0x1e;
    //ECUproto->directDMAwrite(0xfffff000, devComm->p_out_buff, 1);
    _dataLogger->start();
}

void controller::disConnectECU()
{
    _dataLogger->stop();

    qDebug() << "controller::getECUdisconnect _dataLogger->stop";
    devComm->close();
    qDebug() << "controller::getECUdisconnect devComm->close";
    if (_ecu_definition != nullptr)
    {
        delete _ecu_definition;
        _ecu_definition = nullptr;
    }
}

void controller::startLogger()
{
    qDebug()<<"=========== test log ================";
    //QMetaObject::invokeMethod (vehicle_ecu_comm, &comm_device_interface::log0x81, Qt::QueuedConnection);
    //ecu_polling_timer->start(200);
}

void controller::stopLogger()
{

    //ecu_polling_timer->stop();
    //QMetaObject::invokeMethod(vehicle_ecu_comm, &comm_device_interface::log0x81);
}

void controller::RAMreset()
{
    qDebug() << "controller::RAMreset(addr::" << _ecu_definition->DEAD_var << ");";
    quint16 r = 0x0000;
    ECUproto->directDMAwrite(_ecu_definition->DEAD_var, (char*)&r, 2);
}

mapDefinition *controller::getMap(Map *declMap)
{
    //qDebug()<<"ECU_interface::getMap"<<declMap->Name;
    //if(declMap->rom_scaling._storagetype == Storagetype::undef || declMap->rom_scaling._storagetype == Storagetype::bloblist)
    //    return &mapDefinition();
    mapDefinition *defMap = new mapDefinition;
    defMap->declMap = declMap;
    if(declMap->X_axis.addr != 0)
        defMap->X_axis = ECUproto->directDMAread(declMap->X_axis.addr, declMap->X_axis.byteSize());   // читаем оси
    if(declMap->Y_axis.addr != 0)
        defMap->Y_axis = ECUproto->directDMAread(declMap->Y_axis.addr, declMap->Y_axis.byteSize());
    defMap->Map = ECUproto->directDMAread(declMap->addr, declMap->byteSize());
    //emit gettedMap(defMap);
    return defMap;
}

void controller::updateRAM(abstractMemoryScaled memory)
{
    //_dataLogger->stop();
    ECUproto->directDMAwrite(memory.addr, memory.data(), memory.size());
    //_dataLogger->start();
    //QThread::msleep(50);
}

QString controller::SearchFiles(QString path, QString CalID)       // Для поиска файлов в каталоге
{
    // Пытаемся найти правильные файлы, в текущем каталоге
    QStringList listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
    if (listFiles.size()  == 0)            // если файл не найдем вернем егог
        return "";
    //return QFileDialog::getOpenFileName(nullptr,  tr("Open xml"), path, tr("xml files (*.xml)"));
    else
        return path + listFiles.at(0);
}

void controller::init()
{
    _dataLogger = new dataLogger(&_ecu_definition, &devComm, &ECUproto);
    connect(_dataLogger, &dataLogger::logReady, this, &controller::logReady);
    connect(this, &controller::logChanged, _dataLogger, &dataLogger::setLogRate);
}
