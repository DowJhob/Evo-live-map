#include "gauge_widget.h"

gaugeWidget::gaugeWidget(QString name, uint DigitNum, QWidget *parent):QWidget(parent)
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
    QActionGroup *group = new QActionGroup(m_pmnu);
    group->setExclusive(true);

    QAction* pAction = m_pmnu->addAction("Innovate");

    //m_pmnu->setDefaultAction(pAction);

    group->addAction(pAction);
    pAction->setCheckable(true);
    pAction->setChecked(true);
    pAction = m_pmnu->addAction("PLX");
    group->addAction(pAction);
    pAction->setCheckable(true);
    pAction = m_pmnu->addAction("AEM");
    group->addAction(pAction);
    pAction->setCheckable(true);


    connect(group, &QActionGroup::triggered, this, &gaugeWidget::setProto);
    setToolTip("Press right mouse button for set wideband proto");
}

void gaugeWidget::contextMenuEvent(QContextMenuEvent *pe)
{
    m_pmnu->exec(pe->globalPos());
}

void gaugeWidget::display(float in)
{
    lcd.display(QString::number(in));
}

void gaugeWidget::setProto(QAction* pAction)
{
    QString wbProto = pAction->text();
    qDebug() << "wbProto" << wbProto;
}
