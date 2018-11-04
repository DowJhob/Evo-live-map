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

        connect(table, &DynamicTableWidget::cellChanged, this, on_tableWidget_cellChanged, Qt::DirectConnection); //

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
                if ( tablewidget->Table_Decl.Table.swapxy )
                {
                    if (tablewidget->item(y, x) == nullptr)  //если итема нет создадим
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        tablewidget->setItem(y, x, item);
                    }


                    compute = math->fast_calc(tablewidget->Table_Decl.Table.scaling.toexpr2, variable_value);


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

private:
signals:
    void timer_lock();
    void timer_unlock();

public slots:
    void table_show_hide()
    {
        /* Определяем объект, который вызвал сигнал*/
        QPushButton *tableButton = (QPushButton*)sender();
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
        DynamicTableWidget *tablewidget = (DynamicTableWidget*)sender();
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
};

#endif // DYNAMICWINDOW_H
