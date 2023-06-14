#include "patcher.h"
#include "ui_patcher.h"

Patcher::Patcher(QWidget *parent) : QGroupBox(parent), ui(new Ui::Patcher)
{
    ui->setupUi(this);
}

Patcher::~Patcher()
{
    delete ui;
}

void Patcher::addPatches(QHash<QString, bloblistPatch *> *patches)
{
    this->patches = patches;
    for ( auto *patch : *qAsConst(patches) )
    {
        addPatchItem(patch);
    }
}

void Patcher::clearPatches()
{
ui->treeWidget->clear();
}

void Patcher::addPatchItem(bloblistPatch* patch)
{
    QTreeWidgetItem* patchItem = new QTreeWidgetItem(QStringList() << patch->Name + " | RAM address: " + QString::number(patch->addr, 16));
    patchItem->setFlags(patchItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    patchItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* blobItem;
    if (!patch->scaling.Original.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Original:");
        blobItem->setFlags(0);
        patchItem->addChild(blobItem);
    }
    if (!patch->scaling.Patched.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Patched:");
        blobItem->setFlags(0);
        patchItem->addChild(blobItem);
    }
    ui->treeWidget->addTopLevelItem(patchItem);
}

void Patcher::itemChecks(QTreeWidgetItem *item, int column)
{
    //    mapWidget *map = ptrRAMtables.value( item->text(column).split(" RAM").at(0), nullptr );
    //    if (map == nullptr)
    //        return;
    //    if ( item->checkState(column) )
    //        map->show();
    //    else
    //        map->hide();
}
