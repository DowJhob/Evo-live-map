#include "patcher.h"
#include "ui_patcher.h"

Patcher::Patcher(QWidget *parent) : QGroupBox(parent), ui(new Ui::Patcher)
{
    ui->setupUi(this);
    ui->gridLayout_2->addWidget(&hexEdit, 3, 3, 1, 5);

    hexEdit.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    hexEdit.setAddressWidth(8);
    hexEdit.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //hexEdit.setDynamicBytesPerLine(true);
}

Patcher::~Patcher()
{
    delete ui;
    clearPatches();
}

void Patcher::addPatches()
{
    for ( auto *patch : qAsConst(patches) )
    {
        QTreeWidgetItem *item = addPatchTreeItem(patch);
        checkPatch(item);
    }
}

void Patcher::checkPatches()
{
        for(int i =0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);

        auto data = item->data(2, Qt::UserRole);
        if(!data.isNull())
        {
//            auto Patch = data.value<patch*>();
            checkPatch(item);
        }
        else
        {
            for(int j =0; j < item->childCount(); j++)
            {
                QTreeWidgetItem *subItem = item->child(j);
                auto subData = subItem->data(2, Qt::UserRole);
                if(!subData.isNull())
                {

                    checkPatch(subItem);
                }
            }
        }
    }
}

void Patcher::clearPatches()
{
    ui->treeWidget->clear();
    for ( auto *patch : qAsConst(patches) )
    {
        delete(patch->blobs);
        delete(patch);
    }
    bloblists.clear();
    patches.clear();
}

QTreeWidgetItem* Patcher::addPatchTreeItem(patch *pt)
{
    QTreeWidgetItem* patchItem = new QTreeWidgetItem(QStringList() << pt->Name + " | RAM address: " + QString::number(pt->addr, 16));
    patchItem->setFlags(patchItem->flags() | Qt::ItemIsUserCheckable /*| Qt::ItemIsSelectable*/);
//    patchItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* blobItem;
    if (!pt->blobs->Original.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Original:");
        blobItem->setFlags(Qt::NoItemFlags);
        patchItem->addChild(blobItem);
    }
    if (!pt->blobs->Patched.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Patched:");
        blobItem->setFlags(Qt::NoItemFlags);
        patchItem->addChild(blobItem);
    }

    QTreeWidgetItem* catItem = checkCategory(pt->Category);
    if (catItem != nullptr)
    {
        catItem->addChild(patchItem);
    }
    else
        ui->treeWidget->addTopLevelItem(patchItem);

    patchItem->setData(2, Qt::UserRole, QVariant::fromValue<patch*>( pt ));
    return patchItem;
}

bloblist2 *Patcher::getBloblist(const QDomElement &element)
{
    QDomNode node = element.firstChild();

    bloblist2 *pt = new bloblist2;

    while (!node.isNull())
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "data")                                       //находим блобы
        {
            if (el.attribute("name") == "Original")
                pt->Original = QByteArray::fromHex(el.attribute("value").toUtf8());
            else if (el.attribute("name") == "Patched")
                pt->Patched = QByteArray::fromHex(el.attribute("value").toUtf8());
        }

        node = node.nextSibling();
    }
    return pt;
}

QTreeWidgetItem *Patcher::checkCategory(QString cat)
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

patchState Patcher::checkPatch(QTreeWidgetItem *item)
{
    auto data = item->data(2, Qt::UserRole);
    auto selectedPatch = data.value<patch*>();
    if(selectedPatch != nullptr)
    {
        hexEdit.setAddrpos(selectedPatch->addr, 0);
        hexEdit.setCursorPosition(selectedPatch->addr * 2);



        auto rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Patched.count());
        qDebug() << selectedPatch->Name << selectedPatch->addr << rom.count() << rom ;

        if(!rom.isEmpty() && rom == selectedPatch->blobs->Patched)
        {
            item->setText(1, "Patched");
            item->setData(3, Qt::UserRole, 'P');
            return Patched;
        }
        else
        {
            auto rom2 = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Original.count());
            qDebug() << selectedPatch->Name << selectedPatch->addr << rom2.count() << rom2;
            if(!rom2.isEmpty() && rom2 == selectedPatch->blobs->Original)
            {
                item->setText(1, "Not patched");
                item->setData(3, Qt::UserRole, 'N');
                return NotPatched;
            }
            else
            {
                item->setText(1, "Does not match both!");
                item->setData(3, Qt::UserRole, 'B');
                return dontMatchBoth;
            }
        }
    }
}

void Patcher::serialize(QDomElement el)
{
    if(el.tagName() == "include")
    {
        QString includeID2 = el.text();
        //            qDebug() << "T==================================================== includeID =======================================================" << includeID;
        if(!includeID2.isEmpty())
        {
            QDir dir(QApplication::applicationDirPath() + "/xml");
            for (const QFileInfo &file : dir.entryInfoList(QDir::Files))
            {
                //               qDebug() << "T======================================= file.fileName() ======================================" << includeID << xmlid_node.toElement().text() << file.fileName();
                QFile iod(file.absoluteFilePath());
                if (!iod.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(iod.fileName()));
                    return;
                }
                //                    qDebug() << "T============================================== xmlid_node =============================================================";
                _parser(&iod, includeID2);
            }
        }
    }
    else
    {
        if ((el.tagName() == "scaling") && (el.attribute("storagetype") == "bloblist"))                                                 //сохраним все блобсы
        {
            auto blb = getBloblist(el);
            bloblists.insert(el.attribute("name"), blb);
        }
    }
}

void Patcher::subSerialize(QDomNode node)
{
    while (!node.isNull())
    {
        const QDomElement el = node.toElement();

        if (el.tagName() == "table")                                                   // находим таблицу
        {
            QString scalingName = el.attribute("scaling");
            auto blobs = bloblists.value(scalingName, nullptr);
            if(blobs != nullptr)
            {
                patch *pt = new patch();

                pt->Category = el.attribute("category");
                pt->Name = el.attribute("name");
                pt->addr = el.attribute("address").toUInt(nullptr, 16);
                pt->blobs = blobs;
                patches.insert(pt->Name, pt);
            }
        }
        node = node.nextSibling();
    }
}

void Patcher::itemClicked(QTreeWidgetItem *item, int column)
{
    currentPatches.clear();
    if(!item->data(2, Qt::UserRole).isNull())
    {
        currentPatches.append(item);
    }
    else
    {
        for(int i = 0; i< item->childCount(); i++)
        {
            currentPatches.append(item->child(i));
        }
    }
    auto firstItem = currentPatches[0];
    auto data = firstItem->data(2, Qt::UserRole);
    auto Patch = data.value<patch*>();
    if(!data.isNull())
    {
        hexEdit.setAddrpos(Patch->addr, 0);
        hexEdit.setCursorPosition(Patch->addr * 2);
    }
}

void Patcher::selectROMfilename()
{
    if (ROMfile_handler.isOpen())
        ROMfile_handler.close();

    ROMfile_handler.setFileName( QFileDialog::getOpenFileName(nullptr, tr("Open bin"), QApplication::applicationDirPath(), tr("bin files (*.bin)")));

    if (!hexEdit.setData(ROMfile_handler))
    {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(ROMfile_handler.fileName()));
        return;
    }
    checkPatches();
}

void Patcher::selectXMLfilename()
{
    QFile XMLfile_handler;
    clearPatches();
    //    XMLfile_handler.setFileName( "C:\\Program Files (x86)\\OpenECU\\EcuFlash\\rommetadata\\mitsubishi\\evo\\88840017 2006 EDM Lancer Evolution MT.xml");
    //    XMLfile_handler.setFileName( QApplication::applicationDirPath() + "/xml/90550001 fuel pump patch.xml");

    XMLfile_handler.setFileName( QFileDialog::getOpenFileName(nullptr, tr("Open xml"), QApplication::applicationDirPath() + "/xml", tr("xml files (*.xml)")));

    if (!XMLfile_handler.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(XMLfile_handler.fileName()));
        return;
    }
    _parser(&XMLfile_handler, "");
    XMLfile_handler.close();
    addPatches();
}

void Patcher::Undo()
{
    hexEdit.undo();
}

void Patcher::Save()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool ok = hexEdit.write(ROMfile_handler);
    //QApplication::setOverrideCursor(Qt::CursorMoveStyle);
    QApplication::restoreOverrideCursor();
    if (!ok) {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot write file %1.").arg(ROMfile_handler.fileName()));
    }
}

void Patcher::Apply()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Ок - proceed anyway\nCancel - it cancel");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setDefaultButton(QMessageBox::Ok);

    for( auto item : qAsConst(currentPatches))
    {
        auto data = item->data(2, Qt::UserRole);
        auto selectedPatch = data.value<patch*>();

        auto stateData = item->data(3, Qt::UserRole);
        auto statePatch = stateData.value<char>();

        switch (statePatch)
        {
        case patchState::dontMatchBoth:
            msgBox.setText("Does not match both!");
            if (msgBox.exec() == QMessageBox::Ok)
        case patchState::NotPatched:
                hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Patched.count(), selectedPatch->blobs->Patched);
                item->setText(1, "Patched:");
                item->setData(3, Qt::UserRole, 'P');
                break;
            case patchState::Patched:
                //            msgBox.setText("Allready patched");
                break;
            default:
                break;
        }
    }
}

void Patcher::Undo_patch()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Ок - proceed anyway\nCancel - it cancel");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setDefaultButton(QMessageBox::Ok);

    for( auto item : qAsConst(currentPatches))
    {
        auto data = item->data(2, Qt::UserRole);
        auto selectedPatch = data.value<patch*>();

        auto stateData = item->data(3, Qt::UserRole);
        auto statePatch = stateData.value<char>();

        switch (statePatch)
        {
        case patchState::dontMatchBoth:
            msgBox.setText("Does not match both!");
            if (msgBox.exec() == QMessageBox::Ok)
        case patchState::Patched:
                hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Original.count(), selectedPatch->blobs->Original);
                item->setData(3, Qt::UserRole, 'N');
        item->setText(1, "Original:");
                break;
            case patchState::NotPatched:
                //            msgBox.setText("Allready patched");
                break;
            default:
                break;
        }
    }
}

void Patcher::_readECU()
{

}

void Patcher::_writeECU()
{
    emit writeECU(hexEdit.data());
}
