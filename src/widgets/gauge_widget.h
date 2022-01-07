#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <src/ecu/ecu-definition.h>
#include <QLCDNumber>

class gauge_widget:public QWidget
{
    Q_OBJECT
public:
    uint offset;
    Scaling *scaling;
    QLCDNumber lcd;
    gauge_widget(QString name, uint DigitNum = 4, uint offset = 0, Scaling *scaling = nullptr, QWidget* parent = nullptr);

public slots:
    void display(float in);
};

#endif // GAUGE_WIDGET_H
