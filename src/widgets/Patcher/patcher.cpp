#include "patcher.h"
#include "ui_patcher.h"

Patcher::Patcher(QWidget *parent) : QGroupBox(parent), ui(new Ui::Patcher)
{
    ui->setupUi(this);
    ui->gridLayout_2->addWidget(&hexEdit, 3, 1, 1, 7);

    hexEdit.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    hexEdit.setAddressWidth(8);
    hexEdit.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

Patcher::~Patcher()
{
    delete ui;
}

void Patcher::addPatches()
{
    for ( auto *patch : qAsConst(this->patches) )
    {
        QTreeWidgetItem *item = addPatchTreeItem(patch);
        checkPatch(item);
    }
}

void Patcher::clearPatches()
{
    ui->treeWidget->clear();
    bloblists.clear();
    patches.clear();
}

QTreeWidgetItem* Patcher::addPatchTreeItem(patch *pt)
{
    QTreeWidgetItem* patchItem = new QTreeWidgetItem(QStringList() << pt->Name + " | RAM address: " + QString::number(pt->addr, 16));
    patchItem->setFlags(patchItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    patchItem->setCheckState(0, Qt::Unchecked);
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

void Patcher::_parser(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    //открываем документ
    if (!doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        //            lastError = "Line %1, column %2";
        //            lastError = lastError.arg( errorLine).arg(errorColumn);
        qDebug() << errorStr << errorLine << errorColumn ;
        return;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "rom") {
        //            lastError = "The file is not a rom xml";
        return;
    }
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        const QDomElement el = node.toElement();

        if ((el.tagName() == "scaling") && (el.attribute("storagetype") == "bloblist"))                                                 //сохраним все блобсы
        {
            auto blb = getBloblist(el);
            bloblists.insert(el.attribute("name"), blb);
        }
        node = node.nextSibling();
    }

    node = root.firstChild();
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
        hexEdit.setAddressOffset(selectedPatch->addr);
        auto rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Patched.count());

        if(!rom.isEmpty() && rom == selectedPatch->blobs->Patched)
        {
            item->setText(1, "Patched");
            item->setData(3, Qt::UserRole, 'P');
            return Patched;
        }
        else
        {
            rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Original.count());
            if(!rom.isEmpty() && rom == selectedPatch->blobs->Original)
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

void Patcher::itemChecks(QTreeWidgetItem *item, int column)
{
    currentPatches.clear();
    if(!item->data(2, Qt::UserRole).isNull())
    {
        //checkPatch(item);
        currentPatches.append(item);
    }
    else
    {
        for(int i = 0; i< item->childCount(); i++)
        {
            //checkPatch(item);
            currentPatches.append(item->child(i));
        }
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
}

void Patcher::selectXMLfilename()
{
    QFile XMLfile_handler;
    clearPatches();
    //    XMLfile_handler.setFileName( "C:\\Program Files (x86)\\OpenECU\\EcuFlash\\rommetadata\\mitsubishi\\evo\\88840017 2006 EDM Lancer Evolution MT.xml");
    XMLfile_handler.setFileName( QFileDialog::getOpenFileName(nullptr, tr("Open xml"), QApplication::applicationDirPath(), tr("xml files (*.xml)")));

    if (!XMLfile_handler.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(XMLfile_handler.fileName()));
        return;
    }
    _parser(&XMLfile_handler);
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
        auto statePatch = data.value<char>();

        switch (statePatch)
        {
        case patchState::dontMatchBoth:
            msgBox.setText("Does not match both!");
            if (msgBox.exec() == QMessageBox::Ok)
        case patchState::NotPatched:
                hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Patched.count(), selectedPatch->blobs->Patched);
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
        auto statePatch = data.value<char>();

        switch (statePatch)
        {
        case patchState::dontMatchBoth:
            msgBox.setText("Does not match both!");
            if (msgBox.exec() == QMessageBox::Ok)
        case patchState::Patched:
                hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Original.count(), selectedPatch->blobs->Original);
                item->setData(3, Qt::UserRole, 'N');
                break;
            case patchState::NotPatched:
                //            msgBox.setText("Allready patched");

                break;
            default:
                break;
        }
    }
}
