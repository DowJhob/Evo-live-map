#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

#include <QGridLayout>

#include "mapmodel.h"
#include "mapview.h"


class mapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mapWidget(QWidget *parent = nullptr, mapDefinition *def = nullptr, QVector<QColor> *colormap = nullptr);

    mapView *mapTable;
    mapModel *mapModel_;
    void setColorMap(QVector<QColor> *colormap);

private:
    Map *decl = nullptr;

private slots:

signals:
    void updateRAM(abstractMemoryScaled);

};

#endif // MAPWIDGET_H
