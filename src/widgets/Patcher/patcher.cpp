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

void Patcher::addPatches(QHash<QString, patch*> *patches)
{
    //    this->patches = patches;
    for ( auto *patch : qAsConst(this->patches) )
    {
        addPatchItem(patch);
    }
}

void Patcher::clearPatches()
{
    ui->treeWidget->clear();
}

void Patcher::addPatchItem(patch *pt)
{
    QTreeWidgetItem* patchItem = new QTreeWidgetItem(QStringList() << pt->Name + " | RAM address: " + QString::number(pt->addr, 16));
    patchItem->setFlags(patchItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
    patchItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* blobItem;
    if (!pt->blobs->Original.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Original:");
        blobItem->setFlags(0);
        patchItem->addChild(blobItem);
    }
    if (!pt->blobs->Patched.isNull())
    {
        blobItem = new QTreeWidgetItem(QStringList() << "Patched:");
        blobItem->setFlags(0);
        patchItem->addChild(blobItem);
    }

    patchItem->setData(2, Qt::UserRole, QVariant::fromValue<patch*>( pt ));

    ui->treeWidget->addTopLevelItem(patchItem);
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
                pt->Name = el.attribute("name");
                pt->addr = el.attribute("address").toUInt(nullptr, 16);
                pt->blobs = blobs;
                patches.insert(pt->Name, pt);
            }
        }

        node = node.nextSibling();
    }
}

void Patcher::itemChecks(QTreeWidgetItem *item, int column)
{
    auto data = item->data(2, Qt::UserRole);
    selectedPatch = data.value<patch*>();
    hexEdit.setAddressOffset(selectedPatch->addr);

    auto rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Original.count());

    qDebug() << "ptr_patch->addr" << selectedPatch->addr << Qt::endl
             << "rom" << rom << Qt::endl
             << "ptr_patch->blobs->Original" << selectedPatch->blobs->Original << Qt::endl
             << "ptr_patch->blobs->Patched" << selectedPatch->blobs->Patched << Qt::endl;

    if(!rom.isEmpty())
    {
        if(rom == selectedPatch->blobs->Patched)
            item->setText(1, "Patched");
        else
            if(rom == selectedPatch->blobs->Original)
                item->setText(1, "Not patched");
            else
                item->setText(1, "Does not match both!");
    }

}

void Patcher::selectROMfilename()
{
    if (!ROMfile_handler.isOpen())
        ROMfile_handler.close();
    ROMfile.clear();

    ROMfile_handler.setFileName( QFileDialog::getOpenFileName(nullptr, tr("Open bin"), QApplication::applicationDirPath(), tr("bin files (*.bin)")));

    if (!hexEdit.setData(ROMfile_handler))
    {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(ROMfile_handler.fileName()));
        return;
    }
    ;
}

void Patcher::selectXMLfilename()
{
    QFile XMLfile_handler;
    if (!XMLfile_handler.isOpen())
        XMLfile_handler.close();
    //ROMfile.clear();
    XMLfile_handler.setFileName( "C:\\Program Files (x86)\\OpenECU\\EcuFlash\\rommetadata\\mitsubishi\\evo\\88840017 2006 EDM Lancer Evolution MT.xml");
    //    XMLfile_handler.setFileName( QFileDialog::getOpenFileName(nullptr, tr("Open xml"), QApplication::applicationDirPath(), tr("xml files (*.xml)")));

    if (!XMLfile_handler.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Patcher"), tr("Cannot open file %1.").arg(XMLfile_handler.fileName()));
        return;
    }
    //        ROMfile = ROMfile_handler.readAll();

    _parser(&XMLfile_handler);
    XMLfile_handler.close();
    addPatches(&patches);
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
    auto rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Patched.count());
    if(!rom.isEmpty())
    {
        if(rom == selectedPatch->blobs->Patched)
            QMessageBox::warning(this, tr("Patcher"), tr("Allready patched"));
        else
            hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Patched.count(), selectedPatch->blobs->Patched);
    }
}

void Patcher::Undo_patch()
{
    auto rom = hexEdit.dataAt(selectedPatch->addr, selectedPatch->blobs->Original.count());
    if(!rom.isEmpty())
    {
        if(rom == selectedPatch->blobs->Original)
            QMessageBox::warning(this, tr("Patcher"), tr("Allready original"));
        else
            hexEdit.replace(selectedPatch->addr, selectedPatch->blobs->Original.count(), selectedPatch->blobs->Original);
    }
}
