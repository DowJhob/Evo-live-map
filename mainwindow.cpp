#include <QtCore>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QScrollArea>
#include <QtGlobal>
#include <QMessageBox>

//#include "common/ecutools.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //=============================================================================
    //    connect(&logger_thread, &QThread::started, ecu_comm, &ECU_interface::init);
    _logger.moveToThread(&logger_thread);
    logger_thread.start();


    ui->setupUi(this);
    CurrDir = QApplication::applicationDirPath();   //текущая директории
    //=============================================================================
    //=============================================================================
    hexEdit = new QHexEdit;
    hexEdit->setAddressWidth(8);
    hexEdit->setAddressOffset(ui->start_addr_lineEdit->text().toUInt(nullptr, 16));
    ui->RAMeditorLayout->addWidget(hexEdit, 3,0,1,2);
    //=============================================================================
    start_action = ui->toolBar->addAction( QIcon( ":ico/connect.png" ), "Start", this, SLOT(StartButton_slot()));
    ram_reset = ui->toolBar->addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", this, SLOT(RAM_reset_slot()));

    ui->toolBar->addSeparator();
    QWidget* empty = new QWidget(this);
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->toolBar->addWidget(empty);
    debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, SLOT(debugButton_slot()));

    interfaceLock();

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemChecks(QTreeWidgetItem*, int)));
}
MainWindow::~MainWindow()
{
    dll_disconnect();

    TableDelete();
    //_logger.deleteLater();
    logger_thread.quit();
    logger_thread.wait(1000);
    delete ui;
}

void MainWindow::create_tree(tableDeclaration *tab)
{
    if (tab->Table.ram_addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
    {
        QTreeWidgetItem* map_name_item = new QTreeWidgetItem(QStringList() << tab->Table.Name + " RAM address: " + QString::number(tab->Table.ram_addr, 16));
        map_name_item->setFlags(map_name_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        map_name_item->setCheckState(0, Qt::Unchecked);
        QTreeWidgetItem* axis_item;
        if (tab->X_axis.rom_addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->X_axis.Name + "  ROM address: " + QString::number(tab->X_axis.rom_addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        if (tab->Y_axis.rom_addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->Y_axis.Name + "  ROM address: " + QString::number(tab->Y_axis.rom_addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
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
        loggerWidgetBar->setMinimumSize(300, 100);
//        loggerWidgetBar->setAllowedAreas( Qt::BottomToolBarArea);

        //insertToolBar(ui->toolBar, loggerWidgetBar);
        addToolBar(Qt::BottomToolBarArea, loggerWidgetBar);

    }

    gauge_widget *_gauge_widget = new gauge_widget(name, 4, param->ram_mut_offset, &param->ram_mut_param_scaling, ui->tabWidget);

    //    ui->toolBar->addWidget(_graph_log_widget);


    //ui->logger_verticalLayout->layout()->addWidget(_gauge_widget);

    loggerWidgetBar->addWidget(_gauge_widget);

    gauge_widget_set.insert(_gauge_widget);
}
void MainWindow::create_table(tableDeclaration *tab)
{
    if (tab->Table.ram_addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
    {
        qDebug() << "====================== Create table : " << tab->Table.Name << " ================================";
        QWidget *mapWidget = new QWidget(this, Qt::Window | Qt::WindowCloseButtonHint);
        widget_set.insert(mapWidget);
        mapWidget->setWindowTitle(tab->Table.Name);
        //mapWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QGridLayout *layout = new QGridLayout(mapWidget);
        mapWidget->setLayout(layout);
        //создаем таблицу с заданной размерностью
        CustomTableWidget *table = new CustomTableWidget(tab->Y_axis.elements, tab->X_axis.elements, mapWidget);
        table_set.insert(table);
        layout->addWidget(table);
        mapWidget->setContentsMargins(1, 1, 1, 1);
        layout->setMargin(1);
        layout->setContentsMargins(1, 1, 1, 1);

        table->Table_Decl = *tab;
        axread(&tab->X_axis, &table->x_axis, true);   // читаем оси
        axread(&tab->Y_axis, &table->y_axis, true);   // читаем оси
        QVector <float> map;
        tab->Table.elements = tab->X_axis.elements * tab->Y_axis.elements;

        axread(&tab->Table, &map, false);

        table->create( &map);
        connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(updateRAM(int, int)));

        QSize Size = table->size();
        Size.setWidth( Size.width() + 20);
        Size.setHeight( Size.height() + 20);
        mapWidget->setFixedSize( Size );

        ptrRAMtables.insert(tab->Table.Name, table );
    }
}

void MainWindow::axread(sub_tableDeclaration *sub_tab, QVector <float> *axis, bool rom)
{
    float variable_value;
    quint32 addr;
    if ( rom )
        addr = sub_tab->rom_addr;
    else
        addr = sub_tab->ram_addr;
    //читаем таблицу заголовка-оси в буфер
    // прочитаем нужное количество данных в соответствии с типом

    vehicle_ecu_comm->sendDMAcomand(0xE1, addr, sub_tab->elements * _ecu->get_sizeData( &sub_tab->rom_scaling ) );
    vehicle_ecu_comm->read();
    //заполняем в соотвествии с формулой
    for (int i = 0; i < sub_tab->elements; i++)
    {
        variable_value = _ecu->mem_cast(&sub_tab->rom_scaling, (uchar*)vehicle_ecu_comm->in_buff, i); //кастуем данные к определенному типу
        axis->append(fast_calc(sub_tab->rom_scaling.toexpr2, variable_value));
    }
}
bool MainWindow::getECU(QString filename)
{
    _ecu = new ecu;

    if (!xmlParser.ReadECUdefinition(filename, _ecu))
        return false;

    _logger.setECU(_ecu);
    connect(&_logger, SIGNAL(log_data_ready()), this, SLOT(logger_and_tableWidget_trace2()));


    // переберем все описания таблиц
    //foreach ( tableDeclaration tab, _ecu->RAMtables)
    for ( tableDeclaration tab : _ecu->RAMtables)
    {
        create_table(&tab);
        create_tree(&tab);
    }

    QHash<QString, mutParam>::iterator i;
    for (i = _ecu->RAM_MUT.begin(); i != _ecu->RAM_MUT.end(); ++i)
        create_gauge(i.key(), &i.value());
    return true;
}
void MainWindow::TableDelete()
{
    //foreach (gauge_widget *gauge, gauge_widget_set)
    for (gauge_widget *gauge : gauge_widget_set)
        gauge->deleteLater();
    gauge_widget_set.clear();
    //foreach (QWidget *w, widget_set)
    for (QWidget *w : widget_set)
        w->deleteLater();
    widget_set.clear();
}

void MainWindow::logger_and_tableWidget_trace2()
{
    QElapsedTimer t;
    t.start();
    //    timer->stop();
    //    ecu_comm->sendDMAcomand(0xE0, _ecu->RAM_MUT_addr, 16);
    //    ecu_comm->read();
    //foreach (gauge_widget *_gauge_widget, gauge_widget_set)
    for (gauge_widget *_gauge_widget : gauge_widget_set)
    {
        _gauge_widget->display( _logger.log_param.value(_gauge_widget->offset) );
    }

    float x = 0;
    float y = 0;
    //foreach (CustomTableWidget *table, ptrRAMtables)
    for (CustomTableWidget *table : ptrRAMtables)
    {
        if ( table->Table_Decl.X_axis.ram_mut_number >= 0 || table->Table_Decl.Y_axis.ram_mut_number >= 0 )
        {
            x = _logger.log_param.value(table->Table_Decl.X_axis.ram_mut_number);
            y = _logger.log_param.value(table->Table_Decl.Y_axis.ram_mut_number);
            if (debug)
            {
                x = QCursor::pos().x()*2.3;
                y = QCursor::pos().y()*10.1;
            }
            table->tracer_calc(x, y);
        }
    }
    ui->trace_time_label->setText(QString::number(t.nsecsElapsed()/1000) + "us");
}

void MainWindow::dll_connect(int VechicleInterfaceType, TCHAR *DllLibraryPath, bool isTactrix)
//по сигналу перечислителя
{
    if (vehicle_ecu_comm == nullptr  )
    {
        if (VechicleInterfaceType == 13 )
            vehicle_ecu_comm = new OP13(DllLibraryPath);
        if (VechicleInterfaceType == 20 )
            vehicle_ecu_comm = new OP20(DllLibraryPath);

        connect(this, SIGNAL(startLogger(quint32, quint16)), vehicle_ecu_comm, SLOT(startLogger(quint32, quint16)));
        connect(this, &MainWindow::stopLogger, vehicle_ecu_comm, &ECU_interface::stopLogger);
        connect(this, &MainWindow::_delete, vehicle_ecu_comm, &ECU_interface::_delete);
        connect(this, SIGNAL(setLoggingInterval(int)), vehicle_ecu_comm, SLOT(setLoggingInterval(int)));
        connect(vehicle_ecu_comm, &ECU_interface::interfaceReady, this, &MainWindow::interfaceUnlock);

        //
        connect(vehicle_ecu_comm, SIGNAL(log_polling_tick(QByteArray)), &_logger, SLOT(poll_data_calc(QByteArray)));

        connect(vehicle_ecu_comm, SIGNAL(Log(QString)), this, SLOT(Log(QString)));
        //=============================================================================
        connect(&vehicle_ecu_interface_thread, &QThread::started, vehicle_ecu_comm, &ECU_interface::init);
        //connect(vehicle_ecu_comm, &QObject::destroyed, &vehicle_ecu_interface_thread, &QThread::quit);

        vehicle_ecu_comm->moveToThread(&vehicle_ecu_interface_thread);
        vehicle_ecu_interface_thread.start();
        //=============================================================================

        //=============================================================================
        if ( isTactrix )
        {
            if( tactrix_afr_lcd == nullptr )
            {
                tactrix_afr_lcd = new gauge_widget("tactrixAFR", 4, 0, nullptr, ui->toolBar);
                ui->toolBar->addWidget(tactrix_afr_lcd);
                connect(vehicle_ecu_comm, SIGNAL(AFR(float)), tactrix_afr_lcd, SLOT(display(float)));
            }
            connect(this, &MainWindow::start_WB, vehicle_ecu_comm, &ECU_interface::start_tactrix_wb);
        }
    }
}

void MainWindow::dll_disconnect()
{
    interfaceLock();
    if (vehicle_ecu_comm != nullptr)
    {
        emit _delete();
        //vehicle_ecu_comm->deleteLater();
        vehicle_ecu_interface_thread.quit();
        vehicle_ecu_interface_thread.wait(1000);
        vehicle_ecu_comm = nullptr;
    }
    if( tactrix_afr_lcd != nullptr )
    {
        tactrix_afr_lcd->deleteLater();
        tactrix_afr_lcd = nullptr;
    }
}

void MainWindow::interfaceThumbler(bool lockFlag)
{
    start_action->setDisabled(lockFlag);
    ram_reset->setDisabled(lockFlag);
    debug_action->setDisabled(lockFlag);

    //foreach( QObject *w, ui->toolBar->children())
    for( QObject *w: ui->toolBar->children())
        if (w->isWidgetType())
            reinterpret_cast<QWidget*>(w)->setDisabled(lockFlag) ;
}
void MainWindow::on_BaudRatelineEdit_textChanged(const QString &arg1)   // Обновляем скорость обмена
{
    //DMA.baudRate = arg1.toUInt() ;
}
void MainWindow::StartButton_slot()
{
    if (start_action->text() == "Start")
    {
        if (!vehicle_ecu_comm->e7_connect())
            return ;
        emit start_WB();
        quint32 *calID = reinterpret_cast<quint32*>(vehicle_ecu_comm->in_buff);
        *calID = 0;                                                     //занулим 4 ре байта
        vehicle_ecu_comm->sendDMAcomand(0xE1, 0xF52, 4);                        //читаем номер калибровки
        vehicle_ecu_comm->read();
        if ( *calID == 0 )
        {
            ui->listWidget->addItem("ECU connect failure");
            return;
        }
        QString romID = QString::number( qFromBigEndian<quint32>(vehicle_ecu_comm->in_buff), 16 );
        ui->listWidget->addItem("romID: " + romID);
        qDebug()<< "romID: " + romID;

        start_action->setChecked(true);

        start_action->setText("Stop");
        ram_reset->setDisabled(false);

        getECU(SearchFiles(CurrDir + "/xml/", romID)); //найдем файл конфига и парсим его

        //timer->start(1000/ui->logger_rate_textedit->text().toUInt());
        ui->listWidget->addItem("CurrDir: " + CurrDir);

        emit startLogger(_ecu->RAM_MUT_addr, 16);
    }
    else
    {
        //        timer->stop();
        emit stopLogger();
        vehicle_ecu_comm->disconnect();
        TableDelete();
        ui->treeWidget->clear();
        delete _ecu;
        start_action->setText("Start");
    }
}
void MainWindow::debugButton_slot()
{
    debug = true;

    //emit Enumerator. InterfaceActive(20);
    vehicle_ecu_comm->e7_connect();
    emit start_WB();
    //if (!ecu_comm->five_baud_init())
    ;//return ;
    vehicle_ecu_comm->sendDMAcomand(0xE1, 0xF52, 4); //читаем номер калибровки
    vehicle_ecu_comm->read();
    QString romID = QString::number( qFromBigEndian<quint32>(vehicle_ecu_comm->in_buff), 16 );

    for (int i =0; i < 4000; i++)
    {
        vehicle_ecu_comm->in_buff[i] = i;
    }
    //SearchFiles(CurrDir + "/xml/", "80700010");   //найдем файл конфига
    getECU(SearchFiles(CurrDir + "/xml/", "90550001"));   	//найдем файл конфига							//парсим его
    //hexEdit->setData(QByteArray::fromRawData((char*)DMA.MUT_Out_buffer, 4000));
    //CreateTable(SearchFiles(CurrDir + "/xml/", "88592715"));
    //    if ( "90550001" != load_bin(SearchFiles(CurrDir + "/bin/", "90550001")) )
    //         qDebug() << "bin mismatch";

    //timer->start(1000/ui->logger_rate_textedit->text().toUInt());

    ui->listWidget->addItem("romID 90550001");
    emit startLogger(_ecu->RAM_MUT_addr, 16);

}
void MainWindow::RAM_reset_slot()
{
    emit stopLogger();
    char buf[2];
    buf[0] = 0x00;
    buf[1] = 0x00;
    vehicle_ecu_comm->sendDMAcomand(0xE2, _ecu->DEAD_var, 2, buf);
    QVector <float> map;

    //foreach (CustomTableWidget *t, table_set)
    for (CustomTableWidget *t : table_set)
    {
        t->Table_Decl.Table.elements = t->Table_Decl.X_axis.elements * t->Table_Decl.Y_axis.elements;

        axread(&t->Table_Decl.Table, &map, false);
        t->table_set_update(&map, 200, 200);
        map.clear();
    }
    emit startLogger(_ecu->RAM_MUT_addr, 16);
}

void MainWindow::on_logger_rate_textedit_editingFinished()
{
    emit setLoggingInterval(1000/ui->logger_rate_textedit->text().toUInt() );
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

void MainWindow::on_save_trace_pushButton_clicked()
{
    save_trace = !save_trace;
    ui->save_trace_pushButton->setDown(save_trace);
}
void MainWindow::Log(QString str)
{
    ui->listWidget->addItem(str);
}
void MainWindow::on_start_addr_lineEdit_returnPressed()
{
    quint16 count = ui->count_lineEdit->text().toUInt(nullptr);
    quint32 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    vehicle_ecu_comm->sendDMAcomand(0xE1, addr, count);
    vehicle_ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)vehicle_ecu_comm->in_buff, count));
    hexEdit->setAddressOffset(addr);

}
void MainWindow::on_count_lineEdit_returnPressed()
{
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint32 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    vehicle_ecu_comm->sendDMAcomand(0xE1, addr, count);
    vehicle_ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)vehicle_ecu_comm->in_buff, count));
    hexEdit->setAddressOffset(addr);
}
