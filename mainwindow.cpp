#include <QtWidgets>
#include <QtCore>
#include "DMA.h"
#include <QDebug>
#include <setupapi.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmldomparser.h"
#include <dbt.h>
#include <QtGlobal>

//#include "common/ecutools.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), SLOT(logger_and_tableWidget_trace()), Qt::DirectConnection);//таймер подключаем
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




connect(&fpss, SIGNAL(timeout()), SLOT(fpssss()));
fpss.start(1000);




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
            DynamicWindow *dynamic_window = new DynamicWindow( this, &tt, &DMA, &math, &list_widget);
            QPushButton *tableButton = new QPushButton(tt.Table.Name, ui->groupBox_mapalloc);
            tableButton->setProperty("tag", tt.tableNum);

            ui->gridLayout_mapalloc->addWidget(tableButton);
            connect(tableButton, SIGNAL(clicked(bool) ), this, SLOT( table_show_hide() ), Qt::DirectConnection);


            connect(this, SIGNAL(timer_lock() ), timer, SLOT(timer_lock()), Qt::DirectConnection);
            connect(this, SIGNAL(timer_unlock() ), timer, SLOT(timer_unlock()), Qt::DirectConnection);

            list_window.insert( tt.tableNum, dynamic_window );
            list_button.insert( tt.tableNum, tableButton );
        }
    }


}

void MainWindow::TableDelete()
{
    foreach (QObject *o, list_window)
    {
        delete o;
    }
    foreach (QObject *o, list_button)
    {
        delete o;
    }
    list_window = {};
    list_button = {};
    list_widget = {};
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
    for(int i=0; i < list_window.count(); i++)
    {
        DynamicWindow *tablewindow = qobject_cast<DynamicWindow*>(list_window[i]);
        DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>(list_widget[i]);
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

   // timer->start(650);
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
int modul(int a, int b)
{
    int mod = qRound(sqrt(pow(a, 2) + pow(b, 2)));
    if (mod > 255)
        mod = 255;

    mod = (255 - mod);
    if (mod < 30)
        mod = 30;
    return mod;
}
void  axis_lookup(int in, int axis_lenght, QVector<int> axis, Tracer_marker *marker)
{

    if (in < axis[0])
        in = axis[0];
    if (in > axis[axis_lenght - 1])
        in = axis[axis_lenght - 1];
    for (int i = 0; i < axis_lenght; i++)
    {
        if (  in >= axis[i] && in < axis[i + 1])
        {
            marker->a = i;
            break;
        }
    }
    if ( marker->a >= (axis_lenght - 1) )
        marker->b = marker->a;
    else
        marker->b = marker->a + 1;

    //return marker;

}

void MainWindow::logger_and_tableWidget_trace()
    {
    fps++;
QTime start = QTime::currentTime();
        bool debug = true;

        emit timer_lock();
        // DMA.read_indirect(xmlParser->RAM_MUT_addr, 16);// логгинг что бы контроллер не уснул
        /* Ищем объект, в списке*/
//        float x = 0;
//        float y = 0;




        foreach (QObject *o, list_widget)
        {
            DynamicTableWidget *tablewidget = reinterpret_cast<DynamicTableWidget*>(o);


//for (int i=0; i < 4; i++)
//{
//     DynamicTableWidget *tablewidget = reinterpret_cast<DynamicTableWidget*>(list_widget.at(i));

            tablewidget->blockSignals( true );

            // читаем из буфера и кастуем

            if (!debug)
            {

                x = read_and_cast(tablewidget->Table_Decl.X_axis.ram_scaling.storagetype.isEmpty(),
                                  tablewidget->Table_Decl.X_axis.ram_scaling.storagetype,
                                  tablewidget->Table_Decl.X_axis.ram_scaling.ram_mut_number,
                                  tablewidget->Table_Decl.X_axis.scaling.endian,
                                  tablewidget->Table_Decl.X_axis.scaling.frexpr2);


                y = read_and_cast(tablewidget->Table_Decl.Y_axis.ram_scaling.storagetype.isEmpty(),
                                  tablewidget->Table_Decl.Y_axis.ram_scaling.storagetype,
                                  tablewidget->Table_Decl.Y_axis.ram_scaling.ram_mut_number,
                                  tablewidget->Table_Decl.Y_axis.scaling.endian,
                                  tablewidget->Table_Decl.Y_axis.scaling.frexpr2);

            }
            else
            {
                x = x + 10;
                y = y + 100;

                if (x > 2700)
                {
                   x = 0;
                }
                if (y >= 28000)
                {
                   y = 0;
                }
            }

            //----------------------- вычисляем координаты маркера------------------------------------------------
            axis_lookup(qRound(x),
                        tablewidget->Table_Decl.X_axis.elements,
                        tablewidget->x_axis,
                        &tablewidget->tracer_marker_X);
            axis_lookup(qRound(y),
                        tablewidget->Table_Decl.Y_axis.elements,
                        tablewidget->y_axis,
                        &tablewidget->tracer_marker_Y);
            //----------------------- вычисляем насыщенность ячеек маркера хидера---------------------------------------
            //----------------------- ось X

            int ax_X_len = tablewidget->x_axis[tablewidget->tracer_marker_X.b] - tablewidget->x_axis[tablewidget->tracer_marker_X.a];
            int ax_Y_len = tablewidget->y_axis[tablewidget->tracer_marker_Y.b] - tablewidget->y_axis[tablewidget->tracer_marker_Y.a];

            int EndXvect = (x - tablewidget->x_axis[tablewidget->tracer_marker_X.a] - ax_X_len/2) * 255 / ax_X_len;  //нормированные координаты концов всех векторов
            int EndYvect = (tablewidget->y_axis[tablewidget->tracer_marker_Y.b] - y - ax_Y_len/2) * 255 / ax_Y_len;  //нормированные координаты концов всех векторов

            int left_vectX = EndXvect + 178; // EndXvect - (-89)
            int right_vectX = EndXvect - 178;
            int up_vectY = EndYvect - 178;
            int down_vectY = EndYvect + 178;

            //модули векторов,
            int leftUP = modul(left_vectX, up_vectY);
            int rightUP  = modul(right_vectX, up_vectY);
            int leftDOWN   = modul(left_vectX, down_vectY);
            int rightDOWN    = modul(right_vectX, down_vectY);

            QColor color_rightDOWN, color_leftDOWN, color_rightUP, color_leftUP;
            color_rightDOWN.setHsv(240, rightDOWN, 255, 255);
            color_leftDOWN.setHsv(240, leftDOWN, 255, 255);
            color_rightUP.setHsv(240, rightUP, 255, 255);
            color_leftUP.setHsv(240, leftUP, 255, 255);

            //------------------------------------------------------------------------------------------------------------------
            if ((tablewidget->tracer_marker_pred_X.a != tablewidget->tracer_marker_X.a)
                    ||(tablewidget->tracer_marker_pred_Y.a != tablewidget->tracer_marker_Y.a)) // тут гашение если изменился X или Y
            {
                //гашение предыдущих маркеров на хидерах
//                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.a)->setBackground(Qt::white);
//                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.b)->setBackground(Qt::white);
//                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.a)->setBackground(Qt::white);
//                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.b)->setBackground(Qt::white);
                //гашение предыдущего маркера таблицы
                tablewidget->item(tablewidget->tracer_marker_pred_Y.a, tablewidget->tracer_marker_pred_X.a)->setBackground(Qt::white);
                tablewidget->item(tablewidget->tracer_marker_pred_Y.a, tablewidget->tracer_marker_pred_X.b)->setBackground(Qt::white);
                tablewidget->item(tablewidget->tracer_marker_pred_Y.b, tablewidget->tracer_marker_pred_X.a)->setBackground(Qt::white);
                tablewidget->item(tablewidget->tracer_marker_pred_Y.b, tablewidget->tracer_marker_pred_X.b)->setBackground(Qt::white);
            }

            //сохраняем  текущее положение для след расчета
            tablewidget->tracer_marker_pred_X = tablewidget->tracer_marker_X;
            tablewidget->tracer_marker_pred_Y = tablewidget->tracer_marker_Y;

            //         рисуем новое положение маркеров на хидерах
//            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.a)->setBackground(color_leftUP);
//            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.b)->setBackground(color_rightUP);
//            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.a)->setBackground(color_leftUP);
//            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.b)->setBackground(color_leftDOWN);

            //         рисуем новое положение маркера
            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.a)->setBackground(color_leftUP);//левый верхний
            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.b)->setBackground(color_rightUP);//правый верхний

            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.a)->setBackground(color_leftDOWN);//левый нижний
            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.b)->setBackground(color_rightDOWN);//правый нижний
            //
            // разблокируем обновления редакции
            tablewidget->blockSignals(false);//

            //QTime start = QTime::currentTime();
            //tablewidget->repaint();
          //  tablewidget->update(tablewidget->tracer_marker_X.a, tablewidget->tracer_marker_Y.a, 2, 2);



        }

        emit timer_unlock();
          //   qDebug() <<  "fucking time " << start.msecsTo( QTime::currentTime() );
    }
