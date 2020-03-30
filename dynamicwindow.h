#ifndef DYNAMICWINDOW_H
#define DYNAMICWINDOW_H
#include <QtWidgets>

#include <QTableWidget>
#include <QtEndian>

#include "xmldomparser.h"
#include "DMA.h"

struct Tracer_marker
{
    int Xtrace;                   //координаты трейсера в индексах таблицы
    int Ytrace;
    int k;                         //добавочные индексы для краев диапазона, их нужно сохранять на случай резкого измения сигнала от края до нуля
    int j;
};

class mapWidget: public QWidget
{
    Q_OBJECT

public:
    tableDeclaration Table_Decl;               //Описание таблицы
    Tracer_marker tracer_marker;
    Tracer_marker tracer_marker_pred = {};
    QVector <int> x_axis={};    //костыли с содержимым осей
    QVector <int> y_axis={};
    QTableWidget *table;
    float x = 0;
    float y = 0;
    dma *DMA;
    //mathParser2 *math;

    mapWidget(QWidget *parent = nullptr, tableDeclaration *Table_Decl = nullptr, dma *DMA = nullptr): QWidget(parent, Qt::Window)
    {
        //this->Table_Decl = new tableDeclaration;
        this->Table_Decl =  *Table_Decl;
        this->DMA = DMA;
        table = new QTableWidget(this->Table_Decl.Y_axis.elements, this->Table_Decl.X_axis.elements, parent);
        table->setProperty("addr", QVariant::fromValue(this) );  //сохраним адрес на окно что бы получить доступ к остальным членам по событиям в виджете
        QGridLayout *layout = new QGridLayout(this);  //лайот с родителем виджетом
        //создаем таблицу с заданной размерностью
        table->setSortingEnabled(false);
        table->setUpdatesEnabled(true);
        //==============================================================================================================================================================
        float variable_value;
        //читаем таблицу заголовка-оси в буфер
        read_by_type(
            this->Table_Decl.X_axis.rom_scaling._storagetype,          //тип данных оси
            this->Table_Decl.X_axis.rom_addr,                     //адрес оси в ром
            this->Table_Decl.X_axis.elements,
            1);                                              //по игреку ось икс равна 1
        //заполняем в соотвествии с формулой
        for (int i = 0; i < this->Table_Decl.X_axis.elements; i++)
        {
            variable_value = mem_cast(this->Table_Decl.X_axis.rom_scaling, DMA->MUT_Out_buffer, i); //кастуем данные к определенному типу
            QTableWidgetItem *item = new QTableWidgetItem();
            int compute = qRound(fast_calc(this->Table_Decl.X_axis.rom_scaling.toexpr2, variable_value));
            x_axis.append(compute);
            item->setData( Qt::DisplayRole, QString::number(compute));
            table->setHorizontalHeaderItem(i, item);
        }
        //----------------------------------------------------------------------------------------------
        //читаем таблицу заголовка-оси в буфер
        read_by_type(
            this->Table_Decl.Y_axis.rom_scaling._storagetype, //тип данных оси
            this->Table_Decl.Y_axis.rom_addr,                //адрес оси в ром
            1,
            this->Table_Decl.Y_axis.elements);
        for (int i = 0; i < Table_Decl->Y_axis.elements; i++)
        {
            variable_value = mem_cast(this->Table_Decl.Y_axis.rom_scaling, DMA->MUT_Out_buffer, i); //кастуем данные к определенному типу
            QTableWidgetItem *item = new QTableWidgetItem();
            int compute = qRound(fast_calc(this->Table_Decl.Y_axis.rom_scaling.toexpr2, variable_value));
            y_axis.append(compute);
            item->setData( Qt::DisplayRole, QString::number(compute));
            table->setVerticalHeaderItem(i, item);
        }
        //заполним таблицу что бы два раза не бегать
        table_set_update();   //создаем обновляем таблицу
        connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableWidget_cellChanged(int, int)), Qt::DirectConnection);
        //----------------------------------
        QSize Size(45  , //ширина вертикальных линий?
                   25   //ширина горизонтальных линий? маджик числа
                   );
        table->resizeRowsToContents();
        table->resizeColumnsToContents();
        //table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        //--------------------------------------расчет размеров таблицы-------------------------------------------------
        for( int i = 0; i <  this->Table_Decl.X_axis.elements; i++)                        //|
        {                                                                       //|
            Size.setWidth( Size.width() + table->columnWidth( i));              //|накапливаем ширину столбцов
        }                                                                       //|
        Size.setWidth( Size.width() + table->verticalHeader()->width());        //|
        for( int i = 0; i < this->Table_Decl.Y_axis.elements; i++)                           //|
        {                                                                       //|
            Size.setHeight( Size.height() + table->rowHeight( i));              //| накапливаем высоту строк
        }                                                                       //|
        Size.setHeight( Size.height() + table->horizontalHeader()->height());   //|
        //--------------------------------------------
        setWindowTitle(this->Table_Decl.Table.Name);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setLayout(layout);
        layout->addWidget(table);
        table->resize(Size);
        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFixedSize(Size);
        layout->setMargin(10);
        layout->setContentsMargins(10, 10, 10, 10);
 //       show();
    }

    void table_set_update()
    {
        emit timer_lock();
        // прочитаем нужное количество данных в соответствии с типом
        read_by_type(
            Table_Decl.Table.rom_scaling._storagetype,
            Table_Decl.Table.ram_addr,
            Table_Decl.X_axis.elements,
            Table_Decl.Y_axis.elements);
        long long variable_value;
        uint c = 0;
        int swapXyLen, swapYxLen;
        if ( Table_Decl.Table.swapxy )
        {
            swapXyLen = Table_Decl.X_axis.elements;
            swapYxLen = Table_Decl.Y_axis.elements;
        }
        else
        {
            swapYxLen = Table_Decl.X_axis.elements;
            swapXyLen = Table_Decl.Y_axis.elements;
        }
        int swapxyX, swapxyY;
        for (int x = 0; x < swapXyLen; x++)
        {
            for (int y = 0; y < swapYxLen; y++)
            {
                variable_value = mem_cast(Table_Decl.Table.rom_scaling, DMA->MUT_Out_buffer, c ); //кастуем данные к определенному типу
                //создаем обновляем итем
                float compute = fast_calc(Table_Decl.Table.rom_scaling.toexpr2, variable_value);
                if ( Table_Decl.Table.swapxy ) {swapxyX = x; swapxyY = y;}
                else {swapxyX = y; swapxyY = x;}
                if (table->item(swapxyY, swapxyX) == nullptr)  //если итема нет создадим
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    table->setItem(swapxyY, swapxyX, item);
                }
                table->item(swapxyY, swapxyX)->setData( Qt::DisplayRole, compute);
                c++;
            }
        }
        emit timer_unlock();
    }

    float mut_cast( Scaling scaling, int mut_number )
    {
        float x = type_cast(scaling, DMA->MUT_Out_buffer + mut_number );
        x = fast_calc(scaling.toexpr2, x);
        return x;
    }

    qint64 mem_cast(Scaling scaling, uchar *in_buf, uint offset)   //кастуем данные к определенному типу
    {
        switch (scaling._storagetype) {
        case Storagetype::int8:
        case Storagetype::uint8:  type_cast(scaling, in_buf + offset); break;
        case Storagetype::int16:
        case Storagetype::uint16: type_cast(scaling, in_buf + 2*offset); break;
        case Storagetype::int32:
        case Storagetype::uint32: type_cast(scaling, in_buf + 4*offset); break;
        default: break;
        }
    }

    qint64 type_cast(Scaling scaling, uchar *in_buf)          //кастуем данные к определенному типу
    {
        if (scaling.endian)
            switch (scaling._storagetype) {
            case Storagetype::int8:   return (qint8)in_buf[0];                break;
            case Storagetype::int16:  return qFromBigEndian<qint16>(in_buf);  break;
            case Storagetype::int32:  return qFromBigEndian<qint32>(in_buf);  break;
            case Storagetype::uint8:  return (quint8)in_buf[0];  break;
            case Storagetype::uint16: return qFromBigEndian<quint16>(in_buf); break;
            case Storagetype::uint32: return qFromBigEndian<quint32>(in_buf);break;
            default: break;
            }
        else
            switch (scaling._storagetype) {
            case Storagetype::int8:   return (qint8)in_buf[0];           break;
            case Storagetype::int16:  return qFromLittleEndian<qint16>(in_buf); break;
            case Storagetype::int32:  return qFromLittleEndian<qint32>(in_buf); break;
            case Storagetype::uint8:  return (quint8)in_buf[0];      break;
            case Storagetype::uint16: return qFromLittleEndian<quint16>(in_buf); break;
            case Storagetype::uint32: return qFromLittleEndian<quint32>(in_buf); break;
            default: break;
            }
    }

signals:
    void timer_lock();
    void timer_unlock();
private:
    void read_by_type(Storagetype storagetype, quint32 mem_addr, int x, int y)
    {
        int lenght = x * y;
        // прочитаем нужное количество данных в соответствии с типом
        switch (storagetype) {
        case Storagetype::int8:
        case Storagetype::uint8:  DMA->read_direct( mem_addr, lenght); break;
        case Storagetype::int16:
        case Storagetype::uint16: DMA->read_direct( mem_addr, lenght * 2); break;
        case Storagetype::int32:
        case Storagetype::uint32: DMA->read_direct( mem_addr, lenght * 4); break;
        default: break;
        }
    }
public slots:

    void on_tableWidget_cellChanged(int row, int column)     //обработчик обновление редакции в таблице
    {
        QTableWidget *tablewidget = qobject_cast<QTableWidget*>( sender() );
        mapWidget *window = qvariant_cast<mapWidget*>( tablewidget->property("addr") ); // указатель на окно
        emit timer_lock();
        uint pos;
        if (window->Table_Decl.Table.swapxy)
        {
            pos = column * window->Table_Decl.Y_axis.elements + row;
        }
        else
        {
            pos = row * window->Table_Decl.X_axis.elements + column;
        }
        double variable_value = tablewidget->item(row, column)->text().toDouble();
        DMA->MUT_Out_buffer[0] = qRound(Computing(get_notation_convert(window->Table_Decl.Table.rom_scaling.frexpr ), variable_value));
        DMA->write_direct(window->Table_Decl.Table.ram_addr + pos, 1);
        emit timer_unlock();
    }


};
#endif // DYNAMICWINDOW_H
