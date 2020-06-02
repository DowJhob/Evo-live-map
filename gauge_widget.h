#ifndef GAUGE_WIDGET_H
#define GAUGE_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <ecu.h>
#include <QLCDNumber>

class gauge_widget:public QWidget
{
    Q_OBJECT
public:
    uint offset;
    Scaling *scaling;
    QLCDNumber lcd;
    gauge_widget(QString name, uint DigitNum = 4, uint offset = 0, Scaling *scaling = nullptr, QWidget* parent = nullptr):QWidget(parent), offset(offset), scaling(scaling)
    {
        lcd.setDigitCount(DigitNum);
        lcd.setMinimumWidth( this->width() + 1 );
        QFont myFont1 = lcd.font();
        myFont1.setPixelSize (64);
        lcd.setFont(myFont1);
        //lcd.setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        QString s;
        s.fill ('-', DigitNum);
        lcd.display(s);
        QVBoxLayout *lay = new QVBoxLayout(this);
        lay->addWidget( new QLabel(name, this));
        lay->addWidget(&lcd);
        setLayout( lay );
    }

public slots:
    void display(QString s)
    {
        lcd.display(s);
    }
};

#endif // GAUGE_WIDGET_H
