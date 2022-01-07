#include <QtCore>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QScrollArea>
#include <QtGlobal>




MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), //enumerator(),
    ui(new Ui::MainWindow)
{
    //=============================================================================
    //    connect(&logger_thread, &QThread::started, ecu_comm, &ECU_interface::init);
    //_logger.moveToThread(&logger_thread);
    //logger_thread.start();


    ui->setupUi(this);

    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;
    //=============================================================================
    statusBar()->showMessage("No interface", 0);
    cpW = new commParamWidget(this);
    connect(cpW, &commParamWidget::interfaceSelected, this, &MainWindow::commDeviceSelected);
    connect(cpW, &commParamWidget::protoSelected, this, &MainWindow::_protoSelected);
    connect(cpW, &commParamWidget::baudChanged, this, &MainWindow::baudChanged);
    connect(cpW, &commParamWidget::logChanged, this, &MainWindow::logChanged);

    //ui->connectionParam->setLayout(new QGridLayout(this));
    ui->connectionParam->layout()->addWidget(cpW);

    //=============================================================================
    hexEdit = new QHexEdit(this);
    hexEdit->setAddressWidth(8);
    hexEdit->setAddressOffset(ui->start_addr_lineEdit->text().toUInt(nullptr, 16));
    //ui->RAMeditorLayout->addWidget(hexEdit, 3,0,1,2);
    static_cast<QGridLayout*>(ui->directHex->layout())->addWidget(hexEdit, 3,0,1,2);
    //=============================================================================
    start_action = ui->toolBar->addAction( QIcon( ":ico/connect.png" ), "Start", this, &MainWindow::StartButton_slot);
    //start_action->setDisabled(true);
    ram_reset = ui->toolBar->addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", this, &MainWindow::RAM_reset);
    ram_reset->setDisabled(true);
    logger = ui->toolBar->addAction( "Start", this, &MainWindow::logger_slot);
    logger->setDisabled(true);

    ui->toolBar->addSeparator();
    QWidget* empty = new QWidget(this);
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->toolBar->addWidget(empty);
    debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, &MainWindow::debugButton_slot);

    readyInterface(false);

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemChecks(QTreeWidgetItem*, int)));





    //mutParam m;
    //create_gauge("AFR", &m);



  //  test();



}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";

    gaugeDelete();
    //_logger.deleteLater();
    //logger_thread.quit();
    //logger_thread.wait(1000);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit _exit();
}

void MainWindow::commDeviceSelected(device dev)
{
    //qDebug()<< "MainWindow::commDeviceSelected" << dev.DeviceDesc;
    statusBar()->showMessage(dev.DeviceDesc, 0);
    emit devSelected(dev);
}

void MainWindow::_protoSelected(int proto)
{    
    qDebug()<< "MainWindow::_protoSelected" << proto;
    emit protoSelected(proto);
}

void MainWindow::deviceEvent(device dev)
{
    switch(dev.direction)
    {
    case dir::arrive :
        cpW->addDevice(dev);
        break;
    case dir::remove :
        cpW->removeDevice(dev);

        emit interfaceRemoved(dev);

        break;
    }

}

void MainWindow::readyInterface(bool lockFlag)
{
    start_action->setDisabled(!lockFlag);
    if(!lockFlag)
    {
        ram_reset->setDisabled(true);
        //logger->setDisabled(true);
    }
    //debug_action->setDisabled(!lockFlag);
    //    for( QObject *w: ui->toolBar->children())
    //        if (w->isWidgetType())
    //            reinterpret_cast<QWidget*>(w)->setDisabled(!lockFlag) ;
}

void MainWindow::StartButton_slot()
{
    if (start_action->text() == "Start")
    {
        emit getECUconnectMainWindow();
    }
    else
    {
        start_action->setText("Start");
        emit getECUdisconnectMainWindow();

        start_action->setChecked(false);
        start_action->setText("Start");
        ram_reset->setDisabled(true);

        gaugeDelete();

        ui->treeWidget->clear();

        for(mapWidget *c: qAsConst(ptrRAMtables))
        {
            c->hide();
            c->deleteLater();
        }
        ptrRAMtables.clear();
    }
}

void MainWindow::logger_slot()
{
    if (logger->text() == "Start")
    {
        logger->setText("Started");
        emit startLogger();
    }
    else
    {
        logger->setText("Start");
        emit stopLogger();
    }
}

void MainWindow::ecu_connected()
{
    start_action->setChecked(true);
    start_action->setText("Stop");
    ram_reset->setDisabled(false);
}

void MainWindow::create_table(mapDefinition *dMap)
{
    //создаем таблицу с заданной размерностью
    //qDebug() << "====================== Create table : " << dMap->declMap->Name << " ================================";
    dMap->declMap->elements = dMap->declMap->X_axis.elements * dMap->declMap->Y_axis.elements;

    mapWidget *table = new mapWidget(nullptr, dMap, &colormap);

    connect(table->mapModel_, &mapModel::updateRAM, this, &MainWindow::updateRAM);

    connect(this, &MainWindow::logReady, table->mapTable, &mapView::logReady);

    connect(this, &MainWindow::_exit, table, &QWidget::deleteLater);

    ptrRAMtables.insert(dMap->declMap->Name, table );

    createMapTree(dMap->declMap);
}

void MainWindow::createMapTree(Map *tab)
{
    if (tab->addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
    {
        QTreeWidgetItem* map_name_item = new QTreeWidgetItem(QStringList() << tab->Name + " RAM address: " + QString::number(tab->addr, 16));
        map_name_item->setFlags(map_name_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        map_name_item->setCheckState(0, Qt::Unchecked);
        QTreeWidgetItem* axis_item;
        if (tab->X_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->X_axis.Name + "  ROM address: " + QString::number(tab->X_axis.addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        if (tab->Y_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->Y_axis.Name + "  ROM address: " + QString::number(tab->Y_axis.addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        ui->treeWidget->addTopLevelItem(map_name_item);
    }
}

void MainWindow::create_gauge(QString name, mutParam *param)
{
    if ( loggerWidgetBar == nullptr)
    {
        loggerWidgetBar = new QToolBar(this);
        loggerWidgetBar->setMovable(true);
        loggerWidgetBar->setMinimumSize(100, 100);
        loggerWidgetBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        //loggerWidgetBar->setAllowedAreas( Qt::BottomToolBarArea);
        //insertToolBar(ui->toolBar, loggerWidgetBar);
        addToolBar(Qt::BottomToolBarArea, loggerWidgetBar);
    }
    gauge_widget *_gauge_widget = new gauge_widget(name, 4, param->offset, &param->scaling, ui->tabWidget);
    //    ui->toolBar->addWidget(_graph_log_widget);
    //ui->logger_verticalLayout->layout()->addWidget(_gauge_widget);
    loggerWidgetBar->addWidget(_gauge_widget);
    gauge_widget_set.insert(_gauge_widget);
}

void MainWindow::colorFromFile(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "file fail";
        //return nullptr;
    }

    QStringList sl;

    //QVector<QColor> *colormap = new QVector<QColor>;

    qDebug() << "file ";
    while( !file->atEnd())
    {
        sl = QString(file->readLine()).simplified()
                .split(' ');
        if(sl.count() == 3)
            colormap.append(QColor(sl[0].toInt(), sl[1].toInt(), sl[2].toInt()));
        //qDebug() << "colormap" ;
    }
    delete file;
}

void MainWindow::test()
{
    QByteArray HighOctaneFuelMapN12Gear{
        "\x8d\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x84\x88\x90\x90\x8d"
        "\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x84\x88\x90\x90\x8d\x80"
        "\x80\x80\x80\x80\x80\x80\x80\x80\x80\x84\x88\x90\x90\x8d\x80\x80"
        "\x80\x80\x80\x80\x80\x80\x80\x80\x84\x88\x90\x90\x8d\x80\x80\x80"
        "\x80\x80\x80\x80\x80\x80\x80\x84\x8c\x96\x96\x8d\x80\x80\x80\x80"
        "\x80\x80\x80\x80\x80\x84\x87\x92\xa3\xa3\x8d\x80\x80\x80\x80\x80"
        "\x80\x80\x80\x84\x87\x8f\x9a\xa7\xa7\x8d\x80\x80\x80\x80\x80\x80"
        "\x80\x83\x86\x8c\x98\xa1\xaa\xaa\x9d\x90\x88\x88\x84\x84\x84\x84"
        "\x88\x8b\x98\x9b\xa4\xac\xac\x9d\x90\x92\x8c\x88\x86\x86\x8a\x8e"
        "\x92\x99\x9d\xa7\xae\xae\xa1\x94\x94\x93\x8c\x88\x88\x93\x99\x99"
        "\x9e\xa6\xab\xb5\xb5\xa5\x98\x96\x96\x90\x8e\x94\x9a\xa0\xa0\xa9"
        "\xab\xb2\xb8\xb8\xa9\x9c\x98\x98\x92\x94\x9f\xa7\xaa\xaa\xb0\xb0"
        "\xb7\xbc\xbc\xad\xa0\x9c\x9c\x96\xa2\xac\xb0\xb2\xb2\xb6\xb7\xbf"
        "\xca\xca\xb1\xa4\xa0\xa0\x9a\xa5\xb4\xba\xba\xba\xbf\xc2\xc6\xca"
        "\xca\xb5\xa8\xa4\xa4\x9e\xa9\xb8\xbe\xbe\xbe\xc3\xc6\xca\xca\xca"
        "\xb9\xac\xa8\xa8\xa2\xad\xbc\xc2\xc2\xc2\xc7\xca\xca\xca\xca\xbd"
        "\xb0\xac\xac\xa6\xb1\xc0\xc6\xc6\xc6\xca\xca\xca\xca\xca", 270};

    abstractMemoryScaled a(HighOctaneFuelMapN12Gear);


    Map *declMap = new Map ;

    declMap->rom_scaling._storagetype = Storagetype::uint8;
    declMap->rom_scaling.frexpr ="14.7*128/x";
    declMap->rom_scaling.toexpr = "14.7*128/x";
    declMap->rom_scaling.endian=true;
    declMap->rom_scaling.format="%.1f";
    declMap->rom_scaling.max = 20;
    declMap->rom_scaling.min =7;
declMap->rom_scaling.setFastNotation();


//declMap->

    declMap->X_axis.elements = 18;
    declMap->Y_axis.elements = 15;

    mapModel *mod = new mapModel(nullptr, declMap);

    //mod->colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;

    auto d = a.fromMemoryA(&declMap->rom_scaling, 270);
    mod->fillMap(&d);

    mapView *vi = new mapView(declMap);

    qDebug() << "set";
    vi->setModel(mod);
    qDebug() << "test" //<< index
                ;
    vi->show();
}

void MainWindow::gaugeDelete()
{
    //foreach (gauge_widget *gauge, gauge_widget_set)
    for (gauge_widget *gauge : qAsConst(gauge_widget_set))
        gauge->deleteLater();
    gauge_widget_set.clear();

}

void MainWindow::debugButton_slot()
{
    debug = true;
    emit ecu_test_connect();
}

void MainWindow::Log(QString str)
{
    ui->systemLog->addItem(str);
}

void MainWindow::on_start_addr_lineEdit_returnPressed()
{
    quint16 count = ui->count_lineEdit->text().toUInt(nullptr);
    quint32 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    //vehicle_ecu_comm->sendDMAcomand(DMAcomand::directRead, addr, count);
    //vehicle_ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)vehicle_ecu_comm->p_in_buff, count));
    hexEdit->setAddressOffset(addr);

}

void MainWindow::on_count_lineEdit_returnPressed()
{
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint32 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    //vehicle_ecu_comm->sendDMAcomand(DMAcomand::directRead, addr, count);
    vehicle_ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)vehicle_ecu_comm->p_in_buff, count));
    hexEdit->setAddressOffset(addr);
}

void MainWindow::itemChecks(QTreeWidgetItem *item, int column)
{
    mapWidget *table = ptrRAMtables.value( item->text(column).split(" RAM").at(0), nullptr );
    if (table == nullptr)
        return;
    if ( item->checkState(column) )
        table->show();
    else
        table->hide();
}
