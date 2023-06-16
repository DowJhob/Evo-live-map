#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //=============================================================================
    addToolBar(Qt::TopToolBarArea, &_ecuManager);
    connect(&_ecuManager, &ecuManager::ecuConnected,    this,         &MainWindow::ecuConnected);
    connect(&_ecuManager, &ecuManager::ecuDisconnected, this,         &MainWindow::ecuDisconnected);
//    connect(&_ecuManager, &ecuManager::createMap,       &_mapManager, &mapManager::createMap);
    connect(&_ecuManager, &ecuManager::Log,             this,         &MainWindow::Log);
    connect(&_ecuManager, &ecuManager::deviceEventLog,  this,         &MainWindow::deviceEventLog);
    //=============================================================================
    //=============================================================================
    ui->tabWidget->addTab(&_mapManager, "Map manager");
    _mapManager.setECUmanager(&_ecuManager);

    connect(&_ecuManager, &ecuManager::s_test, &_mapManager, &mapManager::s_test, Qt::QueuedConnection);






    //=============================================================================
    ui->tabWidget->addTab(&hexEdit, "Hex editor");
    //=============================================================================
    ui->tabWidget->addTab(&_loggerManager, "Logger");
    //=============================================================================
    ui->tabWidget->addTab(&patcher, "Patcher");
    //=============================================================================
//    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::itemChecks);
    statusBar()->showMessage("No interface", 0);
//    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;











//    QByteArray src("dsssssssssssssssssssssssssssssssssssssssssssssssssssssss");
//    qDebug() << src.count();
//    QByteArray dst = src.d
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
    QApplication::closeAllWindows();
}

void MainWindow::setUSBfilter(deviceNativeFilter *usbFilter)
{
    _ecuManager.setUSBfilter(usbFilter);
}

void MainWindow::deviceEventLog(QString msg, int pos)
{
    statusBar()->showMessage(msg, pos);
}

void MainWindow::ecuDisconnected()
{
    gaugeDelete();

    _mapManager.clearMaps();
}

void MainWindow::ecuConnected(QHash<QString, Map*> *RAMtables)
{
    _mapManager.createMapS( RAMtables);
    _ecuManager.startLog();
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

void MainWindow::Log(QString str)
{
    ui->systemLog->addItem(str);
}
