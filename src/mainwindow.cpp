#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addToolBar(Qt::TopToolBarArea, &_ecuManager);
    connect(&_ecuManager, &ecuManager::disConnectECUaction, this, &MainWindow::disConnectECUaction);
    connect(&_ecuManager, &ecuManager::ecu_connected,       this, &MainWindow::ecu_connected);
    connect(&_ecuManager, &ecuManager::create_table,        this, &MainWindow::createMap);
    connect(&_ecuManager, &ecuManager::Log,                 this, &MainWindow::Log);
    //========================================================================================
    _ecuManager.addWidget(&wbWgt);
    _ecuManager.addSeparator();
    //=============================================================================
    setCPW();
    //=============================================================================
    hexEdit = new hexEditor(this);
    ui->directHex->layout()->addWidget(hexEdit);
    //=============================================================================
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::itemChecks);
    statusBar()->showMessage("No interface", 0);
    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";

    gaugeDelete();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit _exit();
}

void MainWindow::setUSBfilter(deviceNativeFilter *usbFilter)
{
    connect(usbFilter, &deviceNativeFilter::deviceEvent, &cpW.devManager, &deviceManager::deviceEvent);
    //connect(usbFilter, &deviceNativeFilter::deviceEvent, this, &MainWindow::deviceEvent);
}

void MainWindow::setCPW()
{
    ui->Settings->layout()->addWidget(&cpW);

    connect(&cpW._protoManager, &protoManager::protoSelected,     &_ecuManager,     &ecuManager::setProto);
    connect(&cpW.devManager,    &deviceManager::deviceSelected,   &_ecuManager,     &ecuManager::setCommDevice);
    //QObject::connect(&cpW._protoManager, &protoManager::logRateChanged,    &controller,     &ecuManager::setLogRate);
    cpW._protoManager.addProtos();   // костыль пока

    connect(&cpW.devManager, &deviceManager::deviceSelected, this, &MainWindow::deviceEvent);
    connect(&cpW._wbManager,    &wbManager::logReady,             &wbWgt,          &gaugeWidget::display);

    cpW._wbManager.fillSerial();
    cpW._wbManager.fillProto();
}

void MainWindow::deviceEvent(comm_device_interface *devComm)
{
    if(devComm == nullptr)
    {
        _ecuManager.lockConnect(true);
        statusBar()->showMessage("No interface", 0);
        return;
    }
    statusBar()->showMessage(devComm->DeviceDesc + " / " + devComm->DeviceUniqueID, 0);
    if( devComm->info() )
        _ecuManager.lockConnect(false);         // Показываем кнопки старт и сброс памяти
}

void MainWindow::disConnectECUaction()
{
    cpW.setDisabled(false);

    gaugeDelete();

    ui->treeWidget->clear();

    for(mapWidget *c: qAsConst(ptrRAMtables))
    {
        c->hide();
        c->deleteLater();
    }
    ptrRAMtables.clear();
}

void MainWindow::ecu_connected()
{
    cpW.setDisabled(true);
}

void MainWindow::createMap(mapDefinition *dMap)
{
    //создаем таблицу с заданной размерностью
    //qDebug() << "====================== Create table : " << dMap->declMap->Name << " ================================";
    dMap->declMap->elements = dMap->declMap->X_axis.elements * dMap->declMap->Y_axis.elements;

    mapWidget *table = new mapWidget(nullptr, dMap, &colormap);

    //connect(table->mapModel_, &mapModel::updateRAM, this, &MainWindow::updateRAM);
    connect(table->mapModel_, &mapModel::updateRAM, &_ecuManager, &ecuManager::updateRAM);

    //connect(this, &MainWindow::dataLog, table->mapTable, &mapView::logReady);
    connect(&_ecuManager, &ecuManager::logReady, table->mapTable, &mapView::logReady);

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
    gaugeWidget *_gauge_widget = new gaugeWidget(name, 4, ui->tabWidget);
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
