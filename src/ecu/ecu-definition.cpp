#include "ecu-definition.h"

ecuDefinition::ecuDefinition()
{
    QThread *this_thread = new QThread();
    QObject::connect(this_thread, &QThread::started, this, [this](){
        pollTimer = new QTimer(this);
        pollTimer->setInterval(50);
    }
    );
    //    //connect(this_thread, &QThread::started, this, &controller::loop, Qt::QueuedConnection);
    connect(this, &ecuDefinition::destroyed, this_thread, &QThread::quit);            // Когда удалим объект остановим поток
    //connect(this, &ecuDefinition::destroyed, pollTimer, &QTimer::deleteLater);            // Когда удалим объект остановим поток
    connect(this_thread, &QThread::finished, this_thread, &QThread::deleteLater);  // Когда остановим поток удалим его
    moveToThread(this_thread);
    this_thread->start();
}

ecuDefinition::~ecuDefinition()
{
    qDebug() << "=========== ~ecuDefinition ================";
    pollTimer->deleteLater();
    for(auto c : qAsConst(RAMtables))
    {
        delete c;
    }
}

bool ecuDefinition::fromFile(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    _parser(file);        // парсим файл
    delete file;
    return true;
}

bool ecuDefinition::fromROMID(QString ROMID)
{
    return fromFile(getFile(QApplication::applicationDirPath() + "/xml/", ROMID));
}

QString ecuDefinition::getFile(QString path, QString CalID)       // Для поиска файлов в каталоге
{
    // Пытаемся найти правильные файлы, в текущем каталоге
    QStringList listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
    if (listFiles.size()  == 0)            // если файл не найдем вернем егог
        return "";
    //return QFileDialog::getOpenFileName(nullptr,  tr("Open xml"), path, tr("xml files (*.xml)"));
    else
        return path + listFiles.at(0);
}

void ecuDefinition::setComDev(comm_device_interface *_devComm)
{
    devComm = _devComm;

    if (devComm == nullptr  )
    {
        // все интерфесы отключены, сделай что нибудь!!!!
        pollTimer->stop();
    }
    else
        ECUproto->setCommDev(&devComm);
}

void ecuDefinition::setDMAproto(DMA_proto *_ECUproto)
{
    ECUproto = _ECUproto;
    _ECUproto->setCommDev(&devComm);
    connect(pollTimer, &QTimer::timeout, this, &ecuDefinition::poll, Qt::QueuedConnection);
}

bool ecuDefinition::connectECU()
{
    qDebug() << "=========== ecuDefinition::connectECU ================" << devComm;
    if (!ECUproto->connect())
    {
        emit Log("failure get ECU connect " + QString::number( devComm->getBaudRate()));
        return false;
    }
    QByteArray a = ECUproto->directDMAread( 0xF52, 4);                        //читаем номер калибровки
    if ( a.isEmpty() )
    {
        emit Log("failure get ECU rom id");
        return false;
    }
    QString romID = QString::number( qFromBigEndian<quint32>(a.data()), 16 );
    emit Log("romID: " + romID);
    //emit Log("CurrDir: " + QApplication::applicationDirPath());
    //===================================================================================================
    if (!fromROMID(romID))
    {
        (*ECUproto->devComm)->close();
        qDebug() << "XML NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!";
        emit Log("xml not found");
        return false;
    }
    ECUproto->RAM_MUT = RAM_MUT;
    ECUproto->RAM_MUT_addr = RAM_MUT_addr;
    //==================================================================================================
    emit ecuConnected();
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(RAMtables) )
    {
        emit createMap( getMap(tab) );
    }
    startLog();
    return true;
}

void ecuDefinition::disConnectECU()
{
    qDebug() << "=========== ecuDefinition::disConnectECU ================";
    pollTimer->stop();
    QThread::msleep(1000);               // костыль
    devComm->close();
    emit ecuDisconnected();
}

void ecuDefinition::RAMreset()
{
    qDebug() << "ecuDefinition::RAMreset(addr::" << DEAD_var << ");";
    quint16 r = 0x0000;
    ECUproto->directDMAwrite(DEAD_var, (char*)&r, 2);
}

void ecuDefinition::updateRAM(abstractMemoryScaled memory)
{
    qDebug()<< "ecuDefinition::updateRAM" << memory.toHex(':');
    ECUproto->directDMAwrite(memory);
}

mapDefinition *ecuDefinition::getMap(Map *declMap)
{
    //qDebug()<<"ecuDefinition::getMap"<<declMap->Name;
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

void ecuDefinition::startLog()
{
    qDebug()<<"=========== ecuDefinition::startLog ================";
    for( int i = 0; i < RAM_MUT.size() ; ++i  )
    {
        RAM_MUT[i].offset = readSize;
        readSize += RAM_MUT[i].scaling.getElementSize();
        //qDebug() << "dataLogger::start" << (*_ecu_definition)->RAM_MUT[i].scaling.name << (*_ecu_definition)->RAM_MUT[i].scaling.getElementSize();
    }
    scaledRAM_MUTvalue.resize(RAM_MUT.size());
    pollTimer->start();
}

void ecuDefinition::poll()
{
    //qDebug() << "jcsbanksDMA::poll" ;
    abstractMemoryScaled a = ECUproto->indirectDMAread(RAM_MUT_addr, readSize);

    //a[0] = abs(QCursor::pos().x())/10;
    //a[1] = abs(QCursor::pos().y())/6;
    for( int i = 0; i < RAM_MUT.size() ; i++  )
    {
        scaledRAM_MUTvalue[i] = a.toFloatOffset( &RAM_MUT[i].scaling, RAM_MUT[i].offset );
    }
    emit logReady(scaledRAM_MUTvalue);
}

void ecuDefinition::_parser(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    //открываем документ
    if (!doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        qWarning("Line %d, column %d: %s", errorLine, errorColumn, "errorStr.ascii()");
        return;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "rom") {
        qWarning("The file is not a rom xml");
        return;
    }
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        const QDomElement el = node.toElement();
        if (el.tagName() == "scaling")                                                 //сохраним все скалинги
            getScaling(el);
        if (el.tagName() == "table")                                                   // находим таблицу
        {
            if (!el.attribute("RAM_addr").isEmpty() )
                getLivemap(el);
            QString nodeName = el.attribute("name");
            if (nodeName == "RAM_MUT")                                     //
            {
                RAM_MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                DEAD_var = node.toElement().attribute("DEAD_var").toUInt(nullptr, 16);
                getMUTparam(node.toElement());
            }
            if (nodeName == "DEAD var")                                    //
                DEAD_var = el.attribute("address").toUInt(nullptr, 16);
        }
        node = node.nextSibling();
    }
    // проставим скалинги
    for(auto c : qAsConst(RAMtables))
    {
        c->setScaling(&scaling_qmap);
    }

}

void ecuDefinition::getMUTparam(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull())
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "ram_mut") //находим параметры мут таблицы
        {
            mutParam _mut_param;
            _mut_param.scaling = scaling_qmap.value(el.attribute("scaling"));

            _mut_param.number = el.attribute("number").toInt(nullptr, 16);
            //++RAM_MUT_count;
            RAM_MUT.insert(_mut_param.number, _mut_param );
        }
        node = node.nextSibling();
    }
}

void ecuDefinition::getLivemap(const QDomElement &element)
{
    Map *mainTableDeclaration = new Map;
    mainTableDeclaration->fromXML(element);
    RAMtables.insert(mainTableDeclaration->Name, mainTableDeclaration);
}

void ecuDefinition::getScaling(const QDomElement &el)
{
    Scaling sc(el);
    scaling_qmap.insert(sc.name, sc);
}
