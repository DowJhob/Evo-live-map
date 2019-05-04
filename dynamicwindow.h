#ifndef DYNAMICWINDOW_H
#define DYNAMICWINDOW_H
#include <QtWidgets>

#include <QTableWidget>
#include <QtEndian>
//#include <math.h>
#include "xmldomparser.h"
#include "DMA.h"
//#include "dynamictablewidget.h"
//#include "mathparser2.h"

struct Tracer_marker
{
    int a;                   //координаты трейсера
    int b;
    int kb;
    int ka;
};

class DynamicWindow: public QWidget
{
    Q_OBJECT

public:


    TableProperty_fr_xml *Table_Decl;               //Описание таблицы

    Tracer_marker tracer_marker_X = {};
    Tracer_marker tracer_marker_pred_X = {};
    Tracer_marker tracer_marker_Y = {};
    Tracer_marker tracer_marker_pred_Y = {};

    QVector <int> x_axis={};    //костыли с содержимым осей
    QVector <int> y_axis={};

    QTableWidget *table;

    float x = 0;
    float y = 0;
    dma *DMA={};
    //mathParser2 *math;

    DynamicWindow(QWidget *parent = nullptr, TableProperty_fr_xml *Table_Decl = nullptr, dma *DMA = nullptr): QWidget(parent, Qt::Window)
    {
        this->Table_Decl = Table_Decl;
table = new QTableWidget(Table_Decl->Y_axis.elements, Table_Decl->X_axis.elements, parent);
        table->setProperty("addr", QVariant::fromValue(this) );  //сохраним адрес на окно что бы получить доступ к остальным членам по событиям в виджете

        this->DMA = DMA;

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
        table->setSortingEnabled(false);
        table->setUpdatesEnabled(true);
        //==============================================================================================================================================================
        float variable_value;
        //читаем таблицу заголовка-оси в буфер
        DMA->read_by_type(
                    Table_Decl->X_axis.scaling.storagetype,          //тип данных оси
                    Table_Decl->X_axis.rom_addr,                     //адрес оси в ром
                    Table_Decl->X_axis.elements,
                    1);                                              //по игреку ось икс равна 1
        //заполняем в соотвествии с формулой
        for (uint i = 0; i < Table_Decl->X_axis.elements; i++)
        {
            variable_value = typed(Table_Decl->X_axis.scaling.storagetype, DMA->MUT_In_buffer, i, Table_Decl->X_axis.scaling.endian); //кастуем данные к определенному типу
            QTableWidgetItem *item = new QTableWidgetItem();
            float compute = fast_calc(Table_Decl->X_axis.scaling.toexpr2, variable_value);
            x_axis.append(qRound(compute));
            item->setData( Qt::DisplayRole, QString::number(qRound(compute)));
            table->setHorizontalHeaderItem(i, item);
        }
        //----------------------------------------------------------------------------------------------
        //читаем таблицу заголовка-оси в буфер
        DMA->read_by_type(
                    Table_Decl->Y_axis.scaling.storagetype, //тип данных оси
                    Table_Decl->Y_axis.rom_addr,                //адрес оси в ром
                    1,
                    Table_Decl->Y_axis.elements);

        for (uint i = 0; i < Table_Decl->Y_axis.elements; i++)
        {
            variable_value = typed(Table_Decl->Y_axis.scaling.storagetype, DMA->MUT_In_buffer, i, Table_Decl->Y_axis.scaling.endian); //кастуем данные к определенному типу
            QTableWidgetItem *item = new QTableWidgetItem();

            float compute = fast_calc(Table_Decl->Y_axis.scaling.toexpr2, variable_value);

            y_axis.append(qRound(compute));
            item->setData( Qt::DisplayRole, QString::number(qRound(compute)));
            table->setVerticalHeaderItem(i, item);
        }
        //заполним таблицу что бы два раза не бегать
        table_set_update();   //создаем обновляем таблицу
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
    void table_set_update()
    {
        emit timer_lock();
        // прочитаем нужное количество данных в соответствии с типом
        DMA->read_by_type(
                    Table_Decl->Table.scaling.storagetype,
                    Table_Decl->Table.ram_addr,
                    Table_Decl->X_axis.elements,
                    Table_Decl->Y_axis.elements);
        //QString inverse_polish_note = math->get_notation_convert(tablewidget->Table_Decl.Table.scaling.toexpr);
        Table_Decl->Table.scaling.toexpr2 = //*math->
                *set_notation(Table_Decl->Table.scaling.toexpr);   //set notation вернула адрес на свою внутренню структуру с бинарной нотацией


        long long variable_value;
        uint c = 0;
        for (int x = 0; x < Table_Decl->X_axis.elements; x++)
        {
            for (int y = 0; y < Table_Decl->Y_axis.elements; y++)
            {
                variable_value = typed(Table_Decl->Table.scaling.storagetype,
                                             DMA->MUT_In_buffer,
                                             c,
                                             Table_Decl->Table.scaling.endian); //кастуем данные к определенному типу
                //создаем обновляем итем
                float compute;
                compute = fast_calc(Table_Decl->Table.scaling.toexpr2, variable_value);
                if ( Table_Decl->Table.swapxy )
                {
                    if (table->item(y, x) == nullptr)  //если итема нет создадим
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        table->setItem(y, x, item);
                    }

                    table->item(y, x)->setData( Qt::DisplayRole, compute);
                }
                else
                {
                    if (table->item(x, y) == nullptr)  //если итема нет создадим
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        table->setItem(x, y, item);
                    }
                    table->item(x, y)->setData( Qt::DisplayRole, compute);
                }
                c++;
            }
        }
        emit timer_unlock();
    }

    void cr_item(QTableWidget *tablewidget, QString storagetype, uchar *in_buf, uint c, bool big, fast_calc_struct toexpr2, bool swapxy)
    {
           float variable_value = typed(storagetype, in_buf, c, big); //кастуем данные к определенному типу
           //создаем обновляем итем
           float compute = fast_calc(toexpr2, variable_value);
           if ( swapxy )
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


       }

    float read_and_cast(bool ram_scaling_storagetype, QString storagetype, quint32 mut_number, bool scaling_endian, fast_calc_struct scaling_frexpr2 )
    {
        float x = 0;
        if (!ram_scaling_storagetype)
        {
            x = typed(storagetype,
                      DMA->MUT_In_buffer,
                      mut_number,                //номер запроса рам мут
                      scaling_endian);
            x = fast_calc(scaling_frexpr2, x);
        }
        return x;
    }

    qint64 typed(QString storagetype, uchar *in_buf, uint c, bool big)//кастуем данные к определенному типу
    {
        if ( storagetype == "int8")
        {

            return ((qint8)(in_buf)[c]);

            //    return (qint8)(*((qint8*) (f + c)));
        }
        if ( storagetype == "int16")
        {
            if (big)
                return qFromBigEndian<qint16>(in_buf + 2 * c);
            else
                return qFromLittleEndian<qint16>(in_buf + 2 * c);
        }
        if ( storagetype == "int32")
        {
            if (big)
                return qFromBigEndian<qint32>(in_buf + 4 * c);
            else
                return qFromLittleEndian<qint32>(in_buf + 4 * c);
        }
        if ( storagetype == "uint8")
        {
            return ((quint8)(in_buf)[c]);
            //  return (quint8)(*((quint8*) (f + c)));
        }
        if ( storagetype == "uint16")
        {
            if (big)
                return qFromBigEndian<quint16>(in_buf + 2 * c);
            else
                return qFromLittleEndian<quint16>(in_buf + 2 * c);
        }
        if ( storagetype == "uint32")
        {
            if (big)
                return qFromBigEndian<quint32>(in_buf + 4 * c);
            else
                return qFromLittleEndian<quint32>(in_buf + 4 * c);
        }
        else
            return ((quint8)(in_buf)[c]);
    }

signals:
    void timer_lock();
    void timer_unlock();
private:
public slots:

    void on_tableWidget_cellChanged(uint row, uint column)     //обработчик обновление редакции в таблице
    {
        /* Определяем объект, который вызвал сигнал */
        //DynamicTableWidget *tablewidget = (DynamicTableWidget*)sender();
        QTableWidget *tablewidget = qobject_cast<QTableWidget*>( sender() );

        DynamicWindow *window = qvariant_cast<DynamicWindow*>( tablewidget->property("addr") ); // указатель на окно


        emit timer_lock();
        uint pos;
        if (window->Table_Decl->Table.swapxy)
        {
            pos = column * window->Table_Decl->Y_axis.elements + row;
        }
        else
        {
            pos = row * window->Table_Decl->X_axis.elements + column;
        }
        double variable_value = tablewidget->item(row, column)->text().toDouble();
        DMA->MUT_Out_buffer[0] = qRound(Computing(get_notation_convert(window->Table_Decl->Table.scaling.frexpr ), variable_value));
        DMA->write_direct(window->Table_Decl->Table.ram_addr + pos, 1);
        emit timer_unlock();
    }

};
#endif // DYNAMICWINDOW_H
