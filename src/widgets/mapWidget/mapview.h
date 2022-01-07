#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QDebug>
#include <QTableView>
#include <QAction>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QGridLayout>
#include <QHeaderView>

#include "../../ecu/ecu-definition.h"
#include "mapmodel.h"

class mapView : public QTableView
{
    Q_OBJECT
public:
    mapView(Map *declaration);
    Map *declaration;

public slots:
    void logReady(QVector<float> scaledValue);

protected:
    void keyPressEvent(QKeyEvent *event);

private:

private slots:
    void actionCopy();
    void actionPaste();
    void actionIncrease();
    void actionDecrease();

signals:
    void updateRAM();

};

#endif // MAPVIEW_H
