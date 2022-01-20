#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent, mapDefinition *def, QVector<QColor> *colormap) : QWidget(nullptr, Qt::Window | Qt::WindowCloseButtonHint ), decl(def->declMap)
{
    setWindowTitle(def->declMap->Name);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout *_layout = new QGridLayout(this);
    _layout->setSizeConstraint(QLayout::SetFixedSize);

    mapTable = new mapView(decl);
    mapModel_ = new mapModel(mapTable, decl, colormap);
    mapModel_->setColorMap(colormap);
    QVector<float> map = def->Map.fromMemoryA(&decl->rom_scaling, decl->getMapSize());
    QVector<float> x_map = def->X_axis.fromMemoryA(&decl->X_axis.rom_scaling, decl->X_axis.elements);
    QVector<float> y_map = def->Y_axis.fromMemoryA(&decl->Y_axis.rom_scaling, decl->Y_axis.elements);
    mapModel_->fillMap(&map);
    mapModel_->fillHeader(&x_map, Qt::Horizontal);
    mapModel_->fillHeader(&y_map, Qt::Vertical);

    mapTable->setModel(mapModel_);

    _layout->addWidget(mapTable);
    setLayout(_layout);

    //connect(mapModel_, &mapModel::upd, mapTable, &mapView::reset);
//mapModel_->headerDataChanged(Qt::Horizontal, 0, 10);
    //show();
}

mapWidget::~mapWidget()
{
    //mapModel_->deleteLater();
    //mapTable->deleteLater();
}

void mapWidget::setColorMap(QVector<QColor> *colormap)
{
    mapModel_->setColorMap(colormap);
}

