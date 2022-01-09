#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QMenu>
#include <QContextMenuEvent>

#include <src/ecu/ecu-definition.h>

class gauge_widget:public QWidget
{
    Q_OBJECT
public:
    uint offset;
    Scaling *scaling;
    QLCDNumber lcd;
    gauge_widget(QString name, uint DigitNum = 4, QWidget* parent = nullptr);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* pe);

public slots:
    void display(float in);

private:
    QMenu* m_pmnu;

private slots:
    void setProto(QAction* pAction);

};

#endif // GAUGE_WIDGET_H
