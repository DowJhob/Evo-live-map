#include "mainwindow.h"
#include "ui_mainwindow.h"

QListWidget *MainWindow::systemLog = nullptr;
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(type);
    Q_UNUSED(context);
    if (MainWindow::systemLog) {
        // Безопасно добавляем текст в QTextEdit
        QMetaObject::invokeMethod(MainWindow::systemLog,
                                  [=]() { MainWindow::systemLog->addItem(msg); }
                                  , Qt::QueuedConnection/*, Q_ARG(QString, msg)*/);
    }
}

MainWindow::MainWindow(QWidget *parent, ecu *ECU) : QMainWindow(parent), ui(new Ui::MainWindow)
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


    //========================================================================================
    _mapManager = new mapManager(this, ECU);
    // this->_mapManager = _mapManager;
    ui->tabWidget->addTab(_mapManager, "Map manager");
    //=============================================================================
    //    ui->tabWidget->addTab(&an, "Ana");

    //        ui->tabWidget->addTab(&plot, "Surface");
    //        ui->tabWidget->addTab(&scat, "Surface");
    systemLog = ui->systemLog;
    qInstallMessageHandler(messageHandler);
    qDebug() << "=========== MainWindow:: ================ QThread:" << thread();
}

void MainWindow::setECUmanager(QToolBar *_ecuManager)
{
    addToolBar(Qt::TopToolBarArea, _ecuManager);

}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";

    // gaugeDelete();
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

void MainWindow::setWBGUAGE(gaugeWidget *wbWgt)
{
    // this->_mapManager = _mapManager;
    addToolBar(Qt::TopToolBarArea, wbWgt);

    // toolbar
    // ui->tabWidget->addTab(wbWgt, "======================");
}
