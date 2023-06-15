#include "mapmanager.h"
#include "ui_mapmanager.h"

mapManager::mapManager(QWidget *parent, ecuManager* _ecuManager) : QGroupBox(parent), ui(new Ui::mapManager), _ecuManager(_ecuManager)
{
    ui->setupUi(this);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &mapManager::itemChecks);
    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;
}

mapManager::~mapManager()
{
    delete ui;
}

void mapManager::createMap(mapDefinition *dMap)
{
    //создаем таблицу с заданной размерностью
    //qDebug() << "====================== Create table : " << dMap->declMap->Name << " ================================";
    dMap->declMap->elements = dMap->declMap->X_axis.elements * dMap->declMap->Y_axis.elements;

    mapWidget *table = new mapWidget(nullptr, dMap, &colormap);

    connect(table->mapModel_, &mapModel::updateRAM, _ecuManager, &ecuManager::updateRAM);

    connect(_ecuManager, &ecuManager::logReady, table->mapTable, &mapView::logReady);

    ptrRAMtables.insert(dMap->declMap->Name, table );

    createMapTree(dMap->declMap);
}

void mapManager::clearMaps()
{
    ui->treeWidget->clear();
    for(mapWidget *c: qAsConst(ptrRAMtables))
    {
        c->hide();
        c->deleteLater();
    }
    ptrRAMtables.clear();
}

void mapManager::createMapTree(Map *tab)
{
    if (tab->addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
    {
        QTreeWidgetItem* map_name_item = new QTreeWidgetItem(QStringList() << tab->Name + " RAM address: " + QString::number(tab->addr, 16));
        map_name_item->setFlags(map_name_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        map_name_item->setCheckState(0, Qt::Unchecked);
        QTreeWidgetItem* axis_item;
        if (tab->X_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->X_axis.Name + "  ROM address: " + QString::number(tab->X_axis.addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        if (tab->Y_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->Y_axis.Name + "  ROM address: " + QString::number(tab->Y_axis.addr, 16) + "  scaling: " + tab->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        ui->treeWidget->addTopLevelItem(map_name_item);
    }
}

void mapManager::colorFromFile(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "file fail";
        //return nullptr;
    }

    QStringList sl;

    //QVector<QColor> *colormap = new QVector<QColor>;

    qDebug() << "file ";
    while( !file->atEnd())
    {
        sl = QString(file->readLine()).simplified()
                .split(' ');
        if(sl.count() == 3)
            colormap.append(QColor(sl[0].toInt(), sl[1].toInt(), sl[2].toInt()));
        //qDebug() << "colormap" ;
    }
    delete file;
}

void mapManager::createMapS(QHash<QString, Map*> *RAMtables)
{
    // переберем все описания таблиц
    for ( Map *tab : *qAsConst(RAMtables) )
    {
        createMap( _ecuManager->ECU->getMap(tab) );
    }
}

void mapManager::itemChecks(QTreeWidgetItem *item, int column)
{
    mapWidget *map = ptrRAMtables.value( item->text(column).split(" RAM").at(0), nullptr );
    if (map == nullptr)
        return;
    if ( item->checkState(column) )
        map->show();
    else
        map->hide();
}
