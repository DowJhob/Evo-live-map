#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLCDNumber>

class gauge_widget:public QLCDNumber
{
    Q_OBJECT
public:
    gauge_widget(uint DigitNum = 4, QWidget* parent = nullptr):QLCDNumber(parent)
    {
        setDigitCount(DigitNum);
        setMinimumWidth(this->width()+1);
        QFont myFont1 = this->font();
        myFont1.setPixelSize (64);
        setFont(myFont1);
        setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        QString s;
        s.fill ('-', DigitNum);
        display(s);
    }
};

#endif // GAUGE_WIDGET_H
