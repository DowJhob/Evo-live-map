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

class Patcher : public QGroupBox
{
    Q_OBJECT

public:
    QHash<QString, bloblist2*> bloblists;
    QHash<QString, patch*> patches;

    explicit Patcher(QWidget *parent = nullptr);
    ~Patcher();

public slots:
    void addPatches();
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

    QHexEdit hexEdit;
    QList<QTreeWidgetItem*> currentPatches{};

    QTreeWidgetItem* addPatchTreeItem(patch *pt);

    bloblist2* getBloblist(const QDomElement &element);

    void _parser(QIODevice *device);
    QTreeWidgetItem* checkCategory(QString cat);

    patchState checkPatch(QTreeWidgetItem *item);

signals:
    void applyPatch(bloblistPatch* patch);
    void applyOriginal(bloblistPatch* patch);

};

#endif // PATCHER_H
