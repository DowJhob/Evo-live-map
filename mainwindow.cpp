#include <QtWidgets>
#include <QtCore>
#include "DMA.h"
#include <QDebug>
#include <setupapi.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enumdev.h"
#include "xmldomparser.h"
#include <dbt.h>
#include <QtGlobal>

//#include "common/ecutools.h"

static bool debug;
static enumerator Enumerator;
static dma DMA;
static Timer* timer;
static mathParser2 math;
static DomParser *xmlParser;

static TableProperty_fr_xml Table_Decl;                       //Описание одной таблицы
static QStringList listFiles; //
static QByteArray *binarray;                                  // массив с бинарником

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new Timer(this);
   // connect(timer, SIGNAL(timeout()), SLOT(logger_and_tableWidget_trace()), Qt::DirectConnection);//таймер подключаем
    connect(&Enumerator, SIGNAL(InterfaceActive(int)), &DMA, SLOT(dll_connect(int)), Qt::DirectConnection);
    connect(&Enumerator, SIGNAL(disconnectInterface()), &DMA, SLOT(dll_disconnect()), Qt::DirectConnection);

    connect(this, SIGNAL(timer_lock()), timer, SLOT(timer_lock()), Qt::DirectConnection);
    connect(this, SIGNAL(timer_unlock()), timer, SLOT(timer_unlock()), Qt::DirectConnection);

    connect(&DMA, SIGNAL(timer_lock()), timer, SLOT(timer_lock()), Qt::DirectConnection);
    connect(&DMA, SIGNAL(timer_unlock()), timer, SLOT(timer_unlock()), Qt::DirectConnection);

    timer->setInterval( 1000/ui->logger_rate_textedit->text().toUInt()  );
    ui->StartButton->setDisabled(true);
    ui->RAM_reset_Button->setDisabled(true);
    ui->read_RAM_Button->setDisabled(true);
    Enumerator.enumerateUSB_Device_by_guid();
    statusBar()->showMessage(Enumerator.result);
    ui->StartButton->setDisabled(!Enumerator.VechicleInterfaceState);
    //Подписываемся на события
    Enumerator.NotifyRegister((HWND)this->winId());



QVector<int> axis = {0, 200,500,822,1000,2000,3000,10000};

    QTime start = QTime::currentTime();
    for ( int i = 0; i < 100000; i++)
    {

       //axis_lookup(10, axis.length(),  axis);
    }
    //qDebug() << " time " << start.msecsTo( QTime::currentTime() );



     start = QTime::currentTime();
    for ( int i = 0; i < 10000; i++)
    {
       //qDebug() << "result = " << math_sqrt( 9);
double s = sqrt( 9);
    }
    //qDebug() << " time " << start.msecsTo( QTime::currentTime() );




}

MainWindow::~MainWindow()
{
    TableDelete();
    delete ui;
    delete timer;
    if (xmlParser != nullptr)
    {
        delete xmlParser;
        xmlParser = nullptr;
    }
}

void MainWindow::OperateButtonsLockUnlock()
{
    ui->StartButton->setDisabled(!Enumerator.VechicleInterfaceState);
    ui->RAM_reset_Button->setDisabled(!Enumerator.VechicleInterfaceState);
    ui->read_RAM_Button->setDisabled(!Enumerator.VechicleInterfaceState);
}

void SearchFiles(QString path, QString CalID)       // Для поиска файлов в каталоге
{
    // Пытаемся найти правильные файлы, в текущем каталоге

    //   QApplication::processEvents();    //что бы не замирал интерфейс при овер дохуа файлов в каталоге

    listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
    if (listFiles.size()  == 0)            // если файл не найдем вернем пустую строку
        listFiles[0] = "error";                               // если файл не найдем вернем error
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED( result );
    Q_UNUSED( eventType );
    MSG* msg = static_cast<MSG*>(message);   //reinterpret_cast?
    // Does this specific message interest us?
    if(msg->message == WM_DEVICECHANGE)
    {
        if (msg->wParam == DBT_DEVNODES_CHANGED)
        {
            //qDebug() << "dev node change";
        }
        if (msg->wParam == DBT_DEVICEARRIVAL)
        {
            //  qDebug() << "arrival";
        }
        if ((msg->wParam == DBT_DEVICEREMOVECOMPLETE) || (msg->wParam == DBT_DEVICEARRIVAL))
        {
            qDebug() << "arrival or remove";
            Enumerator.enumerateUSB_Device_by_guid();
            statusBar()->showMessage(Enumerator.result);
            ui->StartButton->setDisabled(!Enumerator.VechicleInterfaceState);

            //            PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)msg->lParam;
            //            if( pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            //            {
            //                PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)msg->lParam;
            //                QString s;

            //                qDebug() << s.fromWCharArray(pDevInf->dbcc_name);
            //                qDebug() << s;
            //            }
        }
    }
    return false;
}

void MainWindow::ReadConfig(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Unable to open file"), file->errorString());
        return;
    }
    xmlParser = new DomParser(file, &math);        // парсим файл
    delete file;
}

void MainWindow::CreateTable(QString filename)
{
    ReadConfig(filename);                                               // прочтем конфиг
    TableProperty_fr_xml tt;                                            // временная переменная для хранения описания таблицы
    for (int i = 0; i < xmlParser->TableDecl_qvector.size(); i++)       // переберем все описания таблиц
    {
        tt = xmlParser->TableDecl_qvector[i];                           //
        if (tt.Table.ram_addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
        {
            DynamicWindow *dynamic_window = new DynamicWindow( this, &tt, &DMA, &math);
            QPushButton *tableButton = new QPushButton(tt.Table.Name, ui->groupBox_mapalloc);
            tableButton->setProperty("tag", tt.tableNum);

            ui->gridLayout_mapalloc->addWidget(tableButton);
            connect(tableButton, SIGNAL(clicked(bool) ), dynamic_window, SLOT(table_show_hide()), Qt::DirectConnection);

           connect(timer, SIGNAL(timeout() ), dynamic_window, SLOT(logger_and_tableWidget_trace() ), Qt::QueuedConnection);
            connect(dynamic_window, SIGNAL(timer_lock() ), timer, SLOT(timer_lock()), Qt::DirectConnection);
            connect(dynamic_window, SIGNAL(timer_unlock() ), timer, SLOT(timer_unlock()), Qt::DirectConnection);

            DMA.win_manager.list_window.insert( tt.tableNum, dynamic_window );
            DMA.win_manager.list_button.insert( tt.tableNum, tableButton );
        }
    }
}

void MainWindow::TableDelete()
{
    foreach (QObject *o, DMA.win_manager.list_window)
    {
        delete o;
    }
    foreach (QObject *o, DMA.win_manager.list_button)
    {
        delete o;
    }
    DMA.win_manager.list_window = {};
    DMA.win_manager.list_button = {};
    DMA.win_manager.list_widget = {};
}

void MainWindow::on_BaudRatelineEdit_textChanged(const QString &arg1)   // Обновляем скорость обмена
{
    DMA.baudRate = arg1.toUInt() ;
}

void MainWindow::on_StartButton_clicked()
{
    QString s;
    unsigned long romIDnum;

    QString xml_filename;

    if (ui->StartButton->text() == "Start")
    {
        if (!DMA.common_five_baud_init())
        {
            ui->listWidget->addItem(s);
            ui->StartButton->setDown(false);
            //   mut_trans->close_interface();
            return;
        }

        ui->listWidget->addItem(s);
        ui->StartButton->setDown(true);

        ui->StartButton->setText("Stop");
        ui->RAM_reset_Button->setDisabled(!Enumerator.VechicleInterfaceState);
        ui->read_RAM_Button->setDisabled(!Enumerator.VechicleInterfaceState);

        DMA.read_direct(0xF52, 4); //читаем номер калибровки
        romIDnum = qFromBigEndian<quint32>(DMA.MUT_In_buffer);


        /*mut_trans->FT_In_Buffer[0] * 0x1000000 +
                mut_trans->FT_In_Buffer[1] * 0x10000 +
                mut_trans->FT_In_Buffer[2] * 0x100 +
                mut_trans->FT_In_Buffer[3];*/
        QString romID =  DMA.ToHex(romIDnum );

        s =   "romID " + romID + "\r\n";
        ui->listWidget->addItem(s);

        QString CurrDir = QApplication::applicationDirPath()+ "\\"  ;   //текущая директория
        SearchFiles(CurrDir, romID);   //найдем файл конфига
        xml_filename = listFiles[0];
        if (xml_filename == "error")
            xml_filename = QFileDialog::getOpenFileName(this, tr("Open xml"), CurrDir, tr("xml files (*.xml)"));
        else
            xml_filename =  CurrDir + xml_filename;
        CreateTable(xml_filename);   								//парсим его

        timer->start(1000/ui->logger_rate_textedit->text().toUInt());

        s =   "CurrDir " + CurrDir + "\r\n";
    }
    else
    {

        ui->StartButton->setText("Start");
        ui->StartButton->setDown(false);
        //   mut_trans->close_interface();
        TableDelete();

    }
}

void MainWindow::on_RAM_reset_Button_clicked()
{
    DMA.timer_lock();
    DMA.MUT_Out_buffer[0] = 0xAD;
    DMA.MUT_Out_buffer[1] = 0xDE;

    DMA.write_direct(xmlParser->DEAD_var, 2);
    on_read_RAM_Button_clicked();
    DMA.timer_unlock();
}

void MainWindow::on_read_RAM_Button_clicked()
{
    DMA.timer_lock();
    for(int i=0; i < DMA.win_manager.list_window.count(); i++)
    {
        DynamicWindow *tablewindow = qobject_cast<DynamicWindow*>(DMA.win_manager.list_window[i]);
        DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>(DMA.win_manager.list_widget[i]);
        tablewidget->blockSignals(true);//перед обновлением отключим сигнал автообновления ячейки
        tablewindow->table_set_update(tablewidget);
        tablewidget->blockSignals(false);
        qDebug() << "refresh: " << tablewidget->Table_Decl.Table.Name;
    }
    DMA.timer_unlock();
}

void MainWindow::on_logger_rate_textedit_editingFinished()
{
    timer->setInterval(1000/ui->logger_rate_textedit->text().toUInt() );
}

void MainWindow::on_debugButton_clicked()
{
    debug = true;
    QString CurrDir = QApplication::applicationDirPath()+ "/"  ;   //текущая директория
    SearchFiles(CurrDir, "80700010");   //найдем файл конфига
    QString xml_filename = listFiles[0];
    for (uchar i =0; i < 255; i++)
    {
        DMA.MUT_In_buffer[i] = i;
        DMA.MUT_Out_buffer[i] = i;
    }
    if (xml_filename == "error")
        xml_filename = QFileDialog::getOpenFileName(this, tr("Open xml"), CurrDir, tr("xml files (*.xml)"));
    else
        xml_filename =  CurrDir + xml_filename;
    CreateTable(xml_filename);   								//парсим его
    timer->start(1000/ui->logger_rate_textedit->text().toUInt());

    timer->start(650);
}

void MainWindow::on_loadbinbutton_clicked()
{
    QString CurrDir = QApplication::applicationDirPath()+ "\\"  ;   //текущая директория
    QString bin_filename = QFileDialog::getOpenFileName(this, tr("Open .bin"), CurrDir, tr("bin files (*.bin)"));
    QFile binfile(bin_filename);
    if (!binfile.open(QIODevice::ReadWrite ))
        return;

    binarray = new QByteArray( binfile.read(binfile.size()));     // новый массив

    uint romIDnum = qFromBigEndian<quint32>(binarray->mid(0xf52, 4)); // номер калибровки
    QString romID =  DMA.ToHex(romIDnum );

    SearchFiles(CurrDir, romID);   //найдем файл конфига
    QString xml_filename = listFiles[0];
    if (xml_filename == "error")
        xml_filename = QFileDialog::getOpenFileName(this, tr("Open xml"), CurrDir, tr("xml files (*.xml)"));
    else
        xml_filename =  CurrDir + xml_filename;
    ReadConfig(xml_filename);   								//парсим его

    qDebug() << "";

}

void MainWindow::on_stop_live_clicked()
{
    TableDelete();
    debug = false;
}

void MainWindow::on_verticalSlider_2_sliderMoved(int position)
{

}
