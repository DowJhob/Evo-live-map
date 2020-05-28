#include <QtCore>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <dbt.h>
#include <QtGlobal>
#include <QMessageBox>


//#include "common/ecutools.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CurrDir = QApplication::applicationDirPath();   //текущая директории
    //=============================================================================
    connect(&Enumerator, SIGNAL(InterfaceActive(int)), SLOT(dll_connect(int)));
    connect(&Enumerator, SIGNAL(disconnectInterface()), SLOT(dll_disconnect()));
    connect(&Enumerator, SIGNAL(Log(QString)), statusBar(), SLOT(showMessage(QString)));

    //=============================================================================
    hexEdit = new QHexEdit;
    hexEdit->setAddressWidth(8);
    hexEdit->setAddressOffset(ui->start_addr_lineEdit->text().toUInt(nullptr, 16));
    ui->RAMeditorLayout->addWidget(hexEdit, 3,0,1,2);
    //=============================================================================
//    QHBoxLayout *layout = new QHBoxLayout(ui->toolBar);
//    ui->toolBar->setLayout(layout);

    start_action = ui->toolBar->addAction( QIcon( ":ico/connect.png" ), "Start", this, SLOT(StartButton_slot()));
    ram_reset = ui->toolBar->addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", this, SLOT(RAM_reset_slot()));

    ui->toolBar->addSeparator();
    QWidget* empty = new QWidget(this);
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->toolBar->addWidget(empty);
    debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, SLOT(debugButton_slot()));


    //Подписываемся на события нет нужды в подписке WM_change broadcast!
    Enumerator.NotifyRegister((HWND)this->winId());
    Enumerator.enumerateUSB_Device_by_guid();

    interfaceLock();

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemChecks(QTreeWidgetItem*, int)));
}
MainWindow::~MainWindow()
{
    dll_disconnect();
    TableDelete();
    delete ui;
    //delete timer;
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
    gauge_widget *gauge_lcd = new gauge_widget(name, 4, param->ram_mut_offset, &param->ram_mut_param_scaling, ui->toolBar);
    ui->toolBar->addWidget(gauge_lcd);
    gauge_set.insert(gauge_lcd);
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
bool MainWindow::ReadConfig(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Unable to open file"), file->errorString());
        return false;
    }

    xmlParser._parser(file, _ecu, ui->treeWidget);        // парсим файл
    delete file;
    return true;
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
    switch (sub_tab->rom_scaling._storagetype) {                                //тип данных оси
    case Storagetype::int8:
    case Storagetype::uint8:  ecu_comm->sendDMAcomand(0xE1, addr, sub_tab->elements); break;
    case Storagetype::int16:
    case Storagetype::uint16: ecu_comm->sendDMAcomand(0xE1, addr, sub_tab->elements * 2); break;
    case Storagetype::int32:
    case Storagetype::uint32: ecu_comm->sendDMAcomand(0xE1, addr, sub_tab->elements * 4); break;
    default: break;
    }
    ecu_comm->read();
    //заполняем в соотвествии с формулой
    for (int i = 0; i < sub_tab->elements; i++)
    {
        variable_value = _ecu->mem_cast(&sub_tab->rom_scaling, (uchar*)ecu_comm->in_buff, i); //кастуем данные к определенному типу
        axis->append(fast_calc(sub_tab->rom_scaling.toexpr2, variable_value));
    }
}
bool MainWindow::getECU(QString filename)
{
    _ecu = new ecu;
    if (!ReadConfig(filename))
        return false;
    // переберем все описания таблиц
    foreach ( tableDeclaration tab, _ecu->RAMtables)
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
    foreach (gauge_widget *gauge, gauge_set)
        gauge->deleteLater();
    gauge_set.clear();
    foreach (QWidget *w, widget_set)
        w->deleteLater();
    widget_set.clear();
}
void MainWindow::logger_and_tableWidget_trace(QByteArray in)
{
    QElapsedTimer t;
    t.start();
    //    timer->stop();
    //    ecu_comm->sendDMAcomand(0xE0, _ecu->RAM_MUT_addr, 16);
    //    ecu_comm->read();
    foreach (gauge_widget *gauge, gauge_set)
    {

        gauge->display( QString::number(_ecu->mut_cast((uchar*)in.data(), gauge->scaling, gauge->offset)) );
    }

    float x = 0;
    float y = 0;
    foreach (CustomTableWidget *table, ptrRAMtables)
    {
        if ( table->Table_Decl.X_axis.ram_mut_number >= 0 || table->Table_Decl.Y_axis.ram_mut_number >= 0 )
        {
            x = _ecu->mut_cast((uchar*)in.data(), &table->Table_Decl.X_axis.RAM_MUT_scaling, table->Table_Decl.X_axis.ram_mut_number);
            y = _ecu->mut_cast((uchar*)in.data(), &table->Table_Decl.Y_axis.RAM_MUT_scaling, table->Table_Decl.Y_axis.ram_mut_number);
            if (debug)
            {
                x = QCursor::pos().x()*2.3;
                y = QCursor::pos().y()*10.1;
            }
            //----------------------- вычисляем координаты маркера------------------------------------------------
            table->tracer_marker.Xtrace = axis_lookup2(x, table->Table_Decl.X_axis.elements, table->x_axis);
            table->tracer_marker.Ytrace = axis_lookup2(y, table->Table_Decl.Y_axis.elements, table->y_axis);
            //----------------------- вычисляем насыщенность ячеек маркера ---------------------------------------
            table->tracer_marker.k = 0;
            table->tracer_marker.j = 0;
            if ( (table->Table_Decl.X_axis.elements > 1) && (table->tracer_marker.Xtrace < table->Table_Decl.X_axis.elements - 1) )
                table->tracer_marker.j = 1;
            if ( (table->Table_Decl.Y_axis.elements > 1) && (table->tracer_marker.Ytrace < table->Table_Decl.Y_axis.elements - 1) )
                table->tracer_marker.k = 1;

            float kX = (float)255/(table->x_axis[table->tracer_marker.Xtrace + table->tracer_marker.j] - table->x_axis[table->tracer_marker.Xtrace]);    //коэффициент нормирования
            float kY = (float)255/(table->y_axis[table->tracer_marker.Ytrace + table->tracer_marker.k] - table->y_axis[table->tracer_marker.Ytrace]);    //коэффициент нормирования

            int leftNormalX  = qRound((table->x_axis[table->tracer_marker.Xtrace]-x)*kX);                       //нормированные координаты
            int rightNormalX = qRound((table->x_axis[table->tracer_marker.Xtrace + table->tracer_marker.j]-x)*kX);                       //нормированные координаты
            int upNormalY    = qRound((table->y_axis[table->tracer_marker.Ytrace]-y)*kY);
            int downNormalY  = qRound((table->y_axis[table->tracer_marker.Ytrace + table->tracer_marker.k]-y)*kY);

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
            table->blockSignals( true );
            if ((table->tracer_marker_pred.Xtrace != table->tracer_marker.Xtrace) ||(table->tracer_marker_pred.Ytrace != table->tracer_marker.Ytrace)) // тут гашение если изменился X или Y
            {
                //гашение предыдущего маркера таблицы
                if (!save_trace)
                {
                    table->item(table->tracer_marker_pred.Ytrace,  table->tracer_marker_pred.Xtrace)->setBackground(table->tracer_marker_pred.predColorA);
                    table->item(table->tracer_marker_pred.Ytrace,  table->tracer_marker_pred.Xtrace + table->tracer_marker_pred.j)->setBackground(table->tracer_marker_pred.predColorB);
                    table->item(table->tracer_marker_pred.Ytrace + table->tracer_marker_pred.k, table->tracer_marker_pred.Xtrace)->setBackground(table->tracer_marker_pred.predColorC);
                    table->item(table->tracer_marker_pred.Ytrace + table->tracer_marker_pred.k, table->tracer_marker_pred.Xtrace + table->tracer_marker_pred.j)->setBackground(table->tracer_marker_pred.predColorD);
                }


                //сохраняем  текущее положение для след расчета
                table->tracer_marker_pred = table->tracer_marker;
                table->tracer_marker_pred.predColorA = table->item(table->tracer_marker.Ytrace,                          table->tracer_marker.Xtrace                         )->background().color();
                table->tracer_marker_pred.predColorB = table->item(table->tracer_marker.Ytrace,                          table->tracer_marker.Xtrace + table->tracer_marker.j)->background().color();
                table->tracer_marker_pred.predColorC = table->item(table->tracer_marker.Ytrace + table->tracer_marker.k, table->tracer_marker.Xtrace                         )->background().color();
                table->tracer_marker_pred.predColorD = table->item(table->tracer_marker.Ytrace + table->tracer_marker.k, table->tracer_marker.Xtrace + table->tracer_marker.j)->background().color();
            }
            //         рисуем новое положение маркера
            table->item(table->tracer_marker.Ytrace,  table->tracer_marker.Xtrace)->setBackground(color_leftUP);//левый верхний
            table->item(table->tracer_marker.Ytrace,  table->tracer_marker.Xtrace + table->tracer_marker.j)->setBackground(color_rightUP);//правый верхний
            table->item(table->tracer_marker.Ytrace + table->tracer_marker.k, table->tracer_marker.Xtrace)->setBackground(color_leftDOWN);//левый нижний
            table->item(table->tracer_marker.Ytrace + table->tracer_marker.k, table->tracer_marker.Xtrace + table->tracer_marker.j)->setBackground(color_rightDOWN);//правый нижний


            // разблокируем обновления редакции
            table->blockSignals(false);//
        }
    }
    ui->trace_time_label->setText(QString::number(t.nsecsElapsed()/1000) + "us");

    //    timer->start();
}

void MainWindow::dll_connect(int VechicleInterfaceType)                //по сигналу перечислителя
{
    if (ecu_comm == nullptr  )
    {
        if (VechicleInterfaceType == 13  )
            ecu_comm = new OP13(Enumerator.DllLibraryPath);
        if (VechicleInterfaceType == 20  )
            ecu_comm = new OP20(Enumerator.DllLibraryPath);

        connect(this, SIGNAL(startLogger(quint32, quint16)), ecu_comm, SLOT(startLogger(quint32, quint16)));
        connect(this, &MainWindow::stopLogger, ecu_comm, &ECU_interface::stopLogger);
        connect(this, SIGNAL(setLoggingInterval(int)), ecu_comm, SLOT(setLoggingInterval(int)));
connect(ecu_comm, &ECU_interface::interfaceReady, this, &MainWindow::interfaceUnlock);
        connect(ecu_comm, SIGNAL(readyRead(QByteArray)), this, SLOT(logger_and_tableWidget_trace(QByteArray)));
        connect(ecu_comm, SIGNAL(Log(QString)), this, SLOT(Log(QString)));
        //=============================================================================
        connect(&interface_thread, &QThread::started, ecu_comm, &ECU_interface::init);
        ecu_comm->moveToThread(&interface_thread);
        interface_thread.start();
        //=============================================================================//=============================================================================
        if ( Enumerator.isTactrix )
        {
            if( tactrix_afr_lcd == nullptr )
            {
                tactrix_afr_lcd = new gauge_widget("tactrixAFR", 4, 0, nullptr, ui->toolBar);
                ui->toolBar->addWidget(tactrix_afr_lcd);
                connect(ecu_comm, SIGNAL(AFR(QString)), tactrix_afr_lcd, SLOT(display(QString)));
            }
            connect(this, &MainWindow::start_inno, ecu_comm, &ECU_interface::start_tactrix_inno);
            emit start_inno();
            //ecu_comm->start_tactrix_inno();
        }
    }
}

void MainWindow::dll_disconnect()
{
    if (ecu_comm != nullptr)
    {
        interfaceLock();
        ecu_comm->deleteLater();
        interface_thread.quit();
        interface_thread.wait(1000);
        ecu_comm = nullptr;
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

    foreach( QObject *w, ui->toolBar->children())
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
        if (!ecu_comm->e7_connect())
            return ;
        ;
        quint32 *calID = reinterpret_cast<quint32*>(ecu_comm->in_buff);
        *calID = 0;                                                     //занулим 4 ре байта
        ecu_comm->sendDMAcomand(0xE1, 0xF52, 4);                        //читаем номер калибровки
        ecu_comm->read();
        if ( *calID == 0 )
        {
            ui->listWidget->addItem("ECU connect failure");
            return;
        }
        QString romID = QString::number( qFromBigEndian<quint32>(ecu_comm->in_buff), 16 );
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
        ecu_comm->disconnect();
        TableDelete();
        ui->treeWidget->clear();
        delete _ecu;
        start_action->setText("Start");
    }
}
void MainWindow::debugButton_slot()
{
    debug = true;

    emit Enumerator. InterfaceActive(20);
    ecu_comm->e7_connect();
    //if (!ecu_comm->five_baud_init())
    ;//return ;
    ecu_comm->sendDMAcomand(0xE1, 0xF52, 4); //читаем номер калибровки
    ecu_comm->read();
    QString romID = QString::number( qFromBigEndian<quint32>(ecu_comm->in_buff), 16 );

    for (int i =0; i < 4000; i++)
    {
        ecu_comm->in_buff[i] = i;
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
    ecu_comm->sendDMAcomand(0xE2, _ecu->DEAD_var, 2, buf);
    QVector <float> map;

    foreach (CustomTableWidget *t, table_set)
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
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint64 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    ecu_comm->sendDMAcomand(0xE1, addr, count);
    ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)ecu_comm->in_buff, count));
    hexEdit->setAddressOffset(addr);

}
void MainWindow::on_count_lineEdit_returnPressed()
{
    int count = ui->count_lineEdit->text().toUInt(nullptr);
    quint64 addr = ui->start_addr_lineEdit->text().toUInt(nullptr, 16);

    ecu_comm->sendDMAcomand(0xE1, addr, count);
    ecu_comm->read();
    //
    hexEdit->setData(QByteArray::fromRawData( (char*)ecu_comm->in_buff, count));
    hexEdit->setAddressOffset(addr);
}
