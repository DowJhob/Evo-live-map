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
 //       setMinimumSize(64, 64);
//        setMaximumSize(100, 100);
        lcd.setDigitCount(DigitNum);
       // lcd.setMinimumWidth( this->width() + 1 );
        QFont myFont1 = lcd.font();
        myFont1.setPixelSize (120);
        lcd.setFont(myFont1);
        lcd.setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
        QString s;
        s.fill ('-', DigitNum);
        lcd.display(s);
        QVBoxLayout *lay = new QVBoxLayout(this);
        QLabel *lab = new QLabel(name, this);

        setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
        lay->addWidget(lab );
        lay->addWidget(&lcd);
        setLayout( lay );
    }

public slots:
    void display(float in)
    {
        lcd.display(QString::number(in));
    }
};

#endif // GAUGE_WIDGET_H
