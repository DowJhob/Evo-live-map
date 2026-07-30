#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include "qtoolbar.h"
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QContextMenuEvent>
#include <QActionGroup>

#include <src/ecu/ecu-definition.h>

class gaugeWidget:public QToolBar
{
    Q_OBJECT
public:
    // uint offset;
    // Scaling *scaling;
    gaugeWidget(QString name, uint DigitNum = 4, QWidget* parent = nullptr);
    ~gaugeWidget();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* pe);

public slots:
    void display(QString in);

private:
    QLCDNumber *lcd;
    QGridLayout *lay;
    QLabel *lab;

    QMenu* m_pmnu;
    QWidget a;

private slots:
    void setProto(QAction* pAction);

};

#endif // GAUGE_WIDGET_H
