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
    getECUdisconnect();

    if (ECUproto != nullptr)
        ECUproto->deleteLater();


    if (devComm != nullptr)
        devComm->deleteLater();
    qDebug() << "~controller";
}

void controller::start()
{
    this_thread->start();  // запустим поток,
}

void controller::commDeviceSelected(device dev)
{
    if (devComm != nullptr  )
    {
        devComm->close();
        devComm->deleteLater();
    }
    switch (dev.type) {
    case deviceType::OP13  : devComm = new OP13(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    case deviceType::OP20  : devComm = new OP20(dev.FunctionLibrary, dev.DeviceUniqueID);
        emit getWB(reinterpret_cast<commDeviceWB*>(devComm));
        //qDebug()<< "controller::commDeviceSelected" << dev.DeviceDesc + " / WB serial interface";
        break;
    case deviceType::J2534 : devComm = new j2534_interface(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    default              : return;
    }
    p_in_buff = devComm->p_in_buff;

    //connect(devComm, &comm_device_interface::readyInterface, this, &controller::interfaceReady); // форвардим сигнал готовности ннаружу для разблокировки гуя
    connect(devComm, &comm_device_interface::Log, this, &controller::Log, Qt::QueuedConnection);
    //connect(this, &controller::baudChanged, devComm, &comm_device_interface::setBaudRate);

    if( devComm->info() )
        emit interfaceReady(true);                  // Показываем кнопки старт и сброс памяти
}

void controller::commDeviceRemoved(device dev)
{
    if (devComm != nullptr && devComm->DeviceUniqueID == dev.DeviceUniqueID )
    {
        _dataLogger->stop();
        //devComm->close();
        devComm->deleteLater();
        devComm = nullptr;
    }
}

void controller::setProto(int proto)
{
    if ( ECUproto != nullptr )
        ECUproto->deleteLater();
    //qDebug()<< "setProto" << proto;
    switch (proto) {
    case 0 : ECUproto = new jcsbanksDMA(&devComm);break;
    case 1 : ECUproto = new stockDMA(&devComm);break;
    case 2 : ECUproto = new evoX_DMA(&devComm);break;
    }
    //qDebug()<<"=========== proto ================";
    //connect(this, &controller::getMap, ECUproto, &ECU_interface::getMap);
    //connect(ECUproto, &ECU_interface::gettedMap, this, &controller::create_table);

    //connect(ecu_polling_timer, &QTimer::timeout, ECUproto, &ECU_interface::DMApoll);
}

void controller::getECUconnect(uint baudRate)
{
    qDebug() << "=========== controller::getECUconnect ================";
    if (!ECUproto->connect(baudRate))
    {
        emit Log("failure get ECU connect " + QString::number( baudRate));
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

    _dataLogger->start();
}

void controller::getECUdisconnect()
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
    ECUproto->directDMAwrite(memory.addr, memory.data(), memory.size());
    QThread::msleep(50);
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

void controller::setBaudRate(int baudRate)
{
    qDebug() << "baud rate changed" << baudRate;
    //this->baudRate = baudRate;
}
