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
    mapView *mapTable;
    mapModel *mapModel_;

    explicit mapWidget(QWidget *parent = nullptr, mapDefinition *def = nullptr, QVector<QColor> *colormap = nullptr);
    ~mapWidget();

    void setColorMap(QVector<QColor> *colormap);

private:
    Map *decl = nullptr;

private slots:

signals:
    void updateRAM(offsetMemory);

};

#endif // MAPWIDGET_H
