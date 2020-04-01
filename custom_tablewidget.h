#ifndef CUSTOMTABLEWIIDGET_H
#define CUSTOMTABLEWIIDGET_H

#include <QTableWidget>
#include <QObject>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>


#include "ecu.h"

struct Tracer_marker
{
    int Xtrace;                   //координаты трейсера в индексах таблицы
    int Ytrace;
    int k;                         //добавочные индексы для краев диапазона, их нужно сохранять на случай резкого измения сигнала от края до нуля
    int j;
};

class CustomTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    tableDeclaration Table_Decl;               //Описание таблицы
    Tracer_marker tracer_marker;
    Tracer_marker tracer_marker_pred = {};
    QVector <qint64> x_axis={};    //костыли с содержимым осей
    QVector <qint64> y_axis={};
    CustomTableWidget(int row, int column, QWidget *parent = nullptr//, tableDeclaration *Table_Decl = nullptr
                      ): QTableWidget(parent)
    {
        setRowCount(row);
        setColumnCount(column);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        setSortingEnabled(false);
        setUpdatesEnabled(true);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
    void create(QVector <qint64>* map)
    {
        for (int i = 0; i < Table_Decl.X_axis.elements; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData( Qt::DisplayRole, QString::number(x_axis.at(i)));
            setHorizontalHeaderItem(i, item);
        }
        //----------------------------------------------------------------------------------------------
        for (int i = 0; i < Table_Decl.Y_axis.elements; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData( Qt::DisplayRole, QString::number(y_axis.at(i)));
            setVerticalHeaderItem(i, item);
        }
        //заполним таблицу что бы два раза не бегать
        table_set_update(map);   //создаем обновляем таблицу
        //----------------------------------
        QSize Size( 45, 25 );
        resizeRowsToContents();
        resizeColumnsToContents();
        //--------------------------------------расчет размеров таблицы-------------------------------------------------
        for( int i = 0; i <  Table_Decl.X_axis.elements; i++)                        //|
        {                                                                       //|
            Size.setWidth( Size.width() + columnWidth( i));              //|накапливаем ширину столбцов
        }                                                                       //|
        Size.setWidth( Size.width() + this->verticalHeader()->width() );        //|
        for( int i = 0; i < Table_Decl.Y_axis.elements; i++)                           //|
        {                                                                       //|
            Size.setHeight( Size.height() + rowHeight( i));              //| накапливаем высоту строк
        }                                                                       //|
        Size.setHeight( Size.height() + horizontalHeader()->height());   //|
        //--------------------------------------------
        resize(Size);
        setFixedSize(Size);
    }
protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if( event==QKeySequence::ZoomIn )
            slotIncrease();
        else if( event->matches( QKeySequence::ZoomOut ) )
            slotDecrease();
        else if( event->matches( QKeySequence::Copy ) )
            slotCopy();
        else if( event->matches( QKeySequence::Paste ) )
            slotPaste();
    }
private:

    void table_set_update(QVector <qint64>* map)
    {
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
                //создаем обновляем итем
                if ( this->Table_Decl.Table.swapxy ) {swapxyX = x; swapxyY = y;}
                else {swapxyX = y; swapxyY = x;}
                if (item(swapxyY, swapxyX) == nullptr)  //если итема нет создадим
                    setItem(swapxyY, swapxyX, new QTableWidgetItem());
                item(swapxyY, swapxyX)->setData( Qt::DisplayRole, map->at(c));
                c++;
            }
        }
    }

private slots:
    void slotCopy(){
        QString text;
        QItemSelectionRange range = selectionModel()->selection().first();
        for (auto i = range.top(); i <= range.bottom(); ++i)
        {
            QStringList rowContents;
            for (auto j = range.left(); j <= range.right(); ++j)
                rowContents << model()->index(i,j).data().toString();
            text += rowContents.join("\t");
            text += "\n";
        }
        QApplication::clipboard()->setText(text);
    }
    void slotPaste(){
        QString text = QApplication::clipboard()->text();
        QStringList rowContents = text.split("\n", QString::SkipEmptyParts);

        QModelIndex initIndex = selectionModel()->selectedIndexes().at(0);
        auto initRow = initIndex.row();
        auto initCol = initIndex.column();

        for (auto i = 0; i < rowContents.size(); ++i) {
            QStringList columnContents = rowContents.at(i).split("\t");
            for (auto j = 0; j < columnContents.size(); ++j) {
                model()->setData(model()->index(initRow + i, initCol + j), columnContents.at(j));
            }
        }
    }
    void slotIncrease()
    {

    }
    void slotDecrease()
    {

    }

};

#endif // CUSTOMTABLEWIIDGET_H
