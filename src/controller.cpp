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
    //case dev_type::OP13  : devComm = new OP13(dev.FunctionLibrary); break;
    case dev_type::OP20  : devComm = new OP20(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    case dev_type::J2534 : devComm = new j2534_interface(dev.FunctionLibrary, dev.DeviceUniqueID); break;
    default              : return;
    }
    p_in_buff = devComm->p_in_buff;

    //connect(devComm, &comm_device_interface::readyInterface, this, &controller::interfaceReady); // форвардим сигнал готовности ннаружу для разблокировки гуя
    connect(devComm, &comm_device_interface::Log, this, &controller::Log, Qt::QueuedConnection);
    connect(this, &controller::baudChanged, devComm, &comm_device_interface::setBaudRate);

    if( devComm->info() )
        emit interfaceReady(true);                  // Показываем кнопки старт и сброс памяти

    //if ( isTactrix )
    {
        //            if( tactrix_afr_lcd == nullptr )
        //            {
        //                tactrix_afr_lcd = new gauge_widget("tactrixAFR", 4, 0, nullptr, ui->toolBar);
        //                ui->toolBar->addWidget(tactrix_afr_lcd);
        //                connect(vehicle_ecu_comm, &comm_device_interface::AFR, tactrix_afr_lcd, &gauge_widget::display);
        //            }
    }
}

void controller::commDeviceRemoved(device dev)
{
    _dataLogger->stop();
    if (devComm != nullptr && devComm->DeviceUniqueID == dev.DeviceUniqueID )
    {
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
    case 0 : ECUproto = new stockDMA(&devComm);break;
    case 1 : ECUproto = new jcsbanksDMA(&devComm);break;
    }
    //qDebug()<<"=========== proto ================";
    //connect(this, &controller::getMap, ECUproto, &ECU_interface::getMap);
    //connect(ECUproto, &ECU_interface::gettedMap, this, &controller::create_table);
    connect(this, &controller::_updateRAM, ECUproto, &ECU_interface::updateRAM);
    connect(this, &controller::_RAMreset, ECUproto, &ECU_interface::RAMreset);

    //connect(ecu_polling_timer, &QTimer::timeout, ECUproto, &ECU_interface::DMApoll);
}

void controller::getECUconnect()
{

    if (!ECUproto->connect()) //тут специфичные для конкретного варианта коннекты например jcsbanks будет 5бод инит
       return ;
    //QMetaObject::invokeMethod(vehicle_ecu_comm, &comm_device_interface::stoplog0x81);
    //QMetaObject::invokeMethod(vehicle_ecu_comm, &comm_device_interface::log0x81);

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

//    getECUdefinition(); //найдем файл конфига и парсим его

    emit ecu_connected();
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(_ecu_definition->RAMtables) )
    {
        emit create_table( ECUproto->getMap(tab) );
    }

    _dataLogger->start();
    //     startLogger();
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

void controller::setLoggingInterval(int im)
{
    //ecu_polling_timer->setInterval(im);
    //ecu_polling_timer->start(im);
}

void controller::RAMreset()
{
    qDebug() << "controller::RAMreset(addr:" << _ecu_definition->DEAD_var << "):";
    emit _RAMreset(_ecu_definition->DEAD_var);
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

void controller::getECUdefinition(QString xml_definition_filename)
{


    //ECUproto->RAM_MUT_addr = _ecu_definition->RAM_MUT_addr;
    //ECUproto->RAM_MUT_len = _ecu_definition->RAM_MUT_count;
}

void controller::init()
{
    _dataLogger = new dataLogger(&_ecu_definition, &devComm, &ECUproto);
    connect(_dataLogger, &dataLogger::logReady, this, &controller::logReady);
    connect(this, &controller::logChanged, _dataLogger, &dataLogger::setLogRate);
}

