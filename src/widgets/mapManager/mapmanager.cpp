#include "mapmanager.h"
#include "ui_mapmanager.h"

mapManager::mapManager(QWidget *parent, ecu* _ecu) : QGroupBox(parent), ui(new Ui::mapManager), _ecu(_ecu)
{
    ui->setupUi(this);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &mapManager::itemChecks);
    colorFromFile("C:\\Program Files (x86)\\OpenECU\\EcuFlash\\colormaps\\COLDFIRE.MAP") ;

    QObject::connect(_ecu,         &ecu::ecuConnected,          this, &mapManager::_connect);
    QObject::connect(this, &mapManager::mapCreated,     _ecu,         &ecu::startLog);

    QObject::connect(_ecu,         &ecu::s_test,                this, &mapManager::s_test, Qt::QueuedConnection);
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

    connect(table->mapModel_, &mapModel::updateRAM, _ecu, &ecu::updateRAM);

    connect(_ecu->ECUproto, &DMA_proto::logReady, table->mapTable, &mapView::logReady);

    addMapToTree(table);
}

void mapManager::clearMaps()
{
    for(int i = 0; i<  ui->treeWidget->topLevelItemCount(); i++)
    {
        auto item = ui->treeWidget->takeTopLevelItem(i);
        if(item != nullptr)
        {
            auto data = item->data(2, Qt::UserRole);
            if(!data.isNull())
            {
                mapWidget *map = data.value<mapWidget*>();
                map->hide();
                map->deleteLater();
            }
            else
            {
                for(auto child_item : item->takeChildren())
                {
                    if(child_item != nullptr)
                    {
                        auto data = child_item->data(2, Qt::UserRole);
                        if(!data.isNull())
                        {
                            mapWidget *map = data.value<mapWidget*>();
                            map->hide();
                            map->deleteLater();
                        }
                    }
                }
            }
        }
    }
    //    ui->treeWidget->clear();
    //    ptrRAMtables.clear();
}

void mapManager::s_test()
{
    for ( Map *tab : _ecu->ecuDef.RAMtables)
    {
        mapWidget *table = new mapWidget(nullptr, tab);
        addMapToTree(table);
    }
}

void mapManager::addMapToTree(mapWidget *tab)
{
    if (tab->decl->addr != 0)                                    // проверим что это таблица карт, а не таблица патчей
    {
        QTreeWidgetItem* map_name_item = new QTreeWidgetItem(QStringList() << tab->decl->Name + " RAM address: " + QString::number(tab->decl->addr, 16));
        map_name_item->setFlags(map_name_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        map_name_item->setCheckState(0, Qt::Unchecked);
        QTreeWidgetItem* axis_item;
        if (tab->decl->X_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->decl->X_axis.Name + "  ROM address: " + QString::number(tab->decl->X_axis.addr, 16) + "  scaling: " + tab->decl->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        if (tab->decl->Y_axis.addr > 0)
        {
            axis_item = new QTreeWidgetItem(QStringList() << tab->decl->Y_axis.Name + "  ROM address: " + QString::number(tab->decl->Y_axis.addr, 16) + "  scaling: " + tab->decl->X_axis.rom_scaling.toexpr);
            axis_item->setFlags(0);
            map_name_item->addChild(axis_item);
        }
        QTreeWidgetItem* catItem = checkCategory(tab->decl->Category);
        if (catItem != nullptr)
        {
            catItem->addChild(map_name_item);
        }
        else
            ui->treeWidget->addTopLevelItem(map_name_item);

        map_name_item->setData(2, Qt::UserRole, QVariant::fromValue<mapWidget*>( tab ));
    }
}

QTreeWidgetItem* mapManager::checkCategory(QString cat)
{
    QTreeWidgetItem* catItem = nullptr;
    if (!cat.isEmpty())
    {
        QList<QTreeWidgetItem*> all = ui->treeWidget->findItems(cat, Qt::MatchContains | Qt::MatchRecursive, 0);
        if(!all.isEmpty())
        {
            catItem = all.at(0);
        }
        else {

            catItem = new QTreeWidgetItem(QStringList() << cat);
            ui->treeWidget->addTopLevelItem(catItem);
        }
    }
    return catItem;
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

void mapManager::setECU(ecu *_ecu)
{
    this->_ecu = _ecu;
}

void mapManager::_connect(bool state)
{
if(state)
    createMapS();
else
    clearMaps();
}

void mapManager::createMapS()
{
    // переберем все описания таблиц
    for ( Map *tab : qAsConst(_ecu->ecuDef.RAMtables) )
    {
        createMap( _ecu->getMap(tab) );
    }
    emit mapCreated();
}

void mapManager::itemChecks(QTreeWidgetItem *item, int column)
{
    auto data = item->data(2, Qt::UserRole);
    mapWidget *map = data.value<mapWidget*>();

    if (map == nullptr)
        return;
    if ( item->checkState(column) )
        map->show();
    else
        map->hide();
}
