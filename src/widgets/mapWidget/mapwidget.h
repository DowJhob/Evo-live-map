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
    Map *decl = nullptr;

    mapWidget(QWidget *parent = nullptr, mapDefinition *def = nullptr, QVector<QColor> *colormap = nullptr);

    explicit mapWidget(QWidget *parent = nullptr, Map *decl = nullptr);

    ~mapWidget();

    void setColorMap(QVector<QColor> *colormap);

private:

private slots:

signals:
    void updateRAM(offsetMemory);

};

#endif // MAPWIDGET_H
