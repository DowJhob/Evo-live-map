#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QGroupBox>
#include "qtreewidget.h"
#include "src/map-decl/map.h"
#include "src/widgets/ecuManager.h"
#include "src/widgets/mapWidget/mapwidget.h"

namespace Ui {
class mapManager;
}

class mapManager : public QGroupBox
{
    Q_OBJECT

public:
    explicit mapManager(QWidget *parent = nullptr, ecu *_ecu = nullptr);
    ~mapManager();

    void colorFromFile(QString filename);
    void setECU(ecu *_ecu = nullptr);

public slots:
    void _connect(bool state);


    void s_test();


private:
    Ui::mapManager *ui;
    ecu *_ecu = nullptr;
    QVector<QColor> colormap;

    void createMap(mapDefinition *dMap);
    void addMapToTree(mapWidget *tab);
    QTreeWidgetItem *checkCategory(QString cat);
    void createMapS();
    void clearMaps();

private slots:
    void itemChecks(QTreeWidgetItem *item, int column);

signals:
    void mapCreated();

};

#endif // MAPMANAGER_H
