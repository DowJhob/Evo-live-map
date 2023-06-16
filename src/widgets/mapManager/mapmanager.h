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
    explicit mapManager(QWidget *parent = nullptr, ecuManager *_ecuManager = nullptr);
    ~mapManager();

    void colorFromFile(QString filename);
    void setECUmanager(ecuManager *_ecuManager = nullptr);

public slots:
    void createMapS(QHash<QString, Map *> *RAMtables);
    void clearMaps();


    void s_test(QHash<QString, Map*>* tables);


private:
    Ui::mapManager *ui;
    ecuManager *_ecuManager = nullptr;
    QVector<QColor> colormap;

    void createMap(mapDefinition *dMap);
    void addMapToTree(mapWidget *tab);
    QTreeWidgetItem *checkCategory(QString cat);

private slots:
    void itemChecks(QTreeWidgetItem *item, int column);

};

#endif // MAPMANAGER_H
