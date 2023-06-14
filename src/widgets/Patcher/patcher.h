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
    explicit Patcher(QWidget *parent = nullptr);
    ~Patcher();

    void addPatches(bloblistPatch* patch);
    void addPatchItem(bloblistPatch* patch);

public slots:
    void itemChecks(QTreeWidgetItem *item, int column);

private:
    Ui::Patcher *ui;
};

#endif // PATCHER_H
