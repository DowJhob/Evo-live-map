#ifndef PATCHER_H
#define PATCHER_H

#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "src/map-decl/bloblistpatch.h"
#include "src/widgets/hexEditor/qhexedit/qhexedit.h"

namespace Ui {
class Patcher;
}

class bloblist2
{
public:
    bloblist2(){
//        Original = new QByteArray;
//        Patched = new QByteArray;
    }
    QByteArray Original;
    QByteArray Patched;
};

class patch
{
public:
    QString Name;
    quint32 addr = 0;
    bloblist2 *blobs;
};

Q_DECLARE_METATYPE(patch*);

class Patcher : public QGroupBox
{
    Q_OBJECT

public:
    QHash<QString, bloblist2*> bloblists;
    QHash<QString, patch*> patches;

    explicit Patcher(QWidget *parent = nullptr);
    ~Patcher();


public slots:
    void addPatches(QHash<QString, patch *> *patches);
    void clearPatches();

private slots:
    void itemChecks(QTreeWidgetItem *item, int column);
    void selectROMfilename();
    void selectXMLfilename();

    void Undo();
    void Save();
    void Apply();
    void Undo_patch();


private:
    Ui::Patcher *ui;
    QFile ROMfile_handler;
    QByteArray ROMfile;
patch* selectedPatch = nullptr;

    QHexEdit hexEdit;
    void addPatchItem(patch *pt);


    bloblist2* getBloblist(const QDomElement &element);

    void _parser(QIODevice *device);


signals:
    void applyPatch(bloblistPatch* patch);
    void applyOriginal(bloblistPatch* patch);
};

#endif // PATCHER_H
