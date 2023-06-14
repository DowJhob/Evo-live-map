#ifndef PATCHER_H
#define PATCHER_H

#include "src/map-decl/bloblistpatch.h"
#include <QGroupBox>
#include <QTreeWidgetItem>

namespace Ui {
class Patcher;
}

class Patcher : public QGroupBox
{
    Q_OBJECT

public:
    QHash<QString, bloblistPatch *> *patches;

    explicit Patcher(QWidget *parent = nullptr);
    ~Patcher();


public slots:
    void addPatches(QHash<QString, bloblistPatch*> *patches);
    void clearPatches();

private slots:
    void itemChecks(QTreeWidgetItem *item, int column);

private:
    Ui::Patcher *ui;
    void addPatchItem(bloblistPatch* patch);
};

#endif // PATCHER_H
