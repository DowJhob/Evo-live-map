#ifndef CUSTOMTABLEWIIDGET_H
#define CUSTOMTABLEWIIDGET_H

#include <QTableWidget>
#include <QObject>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QKeyEvent>


#include "ecu.h"

struct Tracer_marker
{
    int Xtrace;                   //координаты трейсера в индексах таблицы
    int Ytrace;
    int k;                         //добавочные индексы для краев диапазона, их нужно сохранять на случай резкого измения сигнала от края до нуля
    int j;
    QColor predColorA;
    QColor predColorB;
    QColor predColorC;
    QColor predColorD;
};

class CustomTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    tableDeclaration Table_Decl;               //Описание таблицы
    Tracer_marker tracer_marker = {};
    Tracer_marker tracer_marker_pred = {};
    QVector <float> x_axis={};    //костыли с содержимым осей
    QVector <float> y_axis={};
    CustomTableWidget(int row, int column, QWidget *parent = nullptr//, tableDeclaration *Table_Decl = nullptr
                      ): QTableWidget(parent)
    {
        setRowCount(row);
        setColumnCount(column);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        //setSortingEnabled(false);
        setUpdatesEnabled(true);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setContentsMargins(0, 0, 0, 0);
    }
    void create(QVector <float>* map)
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
        //заполним таблицу
        table_set_update(map, 200, 200);   //создаем обновляем таблицу
        //----------------------------------
        QSize Size( 30, 10 );

        //setHorizontalHeaderLabels("fghshshf");
        resizeRowsToContents();
        resizeColumnsToContents();
        //--------------------------------------расчет размеров таблицы-------------------------------------------------
        for( int i = 0; i <  Table_Decl.X_axis.elements; i++)                        //|
        {                                                                       //|
            Size.setWidth( Size.width() + columnWidth( i));              //|накапливаем ширину столбцов
        }                                                                       //|
        //|
        for( int i = 0; i < Table_Decl.Y_axis.elements; i++)                           //|
        {                                                                       //|
            Size.setHeight( Size.height() + rowHeight( i));              //| накапливаем высоту строк
        }
        Size.setWidth( Size.width() + verticalHeader()->width() );
        Size.setHeight( Size.height() + horizontalHeader()->height());
        //--------------------------------------------
        setFixedSize(Size);
    }

    void table_set_update(QVector <float>* map, int saturation, int alpha)
    {
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
        QColor color;
        int britghtnes = 200;
        int minDesireHUE = 0;
        int maxDesireHUE = 240;
        int desire_hue;

        // normalize input
        float inRange = Table_Decl.Table.rom_scaling.max - Table_Decl.Table.rom_scaling.min;
        int kHue = maxDesireHUE - minDesireHUE;

        for (int x = 0; x < swapXyLen; x++)
        {
            for (int y = 0; y < swapYxLen; y++)
            {
                //создаем обновляем итем
                if ( this->Table_Decl.Table.swapxy ) {swapxyX = x; swapxyY = y;}
                else {swapxyX = y; swapxyY = x;}
                if (item(swapxyY, swapxyX) == nullptr)  //если итема нет создадим
                    setItem(swapxyY, swapxyX, new QTableWidgetItem());

                // normalize input
                float kColor = fabs(  (map->at(c) - Table_Decl.Table.rom_scaling.min) / inRange);
                britghtnes = 200;
                if ( map->at(c) > Table_Decl.Table.rom_scaling.max)
                {
                    britghtnes = 80;
                    kColor = 1;
                }
                else if ( map->at(c) < Table_Decl.Table.rom_scaling.min )
                {
                    britghtnes = 80;
                    kColor = 0;
                }

                {
                    // normalize hue
                    desire_hue = qRound(kHue * kColor);
                    desire_hue += minDesireHUE;
                }
                color.setHsv( desire_hue , saturation, britghtnes, alpha);
                item(swapxyY, swapxyX)->setBackground(color);
                item(swapxyY, swapxyX)->setData( Qt::DisplayRole, map->at(c));
                c++;
            }
        }
        //init marker
        tracer_marker_pred.predColorA = item(0, 0)->background().color();
        if ( Table_Decl.X_axis.elements > 1 )
            tracer_marker_pred.predColorB = item(0, 1)->background().color();
        if ( Table_Decl.Y_axis.elements > 1 )
            tracer_marker_pred.predColorC = item(1, 0)->background().color();
        if ( Table_Decl.Y_axis.elements > 1 && Table_Decl.X_axis.elements > 1 )
            tracer_marker_pred.predColorD = item(1, 1)->background().color();
        tracer_marker.Xtrace = 1;
        tracer_marker.Ytrace = 1;
        tracer_marker_pred.Xtrace = 1;
        tracer_marker_pred.Ytrace = 1;
    }

protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if( event==QKeySequence::ZoomIn || event->matches(QKeySequence::MoveToNextPage) )
            slotIncrease();
        else if( event->matches( QKeySequence::ZoomOut) || event->matches(QKeySequence::MoveToPreviousPage) )
            slotDecrease();
        else if( event->matches( QKeySequence::Copy ) )
            slotCopy();
        else if( event->matches( QKeySequence::Paste ) )
            slotPaste();
    }
private:

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
        QItemSelectionRange range = selectionModel()->selection().first();
        for (auto i = range.top(); i <= range.bottom(); ++i)
            for (auto j = range.left(); j <= range.right(); ++j)
                model()->setData( model()->index(i,j), model()->index(i,j).data().toFloat() + Table_Decl.Table.rom_scaling.increment, Qt::DisplayRole );
    }
    void slotDecrease()
    {
        QItemSelectionRange range = selectionModel()->selection().first();
        for (auto i = range.top(); i <= range.bottom(); ++i)
            for (auto j = range.left(); j <= range.right(); ++j)
                model()->setData( model()->index(i,j), model()->index(i,j).data().toFloat() - Table_Decl.Table.rom_scaling.increment, Qt::DisplayRole );
    }

};

#endif // CUSTOMTABLEWIIDGET_H
