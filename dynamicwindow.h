#ifndef DYNAMICWINDOW_H
#define DYNAMICWINDOW_H
#include <QtWidgets>

#include <qtablewidget.h>
#include <QtEndian>
//#include <math.h>
//#include "xmldomparser.h"
#include "DMA.h"
#include "dynamictablewidget.h"
#include "mathparser2.h"


class Code: public QObject
{
    Q_OBJECT

public:
    Code(  )
    {

    }
public slots:
    void on_tableWidget_cellChanged(int row, int column)     //обработчик обновление редакции в таблице
    {
//        /* Определяем объект, который вызвал сигнал */
//        //DynamicTableWidget *tablewidget = (DynamicTableWidget*)sender();
//        DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>( sender() );
//        emit timer_lock();
//        int pos;
//        if (tablewidget->Table_Decl.Table.swapxy)
//        {
//            pos = column * tablewidget->Table_Decl.Y_axis.elements + row;
//        }
//        else
//        {
//            pos = row * tablewidget->Table_Decl.X_axis.elements + column;
//        }
//        double variable_value = tablewidget->item(row, column)->text().toDouble();
//        DMA->MUT_Out_buffer[0] = math->Computing(math->get_notation_convert(tablewidget->Table_Decl.Table.scaling.frexpr ), variable_value);
//        DMA->write_direct(tablewidget->Table_Decl.Table.ram_addr + pos, 1);
//        emit timer_unlock();
    }

    void logger_and_tableWidget_trace()
    {

//        bool debug = true;

//        emit timer_lock();
//        // DMA.read_indirect(xmlParser->RAM_MUT_addr, 16);// логгинг что бы контроллер не уснул
//        /* Ищем объект, в списке*/
//        float x = 0, f = 300;
//        float y = 0, g = 4000;
//        //QObject* obj = sender();

//        foreach (QObject *o, DMA->win_manager.list_widget)
//        {
//            DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>( o );
//            tablewidget->blockSignals( true );

//            // читаем из буфера и кастуем

//            if (!debug)
//            {

//                x = read_and_cast(tablewidget->Table_Decl.X_axis.ram_scaling.storagetype.isEmpty(),
//                                  tablewidget->Table_Decl.X_axis.ram_scaling.storagetype,
//                                  tablewidget->Table_Decl.X_axis.ram_scaling.ram_mut_number,
//                                  tablewidget->Table_Decl.X_axis.scaling.endian,
//                                  tablewidget->Table_Decl.X_axis.scaling.frexpr2);


//                y = read_and_cast(tablewidget->Table_Decl.Y_axis.ram_scaling.storagetype.isEmpty(),
//                                  tablewidget->Table_Decl.Y_axis.ram_scaling.storagetype,
//                                  tablewidget->Table_Decl.Y_axis.ram_scaling.ram_mut_number,
//                                  tablewidget->Table_Decl.Y_axis.scaling.endian,
//                                  tablewidget->Table_Decl.Y_axis.scaling.frexpr2);

//            }
//            else
//            {
//                if (tablewidget->tracer_marker_X.a == 0)
//                {
//                    x = f;
//                    y = g;
//                }
//                else
//                {
//                    x = 0;
//                    y = 0;
//                }
//            }

//            //----------------------- вычисляем координаты маркера------------------------------------------------
//            //tablewidget->tracer_marker_X =
//            axis_lookup(qRound(x),
//                        tablewidget->Table_Decl.X_axis.elements,
//                        tablewidget->x_axis,
//                        &tablewidget->tracer_marker_X);
//            //tablewidget->tracer_marker_Y =
//            axis_lookup(qRound(y),
//                        tablewidget->Table_Decl.Y_axis.elements,
//                        tablewidget->y_axis,
//                        &tablewidget->tracer_marker_Y);
//            //----------------------- вычисляем насыщенность ячеек маркера хидера---------------------------------------
//            //----------------------- ось X

//            int ax_X_len = tablewidget->x_axis[tablewidget->tracer_marker_X.b] - tablewidget->x_axis[tablewidget->tracer_marker_X.a];
//            int ax_Y_len = tablewidget->y_axis[tablewidget->tracer_marker_Y.b] - tablewidget->y_axis[tablewidget->tracer_marker_Y.a];

//            int EndXvect = (x - tablewidget->x_axis[tablewidget->tracer_marker_X.a] - ax_X_len/2) * 255 / ax_X_len;  //нормированные координаты концов всех векторов
//            int EndYvect = (tablewidget->y_axis[tablewidget->tracer_marker_Y.b] - y - ax_Y_len/2) * 255 / ax_Y_len;  //нормированные координаты концов всех векторов




//            int left_vectX = EndXvect + 178; // EndXvect - (-89)
//            int right_vectX = EndXvect - 178;
//            int up_vectY = EndYvect - 178;
//            int down_vectY = EndYvect + 178;

//            //модули векторов,
//            int leftUP = modul(left_vectX, up_vectY);
//            int rightUP  = modul(right_vectX, up_vectY);
//            int leftDOWN   = modul(left_vectX, down_vectY);
//            int rightDOWN    = modul(right_vectX, down_vectY);

//            QColor color_rightDOWN, color_leftDOWN, color_rightUP, color_leftUP;
//            color_rightDOWN.setHsv(240, rightDOWN, 255, 255);
//            color_leftDOWN.setHsv(240, leftDOWN, 255, 255);
//            color_rightUP.setHsv(240, rightUP, 255, 255);
//            color_leftUP.setHsv(240, leftUP, 255, 255);



//            //------------------------------------------------------------------------------------------------------------------
//            if ((tablewidget->tracer_marker_pred_X.a != tablewidget->tracer_marker_X.a)
//                    ||(tablewidget->tracer_marker_pred_Y.a != tablewidget->tracer_marker_Y.a)) // тут гашение если изменился X или Y
//            {
//                //гашение предыдущих маркеров на хидерах
//                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.a)->setForeground(Qt::black);
//                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.b)->setForeground(Qt::black);
//                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.a)->setForeground(Qt::black);
//                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.b)->setForeground(Qt::black);
//                //гашение предыдущего маркера таблицы
//                tablewidget->item(tablewidget->tracer_marker_pred_Y.a, tablewidget->tracer_marker_pred_X.a)->setBackground(Qt::white);
//                tablewidget->item(tablewidget->tracer_marker_pred_Y.a, tablewidget->tracer_marker_pred_X.b)->setBackground(Qt::white);
//                tablewidget->item(tablewidget->tracer_marker_pred_Y.b, tablewidget->tracer_marker_pred_X.a)->setBackground(Qt::white);
//                tablewidget->item(tablewidget->tracer_marker_pred_Y.b, tablewidget->tracer_marker_pred_X.b)->setBackground(Qt::white);
//            }

//            //сохраняем  текущее положение для след расчета
//            tablewidget->tracer_marker_pred_X = tablewidget->tracer_marker_X;
//            tablewidget->tracer_marker_pred_Y = tablewidget->tracer_marker_Y;

//            //         рисуем новое положение маркеров на хидерах
//            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.a)->setForeground(color_leftUP);
//            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.b)->setForeground(color_rightUP);
//            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.a)->setForeground(color_leftUP);
//            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.b)->setForeground(color_leftDOWN);

//            //         рисуем новое положение маркера
//            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.a)->setBackground(color_leftUP);//левый верхний
//            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.b)->setBackground(color_rightUP);//правый верхний

//            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.a)->setBackground(color_leftDOWN);//левый нижний
//            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.b)->setBackground(color_rightDOWN);//правый нижний
//            //
//            // разблокируем обновления редакции
//            tablewidget->blockSignals(false);//

//            QTime start = QTime::currentTime();
//            tablewidget->repaint(1,1,1,1);
//            // qDebug() <<  " time " << start.msecsTo( QTime::currentTime() );

//        }

//        emit timer_unlock();

    }


};


class DynamicWindow: public QWidget
{
    Q_OBJECT

public:

    dma *DMA;
    mathParser2 *math;
    int Tag;
    DynamicWindow(QWidget *parent = nullptr, TableProperty_fr_xml *Table_Decl = nullptr, dma *DMA = nullptr, mathParser2 *math = nullptr): QWidget(parent, Qt::Window)
    {
        this->math = math;
        this->DMA = DMA;
        this->math = math;
        Tag = Table_Decl->tableNum;
        //генерируем окно

        //тут проверим таблицу на размерность
        if (Table_Decl->X_axis.elements == 0)
        {
            Table_Decl->X_axis.elements++;
            Table_Decl->X_axis.scaling.toexpr = "x + 0";
            Table_Decl->Table.swapxy = true;
        }
        if (Table_Decl->Y_axis.elements == 0)
        {
            Table_Decl->Y_axis.elements++;
            Table_Decl->Y_axis.scaling.toexpr = "x + 0";
            Table_Decl->Table.swapxy = true;
        }
        QGridLayout *layout = new QGridLayout(this);  //лайот с родителем виджетом

        //создаем таблицу с заданной размерностью
        DynamicTableWidget *table = new DynamicTableWidget( this, Table_Decl);  //создаем динамическую таблицу
        table->setSortingEnabled(false);
        table->setUpdatesEnabled(true);
        DMA->win_manager.list_widget.insert(Table_Decl->tableNum, table);

        //==============================================================================================================================================================

        // QString inverse_polish_note = math->get_notation_convert(Table_Decl->X_axis.scaling.toexpr);

        // Table_Decl->X_axis.scaling.toexpr2 = *math->set_notation(Table_Decl->X_axis.scaling.toexpr);   //set notation вернула адрес на свою внутренню структуру с бинарной нотацией

        float variable_value;
        //читаем таблицу заголовка-оси в буфер
        DMA->read_by_type(
                    Table_Decl->X_axis.scaling.storagetype,          //тип данных оси
                    Table_Decl->X_axis.rom_addr,                     //адрес оси в ром
                    //mut_trans,
                    Table_Decl->X_axis.elements,
                    1);                                              //по игреку ось икс равна 1
        //заполняем в соотвествии с формулой
        for (uint i = 0; i < Table_Decl->X_axis.elements; i++)
        {
            variable_value = math->typed(Table_Decl->X_axis.scaling.storagetype, DMA->MUT_In_buffer, i, Table_Decl->X_axis.scaling.endian); //кастуем данные к определенному типу

            QTableWidgetItem *item = new QTableWidgetItem();
            //    float compute = math->Computing(inverse_polish_note, variable_value);


            float compute = math->fast_calc(Table_Decl->X_axis.scaling.toexpr2, variable_value);

            table->x_axis.append(compute);


            item->setData( Qt::DisplayRole, QString::number(compute));
            table->setHorizontalHeaderItem(i, item);
        }
        //----------------------------------------------------------------------------------------------
        //  inverse_polish_note = math->get_notation_convert(Table_Decl->Y_axis.scaling.toexpr);
        //Table_Decl->Y_axis.scaling.toexpr2 = *math->set_notation(Table_Decl->Y_axis.scaling.toexpr);   //set notation вернула адрес на свою внутренню структуру с бинарной нотацией


        //читаем таблицу заголовка-оси в буфер
        DMA->read_by_type(
                    Table_Decl->Y_axis.scaling.storagetype, //тип данных оси
                    Table_Decl->Y_axis.rom_addr,                //адрес оси в ром
                    //mut_trans,
                    1,
                    Table_Decl->Y_axis.elements);

        for (uint i = 0; i < Table_Decl->Y_axis.elements; i++)
        {
            variable_value = math->typed(Table_Decl->Y_axis.scaling.storagetype, DMA->MUT_In_buffer, i, Table_Decl->Y_axis.scaling.endian); //кастуем данные к определенному типу
            QTableWidgetItem *item = new QTableWidgetItem();
            //double compute = math->Computing(inverse_polish_note, variable_value);
            float compute = math->fast_calc(Table_Decl->Y_axis.scaling.toexpr2, variable_value);


            table->y_axis.append(compute);
            item->setData( Qt::DisplayRole, QString::number(compute));
            table->setVerticalHeaderItem(i, item);
        }

        //=--------------------------------------------------------------------------

        //заполним таблицу что бы два раза не бегать
        table_set_update(table);   //создаем обновляем таблицу

        //connect(table, &DynamicTableWidget::cellChanged, this, on_tableWidget_cellChanged, Qt::DirectConnection); //
        connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)), Qt::DirectConnection);
        //----------------------------------
        table->resizeRowsToContents();
        table->resizeColumnsToContents(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Bingo!!!!

        int line_with = 1;


        table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QSize Size(line_with *  2  , //ширина вертикальных линий?
                   line_with *  2   //ширина горизонтальных линий? маджик числа
                   );
        show();

        //--------------------------------------расчет размеров таблицы-------------------------------------------------
        for( uint i = 0; i <  Table_Decl->X_axis.elements; i++)                        //|
        {                                                                //|
            Size.setWidth( Size.width() + table->columnWidth( i));             //|накапливаем ширину столбцов
        }   ;                                                                  //|
        Size.setWidth( Size.width() + table->verticalHeader()->width());      //|
        for( uint i = 0; i < Table_Decl->Y_axis.elements; i++)                           //|
        {                                                                     //|
            Size.setHeight( Size.height() + table->rowHeight( i));              //| накапливаем высоту строк
        };                                                                     //|
        Size.setHeight( Size.height() + table->horizontalHeader()->height()); //|
        //--------------------------------------------
        table->setFixedSize(Size);
        layout->setMargin(0);
        setLayout(layout);
        layout->addWidget(table);

        setWindowTitle(Table_Decl->Table.Name);

        setFixedSize(Size);




    }
    void table_set_update(DynamicTableWidget *tablewidget)
    {
        emit timer_lock();
        // прочитаем нужное количество данных в соответствии с типом
        DMA->read_by_type(
                    tablewidget->Table_Decl.Table.scaling.storagetype,
                    tablewidget->Table_Decl.Table.ram_addr,
                    tablewidget->Table_Decl.X_axis.elements,
                    tablewidget->Table_Decl.Y_axis.elements);
        //QString inverse_polish_note = math->get_notation_convert(tablewidget->Table_Decl.Table.scaling.toexpr);
        tablewidget->Table_Decl.Table.scaling.toexpr2 = *math->set_notation(tablewidget->Table_Decl.Table.scaling.toexpr);   //set notation вернула адрес на свою внутренню структуру с бинарной нотацией


        long long variable_value;
        uint c = 0;
        for (int x = 0; x < tablewidget->Table_Decl.X_axis.elements; x++)
        {
            for (int y = 0; y < tablewidget->Table_Decl.Y_axis.elements; y++)
            {
                variable_value = math->typed(tablewidget->Table_Decl.Table.scaling.storagetype,
                                             DMA->MUT_In_buffer,
                                             c,
                                             tablewidget->Table_Decl.Table.scaling.endian); //кастуем данные к определенному типу
                //создаем обновляем итем
                float compute;
                compute = math->fast_calc(tablewidget->Table_Decl.Table.scaling.toexpr2, variable_value);
                if ( tablewidget->Table_Decl.Table.swapxy )
                {
                    if (tablewidget->item(y, x) == nullptr)  //если итема нет создадим
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        tablewidget->setItem(y, x, item);
                    }

                    tablewidget->item(y, x)->setData( Qt::DisplayRole, compute);
                }
                else
                {
                    if (tablewidget->item(x, y) == nullptr)  //если итема нет создадим
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        tablewidget->setItem(x, y, item);
                    }
                    tablewidget->item(x, y)->setData( Qt::DisplayRole, compute);
                }
                c++;
            }
        }
        emit timer_unlock();
    }


signals:
    void timer_lock();
    void timer_unlock();
private:
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
    float read_and_cast(bool ram_scaling_storagetype, QString storagetype, quint32 mut_number, bool scaling_endian, fast_calc_struct scaling_frexpr2 )
    {
        float x = 0;
        if (!ram_scaling_storagetype)
        {
            x = math->typed(storagetype,
                            DMA->MUT_In_buffer,
                            mut_number,                //номер запроса рам мут
                            scaling_endian);


            x = math->fast_calc(scaling_frexpr2, x);
        }
        return x;
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
public slots:
    void table_show_hide()
    {
        /* Определяем объект, который вызвал сигнал*/
        QPushButton *tableButton = qobject_cast<QPushButton*>( sender() );
        /* Ищем объект, в списке*/
        foreach (QObject *o, DMA->win_manager.list_window)
        {
            DynamicWindow *tablewindow = qobject_cast<DynamicWindow*>(o);
            if (tableButton->property("tag") ==  tablewindow->Tag)
                tablewindow->setVisible( !tablewindow->isVisible());
        }
    }
    void on_tableWidget_cellChanged(int row, int column)     //обработчик обновление редакции в таблице
    {
        /* Определяем объект, который вызвал сигнал */
        //DynamicTableWidget *tablewidget = (DynamicTableWidget*)sender();
        DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>( sender() );
        emit timer_lock();
        int pos;
        if (tablewidget->Table_Decl.Table.swapxy)
        {
            pos = column * tablewidget->Table_Decl.Y_axis.elements + row;
        }
        else
        {
            pos = row * tablewidget->Table_Decl.X_axis.elements + column;
        }
        double variable_value = tablewidget->item(row, column)->text().toDouble();
        DMA->MUT_Out_buffer[0] = math->Computing(math->get_notation_convert(tablewidget->Table_Decl.Table.scaling.frexpr ), variable_value);
        DMA->write_direct(tablewidget->Table_Decl.Table.ram_addr + pos, 1);
        emit timer_unlock();
    }

    void logger_and_tableWidget_trace()
    {

        bool debug = true;

        emit timer_lock();
        // DMA.read_indirect(xmlParser->RAM_MUT_addr, 16);// логгинг что бы контроллер не уснул
        /* Ищем объект, в списке*/
        float x = 0, f = 300;
        float y = 0, g = 4000;
        //QObject* obj = sender();

        foreach (QObject *o, DMA->win_manager.list_widget)
        {
            DynamicTableWidget *tablewidget = qobject_cast<DynamicTableWidget*>( o );
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
                if (tablewidget->tracer_marker_X.a == 0)
                {
                    x = f;
                    y = g;
                }
                else
                {
                    x = 0;
                    y = 0;
                }
            }

            //----------------------- вычисляем координаты маркера------------------------------------------------
            //tablewidget->tracer_marker_X =
            axis_lookup(qRound(x),
                        tablewidget->Table_Decl.X_axis.elements,
                        tablewidget->x_axis,
                        &tablewidget->tracer_marker_X);
            //tablewidget->tracer_marker_Y =
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
                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.a)->setForeground(Qt::black);
                tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_pred_X.b)->setForeground(Qt::black);
                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.a)->setForeground(Qt::black);
                tablewidget->verticalHeaderItem(tablewidget->tracer_marker_pred_Y.b)->setForeground(Qt::black);
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
            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.a)->setForeground(color_leftUP);
            tablewidget->horizontalHeaderItem(tablewidget->tracer_marker_X.b)->setForeground(color_rightUP);
            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.a)->setForeground(color_leftUP);
            tablewidget->verticalHeaderItem(tablewidget->tracer_marker_Y.b)->setForeground(color_leftDOWN);

            //         рисуем новое положение маркера
            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.a)->setBackground(color_leftUP);//левый верхний
            tablewidget->item(tablewidget->tracer_marker_Y.a, tablewidget->tracer_marker_X.b)->setBackground(color_rightUP);//правый верхний

            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.a)->setBackground(color_leftDOWN);//левый нижний
            tablewidget->item(tablewidget->tracer_marker_Y.b, tablewidget->tracer_marker_X.b)->setBackground(color_rightDOWN);//правый нижний
            //
            // разблокируем обновления редакции
            tablewidget->blockSignals(false);//

            QTime start = QTime::currentTime();
            tablewidget->repaint(1,1,1,1);
            // qDebug() <<  " time " << start.msecsTo( QTime::currentTime() );

        }

        emit timer_unlock();

    }
};
#endif // DYNAMICWINDOW_H
