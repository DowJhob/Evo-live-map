#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractTableModel>
#include <QPalette>
#include <QBrush>
#include <QColor>
#include <QElapsedTimer>

#include "../../ecu/ecu-definition.h"

struct Tracer_marker2
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

class mapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    Map *declaration;
    //explicit
    mapModel(QObject *parent = nullptr, Map *decl = nullptr, QVector<QColor> *colormap = nullptr);
    ~mapModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    void setColorMap(QVector<QColor> *colormap);
    void fillMap(QVector<float> *map );
    void fillHeader(QVector<float> *map, Qt::Orientation orientation);

    void c_updateRAM(float value, const QModelIndex &index);

public slots:
    void logReady(QVector<float> scaledValue);

private:
    float mapColorDiscret = 1;
    float xAxisColorDiscret = 1;
    float yAxisColorDiscret = 1;

    QVector<QColor> *coloringMap;                    //карта - легенда для раскрашивания значений
    QBrush *backgroundColorMap = nullptr;                        // собствено значения цвета для таблицы
    float *mapData2 = nullptr;                                 //  считаем что уложено построчно
    float *horizontalHeaderData = nullptr;
    float *verticalHeaderData = nullptr;



    //============== tracer ====================
    Tracer_marker2 current_marker = {};
    Tracer_marker2 pred_marker = {};

    QBrush background(int x, int y);
    void setBackground(int x, int y, QBrush color);

    void tracer_calc(float x, float y);

    void saveTracer();

    void renderTracer(Tracer_marker2 *marker);

    void getSaturation(float x, float y);

    int modul(int a, int b);

    int axis_lookup(float in, float *axis, int size);


    QBrush getColor(float value, int min, float ColorDiscret) const;


signals:    
    void updateRAM(abstractMemoryScaled);
    void upd();

};

#endif // MAPMODEL_H
