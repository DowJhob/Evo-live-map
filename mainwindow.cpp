#include <QtWidgets>
#include <QtCore>
#include "DMA.h"
#include <QDebug>
#include <setupapi.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dbt.h>
#include <QtGlobal>

//#include "common/ecutools.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CurrDir = QApplication::applicationDirPath();   //текущая директории
    connect(timer, SIGNAL(timeout()), SLOT(logger_and_tableWidget_trace()));
    connect(&Enumerator, SIGNAL(InterfaceActive(int)), &DMA, SLOT(dll_connect(int)));
    connect(&Enumerator, SIGNAL(disconnectInterface()), &DMA, SLOT(dll_disconnect()));

    connect(&DMA, SIGNAL(Log(QString)), this, SLOT(Log(QString)));

    timer->setInterval( 1000/ui->logger_rate_textedit->text().toUInt()  );
    ui->StartButton->setDisabled(true);
    ui->RAM_reset_Button->setDisabled(true);
    ui->read_RAM_Button->setDisabled(true);
    Enumerator.enumerateUSB_Device_by_guid();
    statusBar()->showMessage(Enumerator.result);
    ui->StartButton->setDisabled(!Enumerator.VechicleInterfaceState);
    //Подписываемся на события
    Enumerator.NotifyRegister((HWND)this->winId());
    hexEdit = new QHexEdit;
    hexEdit->setAddressWidth(8);
    hexEdit->setAddressOffset(ui->start_addr_lineEdit->text().toUInt(nullptr, 16));
    ui->RAMeditorLayout->addWidget(hexEdit, 3,0,1,2);


}

MainWindow::~MainWindow()
{
    TableDelete();
    delete ui;
    delete timer;
}

void MainWindow::OperateButtonsLockUnlock()
{
    ui->StartButton->setDisabled(!Enumerator.VechicleInterfaceState);
    ui->RAM_reset_Button->setDisabled(!Enumerator.VechicleInterfaceState);
    ui->read_RAM_Button->setDisabled(!Enumerator.VechicleInterfaceState);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED( result )
    Q_UNUSED( eventType )
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

bool MainWindow::ReadConfig(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Unable to open file"), file->errorString());
        return false;
    }

    xmlParser._parser(file, _ecu);        // парсим файл
    delete file;
    return true;
}

void MainWindow::axread(sub_tableDeclaration *sub_tab, QVector <qint64> *axis)
{
    float variable_value;
    //читаем таблицу заголовка-оси в буфер
    // прочитаем нужное количество данных в соответствии с типом
    switch (sub_tab->rom_scaling._storagetype) {                                //тип данных оси
    case Storagetype::int8:
    case Storagetype::uint8:  DMA.read_direct( sub_tab->rom_addr, sub_tab->elements); break;
    case Storagetype::int16:
    case Storagetype::uint16: DMA.read_direct( sub_tab->rom_addr, sub_tab->elements * 2); break;
    case Storagetype::int32:
    case Storagetype::uint32: DMA.read_direct( sub_tab->rom_addr, sub_tab->elements * 4); break;
    default: break;
    }
    //заполняем в соотвествии с формулой
    for (int i = 0; i < sub_tab->elements; i++)
    {
        variable_value = _ecu->mem_cast(sub_tab->rom_scaling, DMA.MUT_Out_buffer, i); //кастуем данные к определенному типу
        int compute = qRound(fast_calc(sub_tab->rom_scaling.toexpr2, variable_value));
        axis->append(compute);
    }
}

bool MainWindow::CreateTable(QString filename)
{
    _ecu = new ecu;
    if (!ReadConfig(filename))
        return false;
    // переберем все описания таблиц
    foreach ( tableDeclaration tab, _ecu->loggingRAMtables)
    {
        if (tab.Table.ram_addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
        {
            mapWidget *dynamic_window = new mapWidget( this, &tab);
            axread(&tab.X_axis, &dynamic_window->x_axis);   // читаем оси
            axread(&tab.Y_axis, &dynamic_window->y_axis);   // читаем оси
            QVector <qint64> map;
            tab.Table.elements = tab.X_axis.elements * tab.Y_axis.elements;
            axread(&tab.Table, &map);
            dynamic_window->create( &map);
            connect(dynamic_window, SIGNAL(update(quint32, QString)), SLOT(updateRAM(quint32, QString)));

            QPushButton *tableButton = new QPushButton(tab.Table.Name, ui->groupBox_mapalloc);
            tableButton->setProperty("tag", tab.tableNum);
            ui->gridLayout_mapalloc->addWidget(tableButton);
            connect(tableButton, SIGNAL(clicked(bool) ), this, SLOT( table_show_hide() ));

            list_table.insert( tab.tableNum, dynamic_window->table );

            list_window.insert( tab.tableNum, dynamic_window );
            list_button.insert( tab.tableNum, tableButton );
        }
    }
    foreach ( tableDeclaration tab, _ecu->not_loggingRAMtables)
    {
        if (tab.Table.ram_addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
        {
            mapWidget *dynamic_window = new mapWidget( this, &tab);
            axread(&tab.X_axis, &dynamic_window->x_axis);   // читаем оси
            axread(&tab.Y_axis, &dynamic_window->y_axis);   // читаем оси
            QVector <qint64> map;
            tab.Table.elements = tab.X_axis.elements * tab.Y_axis.elements;
            axread(&tab.Table, &map);
            dynamic_window->create( &map);
connect(dynamic_window, SIGNAL(update(quint32, QString)), SLOT(updateRAM(quint32, QString)));


            QPushButton *tableButton = new QPushButton(tab.Table.Name, ui->groupBox_mapalloc);
            tableButton->setProperty("tag", tab.tableNum);
            ui->gridLayout_mapalloc->addWidget(tableButton);
            connect(tableButton, SIGNAL(clicked(bool) ), this, SLOT( table_show_hide() ));

            list_table.insert( tab.tableNum, dynamic_window->table );

            list_window.insert( tab.tableNum, dynamic_window );
            list_button.insert( tab.tableNum, tableButton );
        }
    }
    return true;
}

void MainWindow::TableDelete()
{
    foreach (mapWidget *window, list_window)
    {
        delete window;
    }
    foreach (QPushButton *button, list_button)
    {
        delete button;
    }
    list_window = {};
    list_button = {};
    //    list_widget = {};
}

void MainWindow::logger_and_tableWidget_trace()
{
    QElapsedTimer t;
    t.start();
    timer->stop();
    DMA.read_indirect(_ecu->RAM_MUT_addr, 16);
    int x = 0;
    int y = 0;
    foreach (mapWidget *window, list_window)
    {
        if ( window->Table_Decl.X_axis.ram_mut_number || window->Table_Decl.Y_axis.ram_mut_number )
        {
            x = qRound(_ecu->mut_cast(DMA.MUT_Out_buffer, window->Table_Decl.X_axis.RAM_MUT_scaling, window->Table_Decl.X_axis.ram_mut_number));
            y = qRound(_ecu->mut_cast(DMA.MUT_Out_buffer, window->Table_Decl.Y_axis.RAM_MUT_scaling, window->Table_Decl.Y_axis.ram_mut_number));
            if (debug)
            {
                x = QCursor::pos().x();
                y = QCursor::pos().y()*10;
            }
            //----------------------- вычисляем координаты маркера------------------------------------------------
            window->tracer_marker.Xtrace = axis_lookup2(x, window->Table_Decl.X_axis.elements, window->x_axis);
            window->tracer_marker.Ytrace = axis_lookup2(y, window->Table_Decl.Y_axis.elements, window->y_axis);
            //----------------------- вычисляем насыщенность ячеек маркера ---------------------------------------
            window->tracer_marker.k = 0;
            window->tracer_marker.j = 0;
            if ( (window->Table_Decl.X_axis.elements > 1) && (window->tracer_marker.Xtrace < window->Table_Decl.X_axis.elements - 1) )
                window->tracer_marker.j = 1;
            if ( (window->Table_Decl.Y_axis.elements > 1) && (window->tracer_marker.Ytrace < window->Table_Decl.Y_axis.elements - 1) )
                window->tracer_marker.k = 1;

            float kX = (float)255/(window->x_axis[window->tracer_marker.Xtrace+window->tracer_marker.j] - window->x_axis[window->tracer_marker.Xtrace]);    //коэффициент нормирования
            float kY = (float)255/(window->y_axis[window->tracer_marker.Ytrace+window->tracer_marker.k] - window->y_axis[window->tracer_marker.Ytrace]);    //коэффициент нормирования

            int leftNormalX  = qRound((window->x_axis[window->tracer_marker.Xtrace]-x)*kX);                       //нормированные координаты
            int rightNormalX = qRound((window->x_axis[window->tracer_marker.Xtrace+window->tracer_marker.j]-x)*kX);                       //нормированные координаты
            int upNormalY    = qRound((window->y_axis[window->tracer_marker.Ytrace]-y)*kY);
            int downNormalY  = qRound((window->y_axis[window->tracer_marker.Ytrace+window->tracer_marker.k]-y)*kY);

            //модули векторов,
            int leftUP    = modul(leftNormalX, upNormalY);
            int rightUP   = modul(rightNormalX, upNormalY);
            int leftDOWN  = modul(leftNormalX, downNormalY);
            int rightDOWN = modul(rightNormalX, downNormalY);

            QColor color_rightDOWN, color_leftDOWN, color_rightUP, color_leftUP;
            color_rightDOWN.setHsv(240, rightDOWN, 255, 255);
            color_leftDOWN.setHsv(240, leftDOWN, 255, 255);
            color_rightUP.setHsv(240, rightUP, 255, 255);
            color_leftUP.setHsv(240, leftUP, 255, 255);
            //------------------------------------------------------------------------------------------------------------------
            window->table->blockSignals( true );
            if ((window->tracer_marker_pred.Xtrace != window->tracer_marker.Xtrace) ||(window->tracer_marker_pred.Ytrace != window->tracer_marker.Ytrace)) // тут гашение если изменился X или Y
            {
                //гашение предыдущего маркера таблицы
                if (!save_trace)
                {
                    window->table->item(window->tracer_marker_pred.Ytrace,   window->tracer_marker_pred.Xtrace)->setBackground(Qt::white);
                    window->table->item(window->tracer_marker_pred.Ytrace,   window->tracer_marker_pred.Xtrace+window->tracer_marker_pred.j)->setBackground(Qt::white);
                    window->table->item(window->tracer_marker_pred.Ytrace+window->tracer_marker_pred.k, window->tracer_marker_pred.Xtrace)->setBackground(Qt::white);
                    window->table->item(window->tracer_marker_pred.Ytrace+window->tracer_marker_pred.k, window->tracer_marker_pred.Xtrace+window->tracer_marker_pred.j)->setBackground(Qt::white);
                }
            }
            //сохраняем  текущее положение для след расчета
            window->tracer_marker_pred = window->tracer_marker;
            //         рисуем новое положение маркера
            window->table->item(window->tracer_marker.Ytrace, window->tracer_marker.Xtrace)->setBackground(color_leftUP);//левый верхний
            window->table->item(window->tracer_marker.Ytrace, window->tracer_marker.Xtrace+window->tracer_marker.j)->setBackground(color_rightUP);//правый верхний
            window->table->item(window->tracer_marker.Ytrace+window->tracer_marker.k, window->tracer_marker.Xtrace)->setBackground(color_leftDOWN);//левый нижний
            window->table->item(window->tracer_marker.Ytrace+window->tracer_marker.k, window->tracer_marker.Xtrace+window->tracer_marker.j)->setBackground(color_rightDOWN);//правый нижний

            // разблокируем обновления редакции
            window->table->blockSignals(false);//
        }
    }
    ui->trace_time_label->setText(QString::number(t.nsecsElapsed()/1000) + "us");
    //qDebug() << time.msecsTo( QTime::currentTime() );
    timer->start();
}

void MainWindow::on_BaudRatelineEdit_textChanged(const QString &arg1)   // Обновляем скорость обмена
{
    DMA.baudRate = arg1.toUInt() ;
}

void MainWindow::on_StartButton_clicked()
{
    QString s;
    if (ui->StartButton->text() == "Start")
    {
        QString romID = DMA.connect();

        if ( romID.isEmpty() )
        {
            ui->listWidget->addItem("connect failure");
            ui->StartButton->setDown(false);
            return;
        }
        ui->listWidget->addItem(s);
        ui->StartButton->setDown(true);
        ui->StartButton->setText("Stop");
        ui->RAM_reset_Button->setDisabled(!Enumerator.VechicleInterfaceState);
        ui->read_RAM_Button->setDisabled(!Enumerator.VechicleInterfaceState);
        s = "romID " + romID;
        ui->listWidget->addItem(s);
        CreateTable(SearchFiles(CurrDir + "/xml/", romID)); //найдем файл конфига и парсим его
        timer->start(1000/ui->logger_rate_textedit->text().toUInt());
        s =   "CurrDir " + CurrDir;
    }
    else
    {
        ui->StartButton->setText("Start");
        ui->StartButton->setDown(false);
        TableDelete();
    }
}

void MainWindow::on_RAM_reset_Button_clicked()
{
    DMA.timer_lock();
    DMA.MUT_Out_buffer[0] = 0x00;
    DMA.MUT_Out_buffer[1] = 0x00;

    DMA.write_direct(_ecu->DEAD_var, 2);
    on_read_RAM_Button_clicked();
    DMA.timer_unlock();
}

void MainWindow::on_read_RAM_Button_clicked()
{
    DMA.timer_lock();
    //for(int i=0; i < list_window->count(); i++)

    foreach(mapWidget *window, list_window)
    {
        //DynamicWindow *tablewindow = qobject_cast<DynamicWindow*>(list_window[i]);
        //DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>(list_widget[i]);
        window->table->blockSignals(true);//перед обновлением отключим сигнал автообновления ячейки
        //window->table_set_update();
        window->table->blockSignals(false);
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
    //emit Enumerator. InterfaceActive(20);
    //QString romID = DMA.connect();
    for (int i =0; i < 4000; i++)
    {
        DMA.rx_msg[0].Data[i] = i;
        DMA.MUT_Out_buffer[i] = i;
    }
    //SearchFiles(CurrDir + "/xml/", "80700010");   //найдем файл конфига
    CreateTable(SearchFiles(CurrDir + "/xml/", "90550001"));   	//найдем файл конфига							//парсим его
    hexEdit->setData(QByteArray::fromRawData((char*)DMA.MUT_Out_buffer, 4000));
    //CreateTable(SearchFiles(CurrDir + "/xml/", "88592715"));
    //    if ( "90550001" != load_bin(SearchFiles(CurrDir + "/bin/", "90550001")) )
    //         qDebug() << "bin mismatch";

    timer->start(1000/ui->logger_rate_textedit->text().toUInt());
}

void MainWindow::on_loadbinbutton_clicked()
{
    QString CurrDir = QApplication::applicationDirPath()+ "\\"  ;   //текущая директория
    QString bin_filename = QFileDialog::getOpenFileName(this, tr("Open .bin"), CurrDir, tr("bin files (*.bin)"));
    QFile binfile(bin_filename);
    if (!binfile.open(QIODevice::ReadWrite ))
        return;

    binarray = new QByteArray( binfile.read(binfile.size()));     // новый массив

    //uint romIDnum = qFromBigEndian<quint32>(binarray->mid(0xf52, 4)); // номер калибровки
    //QString romID =  QString::number( romIDnum, 16 );

    //SearchFiles(CurrDir, romID);   //найдем файл конфига
    //QString xml_filename = listFiles[0];
    //if (xml_filename == "error")
    //    xml_filename = QFileDialog::getOpenFileName(this, tr("Open xml"), CurrDir, tr("xml files (*.xml)"));
    // else
    //     xml_filename =  CurrDir + xml_filename;
    //  ReadConfig(xml_filename);   								//парсим его

    qDebug() << "";

}

void MainWindow::on_stop_live_clicked()
{
    TableDelete();
    debug = false;
}

void MainWindow::on_save_trace_pushButton_clicked()
{
    save_trace = !save_trace;
    ui->save_trace_pushButton->setDown(save_trace);
}

void MainWindow::Log(QString str)
{
    ui->listWidget->addItem(str);
}

void MainWindow::on_inno_initButton_clicked()
{
    DMA.init_inno();
}

void MainWindow::on_start_addr_lineEdit_returnPressed()
{
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint64 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    DMA.read_direct(addr, count);
    QByteArray b = QByteArray::fromRawData( (char*)DMA.MUT_Out_buffer, count );
    hexEdit->setData(b);
    hexEdit->setAddressOffset(addr);

}

void MainWindow::on_count_lineEdit_returnPressed()
{
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint64 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    DMA.read_direct(addr, count);
    QByteArray b = QByteArray::fromRawData( (char*)DMA.MUT_Out_buffer, count );
    hexEdit->setData(b);
    hexEdit->setAddressOffset(addr);
}
