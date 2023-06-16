#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "qtreewidget.h"
#include "src/map-decl/map.h"
#include "src/widgets/ecuManager.h"
#include "src/widgets/mapWidget/mapwidget.h"
#include <QGroupBox>

namespace Ui {
class mapManager;
}

class mapManager : public QGroupBox
{
    Q_OBJECT

public:
    QHash<QString, mapWidget*> ptrRAMtables;

    explicit mapManager(QWidget *parent = nullptr, ecuManager *_ecuManager = nullptr);
    ~mapManager();

    void colorFromFile(QString filename);

public slots:
    void createMapS(QHash<QString, Map *> *RAMtables);
    void clearMaps();


private:
    Ui::mapManager *ui;
    ecuManager *_ecuManager = nullptr;
    QVector<QColor> colormap;

    void createMap(mapDefinition *dMap);
    void createMapTree(Map *tab);
    QTreeWidgetItem *checkCategory(QString cat);

private slots:
    void itemChecks(QTreeWidgetItem *item, int column);

};

#endif // MAPMANAGER_H
