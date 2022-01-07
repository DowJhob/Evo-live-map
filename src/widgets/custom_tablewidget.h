#ifndef CUSTOMTABLEWIIDGET_H
#define CUSTOMTABLEWIIDGET_H

#include <QTableWidget>
#include <QObject>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QKeyEvent>

#include <QGridLayout>

#include "../ecu/ecu-definition.h"

struct Tracer_marker
{
    int X = 0;                   //координаты трейсера в индексах таблицы
    int Y = 0;
//    int k = 1;      //добавочные индексы для краев диапазона, их нужно сохранять на случай резкого измения сигнала от края до нуля
//    int j = 1;
    QColor color[4];
    QColor *leftUpColor = &color[0];
    QColor *rightUpColor = &color[1];
    QColor *leftDownColor = &color[2];
    QColor *rightDownColor = &color[3];
};

class CustomTableWidget: public QTableWidget
{
    Q_OBJECT

public:
    QVector<float> *scaledRAM_MUTvalue;
    Map *declaration;               //Описание таблицы
    Tracer_marker _tracer_marker;
    Tracer_marker current_marker = {};
    Tracer_marker pred_marker = {};
    QVector <float> x_axis={};    //костыли с содержимым осей
    QVector <float> y_axis={};

    CustomTableWidget(Map *declaration );

    void fillTable(QVector <float>* map);

    void traceUpdate();

public slots:
    void fillMap(QVector <float>* map);
//    void setItem(int row, int column, QTableWidgetItem *item)
//    {

//    }
protected:
    void keyPressEvent(QKeyEvent *event);

private:
    bool test = true;
    float x = 19;
    float y = 750;


    int saturation = 200;
    int alpha = 200;
    void fillHeaders();
    void saveTracer();
    void renderTracer(Tracer_marker *marker);
    void getSaturation(float x, float y);
    void tracer_calc(float x = 0, float y = 0);
    int modul(int a, int b);
    int axis_lookup(float in, QVector<float> *axis);
    void createParentWdgt();
    void fixSize();


private slots:
    void actionCopy();
    void actionPaste();
    void actionIncrease();
    void actionDecrease();

};

#endif // CUSTOMTABLEWIIDGET_H
