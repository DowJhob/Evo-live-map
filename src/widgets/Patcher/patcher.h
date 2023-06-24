#ifndef PATCHER_H
#define PATCHER_H

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDomElement>

#include "3rdparty/qhexedit2/src/qhexedit.h"
#include "src/xmlmanager.h"

namespace Ui {
class Patcher;
}

enum patchState
{
    dontMatchBoth = 'B',
    NotPatched = 'N',
    Patched = 'P',
    OutsideROM = 'O'
};

struct bloblist2
{
    QByteArray Original;
    QByteArray Patched;
};

struct patch
{
    QString Category;
    QString Name;
    quint32 addr = 0;
    bloblist2 *blobs;
};

Q_DECLARE_METATYPE(patch*);

class Patcher : public QGroupBox, xmlManager
{
    Q_OBJECT

public:
    QHash<QString, bloblist2*> bloblists;
    QHash<QString, patch*> patches;

    explicit Patcher(QWidget *parent = nullptr);
    ~Patcher();

public slots:
    void addPatches();
    void checkPatches();
    void clearPatches();

private slots:
    void itemClicked(QTreeWidgetItem *item, int column);
    void selectROMfilename();
    void selectXMLfilename();

    void Undo();
    void Save();
    void Apply();
    void Undo_patch();
 //   QDomElement getXMLDom(QIODevice *device);


    void _writeECU(QByteArray data);

private:
    Ui::Patcher *ui;
    QFile ROMfile_handler;

    QHexEdit hexEdit;
    QList<QTreeWidgetItem*> currentPatches{};

    QTreeWidgetItem* addPatchTreeItem(patch *pt);

    bloblist2* getBloblist(const QDomElement &element);

 //   void _parser(QIODevice *device, QString includeID);
    QTreeWidgetItem* checkCategory(QString cat);

    patchState checkPatch(QTreeWidgetItem *item);


    void serialize(QDomElement el);

    void subSerialize(QDomNode node);

signals:
    void applyPatch(bloblist2* patch);
    void applyOriginal(bloblist2* patch);
    void writeECU(QByteArray);
    void readECU();

};

#endif // PATCHER_H
