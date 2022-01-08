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
    hexEdit = new hexEditor(this);
    ui->directHex->layout()->addWidget(hexEdit);
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
    //debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, &MainWindow::debugButton_slot);

    readyInterface(false);

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::itemChecks);
    //connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemChecks(QTreeWidgetItem*, int)));

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
        qDebug() << "MainWindow::StartButton_slot Start";
        emit getECUconnectMainWindow();
    }
    else
    {
        emit getECUdisconnectMainWindow();

        start_action->setText("Start");
        start_action->setChecked(false);
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

void MainWindow::ecu_connected(//QHash<QString, Map*> m
                               )
{
    start_action->setChecked(true);
    start_action->setText("Stop");
    ram_reset->setDisabled(false);

    // переберем все описания таблиц
//    for ( Map *tab : qAsConst(m) )
//    {
//        emit create_table( ECUproto->getMap(tab) );
//    }
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
    gauge_widget *_gauge_widget = new gauge_widget(name, 4, param->offset, &param->scaling, ui->tabWidget);
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
