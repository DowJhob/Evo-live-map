#include "gauge_widget.h"

gauge_widget::gauge_widget(QString name, uint DigitNum, QWidget *parent):QWidget(parent)
{
    //       setMinimumSize(64, 64);
    //        setMaximumSize(100, 100);
    lcd.setDigitCount(DigitNum);
    lcd.resize(QSize(400, 200));
    // lcd.setMinimumWidth( this->width() + 1 );
    QFont myFont1 = lcd.font();
    //myFont1.setPixelSize (120);
    //lcd.setFont(myFont1);
    QString s;
    s.fill ('-', DigitNum);
    lcd.display(s);
    QGridLayout *lay = new QGridLayout(this);
    QLabel *lab = new QLabel(name, this);
    lay->addWidget(lab );
    lay->addWidget(&lcd);
    lab->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

    lcd.setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );


    setLayout( lay );
    //lay->setSizeConstraint(
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );

    resize(200, 200);

    m_pmnu = new QMenu(this);
    QAction* pAction = m_pmnu->addAction("Innovate");
    pAction->setCheckable(true);
    pAction = m_pmnu->addAction("PLX");
    pAction->setCheckable(true);
    pAction = m_pmnu->addAction("AEM");
    pAction->setCheckable(true);
    connect(m_pmnu, &QMenu::triggered, this, &gauge_widget::setProto);
    setToolTip("Press right mouse button for set wideband proto");
}

void gauge_widget::contextMenuEvent(QContextMenuEvent *pe)
{
    m_pmnu->exec(pe->globalPos());
}

void gauge_widget::display(float in)
{
    lcd.display(QString::number(in));
}

void gauge_widget::setProto(QAction* pAction)
{
QString strColor = pAction->text();
}
