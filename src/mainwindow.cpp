#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mainToolBar = new mainToolBar(this);
    addToolBar(Qt::TopToolBarArea, _mainToolBar);
    connect(_mainToolBar, &mainToolBar::s_connect, this, &MainWindow::StartButton_slot);
    connect(_mainToolBar, &mainToolBar::s_ramReset, this, &MainWindow::resetRAM);

    //=============================================================================
    cpW = new commParamWidget(this, 62500, 10);
    //connect(cpW, &commParamWidget::interfaceSelected, this, &MainWindow::commDeviceSelected);
    connect(cpW, &commParamWidget::protoSelected, this, &MainWindow::DMAprotoSelected);
    connect(cpW, &commParamWidget::logChanged, this, &MainWindow::logChanged);
    ui->Settings->layout()->addWidget(cpW);
    //=============================================================================
    hexEdit = new hexEditor(this);
    ui->directHex->layout()->addWidget(hexEdit);
    //=============================================================================

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::itemChecks);
    //connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemChecks(QTreeWidgetItem*, int)));
    statusBar()->showMessage("No interface", 0);
    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;
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

void MainWindow::setDeviceManager(deviceManager *devManager)
{
    cpW->setDeviceManager(devManager);
    //settings->layout()->addWidget(devManager);
    connect(devManager, &deviceManager::deviceSelected, this, &MainWindow::deviceEvent);
}

void MainWindow::deviceEvent(comm_device_interface *devComm)
{
    emit deviceSelected(devComm);
    if(devComm == nullptr)
    {
        _mainToolBar->lockConnect(true);
        statusBar()->showMessage("No interface", 0);
        return;
    }
    statusBar()->showMessage(devComm->DeviceUniqueID, 0);
    if( devComm->info() )
        _mainToolBar->lockConnect(false);         // Показываем кнопки старт и сброс памяти
}

void MainWindow::DMAprotoSelected(int proto)
{    
    //qDebug()<< "MainWindow::_protoSelected" << proto;
    emit protoSelected(proto);
}

void MainWindow::StartButton_slot()
{
    if (start_action == "Start")
    {
        qDebug() << "MainWindow::StartButton_slot Start";

        emit getECUconnectMainWindow(cpW->baudRate);
    }
    else
    {
        emit getECUdisconnectMainWindow();

        start_action = "Start";
        _mainToolBar->lockReset( true);
        cpW->setDisabled(false);

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

void MainWindow::ecu_connected()
{
    cpW->setDisabled(true);
    start_action = "Stop";
    _mainToolBar->lockReset( false);
}

void MainWindow::createMap(mapDefinition *dMap)
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

void MainWindow::itemChecks(QTreeWidgetItem *item, int column)
{
    mapWidget *map = ptrRAMtables.value( item->text(column).split(" RAM").at(0), nullptr );
    if (map == nullptr)
        return;
    if ( item->checkState(column) )
        map->show();
    else
        map->hide();
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
    gauge_widget *_gauge_widget = new gauge_widget(name, 4, ui->tabWidget);
    //    ui->toolBar->addWidget(_graph_log_widget);
    //ui->logger_verticalLayout->layout()->addWidget(_gauge_widget);
    loggerWidgetBar->addWidget(_gauge_widget);
    gauge_widget_set.insert(_gauge_widget);
}

void MainWindow::gaugeDelete()
{
    //foreach (gauge_widget *gauge, gauge_widget_set)
    //    for (gauge_widget *gauge : qAsConst(gauge_widget_set))
    //        gauge->deleteLater();
    //    gauge_widget_set.clear();

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

void MainWindow::Log(QString str)
{
    ui->systemLog->addItem(str);
}

void MainWindow::createWB(commDeviceWB *wb)
{
    gauge_widget *_gauge_widget = new gauge_widget("AFR", 4, ui->tabWidget);
    _mainToolBar->addWidget(_gauge_widget);
    _mainToolBar->addSeparator();
}
