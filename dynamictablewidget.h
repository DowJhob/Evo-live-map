#ifndef QDYNAMICTABLEWIDGET_H
#define QDYNAMICTABLEWIDGET_H
#include "xmldomparser.h"


#include <qtablewidget.h>
//#include "mut_transport.h"
#include "mathparser2.h"
struct Tracer_marker
{
    int a;                   //координаты трейсера
    int b;
    int kb;
    int ka;
};
class DynamicTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    DynamicTableWidget(QWidget *parent = nullptr, TableProperty_fr_xml *Table_Decl = nullptr)
        :  QTableWidget(Table_Decl->Y_axis.elements, Table_Decl->X_axis.elements, parent)
    {
        this->Table_Decl = *Table_Decl;     //все в таблицу

    }
    ~DynamicTableWidget()
    {
    }

    TableProperty_fr_xml Table_Decl;               //Описание таблицы в самом объекте таблицы

    Tracer_marker tracer_marker_X = {};
    Tracer_marker tracer_marker_pred_X = {};
    Tracer_marker tracer_marker_Y = {};
    Tracer_marker tracer_marker_pred_Y = {};

    QVector <int> x_axis;    //костыли с содержимым осей
    QVector <int> y_axis;

private slots:


signals:
  //  void cellChanged();

};

#endif // QDYNAMICTABLEWIDGET_H
