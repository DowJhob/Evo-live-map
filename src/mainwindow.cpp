#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //=============================================================================
    ui->tabWidget->addTab(&hexEdit, "Hex editor");
    //=============================================================================
    ui->tabWidget->addTab(&_loggerManager, "Logger");
    //=============================================================================
    ui->tabWidget->addTab(&patcher, "Patcher");
    //=============================================================================
    statusBar()->showMessage("No interface", 0);


    //=============================================================================
//    ui->tabWidget->addTab(&an, "Ana");

//        ui->tabWidget->addTab(&plot, "Surface");
//        ui->tabWidget->addTab(&scat, "Surface");


    qDebug() << "=========== MainWindow:: ================ QThread:" << thread();
}

void MainWindow::setECUmanager(QToolBar *_ecuManager)
{
    addToolBar(Qt::TopToolBarArea, _ecuManager);
}

void MainWindow::setMAPmanager(mapManager *_mapManager)
{
    this->_mapManager = _mapManager;
    ui->tabWidget->addTab(_mapManager, "Map manager");
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

void MainWindow::deviceEventLog(QString msg, int pos)
{
    statusBar()->showMessage(msg, pos);
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
