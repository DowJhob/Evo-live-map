#ifndef DYNAMICWINDOW_H
#define DYNAMICWINDOW_H
#include <QtWidgets>

#include <custom_tablewidget.h>
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
    QVector <qint64> x_axis={};    //костыли с содержимым осей
    QVector <qint64> y_axis={};
    CustomTableWidget *table;
    float x = 0;
    float y = 0;
    QByteArray updateData;
    //mathParser2 *math;

    mapWidget(QWidget *parent = nullptr, tableDeclaration *Table_Decl = nullptr): QWidget(parent, Qt::Window)
    {
        this->Table_Decl =  *Table_Decl;
        qDebug() << "====================== Create table : " << this->Table_Decl.Table.Name << " ================================";

        //создаем таблицу с заданной размерностью
        table = new CustomTableWidget(this->Table_Decl.Y_axis.elements, this->Table_Decl.X_axis.elements, parent);

        table->setProperty("addr", QVariant::fromValue(this) );  //сохраним адрес на окно что бы получить доступ к остальным членам по событиям в виджете
        QGridLayout *layout = new QGridLayout(this);  //лайот с родителем виджетом

        table->setSortingEnabled(false);
        table->setUpdatesEnabled(true);

        setWindowTitle(this->Table_Decl.Table.Name);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setLayout(layout);
        layout->addWidget(table);

        table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout->setMargin(1);
        layout->setContentsMargins(1, 1, 1, 1);
 //       show();
    }
protected:

signals:

private:

public slots:
    void create(QVector <qint64>* map)
    {
        for (int i = 0; i < this->Table_Decl.X_axis.elements; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData( Qt::DisplayRole, QString::number(x_axis.at(i)));
            table->setHorizontalHeaderItem(i, item);
        }
        //----------------------------------------------------------------------------------------------
        for (int i = 0; i < this->Table_Decl.Y_axis.elements; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData( Qt::DisplayRole, QString::number(y_axis.at(i)));
            table->setVerticalHeaderItem(i, item);
        }
        //заполним таблицу что бы два раза не бегать
        table_set_update(map);   //создаем обновляем таблицу
        //----------------------------------
        QSize Size( 45, 25 );
        table->resizeRowsToContents();
        table->resizeColumnsToContents();
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
        table->resize(Size);setFixedSize(Size);
    }
    void table_set_update(QVector <qint64>* map)
    {
        long long variable_value;
        uint c = 0;
        int swapXyLen, swapYxLen;
        if ( this->Table_Decl.Table.swapxy )
        {
            swapXyLen = this->Table_Decl.X_axis.elements;
            swapYxLen = this->Table_Decl.Y_axis.elements;
        }
        else
        {
            swapYxLen = this->Table_Decl.X_axis.elements;
            swapXyLen = this->Table_Decl.Y_axis.elements;
        }
        int swapxyX, swapxyY;
        for (int x = 0; x < swapXyLen; x++)
        {
            for (int y = 0; y < swapYxLen; y++)
            {
                //создаем обновляем итем
                if ( this->Table_Decl.Table.swapxy ) {swapxyX = x; swapxyY = y;}
                else {swapxyX = y; swapxyY = x;}
                if (table->item(swapxyY, swapxyX) == nullptr)  //если итема нет создадим
                    table->setItem(swapxyY, swapxyX, new QTableWidgetItem());
                table->item(swapxyY, swapxyX)->setData( Qt::DisplayRole, map->at(c));
                c++;
            }
        }
    }
};
#endif // DYNAMICWINDOW_H
