#include "gauge_widget.h"

gauge_widget::gauge_widget(QString name, uint DigitNum, uint offset, Scaling *scaling, QWidget *parent):QWidget(parent), offset(offset), scaling(scaling)
{
    //       setMinimumSize(64, 64);
    //        setMaximumSize(100, 100);
    lcd.setDigitCount(DigitNum);
    // lcd.setMinimumWidth( this->width() + 1 );
    QFont myFont1 = lcd.font();
    myFont1.setPixelSize (120);
    lcd.setFont(myFont1);
    lcd.setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
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

void gauge_widget::display(float in)
{
    lcd.display(QString::number(in));
}
