#include "loggermanager.h"

loggerManager::loggerManager(QWidget *parent) : QGroupBox{parent}
{
    setDisabled(true);
    //tileSubWindows();
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QBoxLayout *l = new QBoxLayout(QBoxLayout::Direction::Down, this);
    setLayout(l);
    //l->addWidget(&commonGrpBx);
    l->setMenuBar(&tb);
    //l = new QBoxLayout(QBoxLayout::Direction::Down, this);
    //commonGrpBx.setLayout(l);
tb.setStyleSheet("QToolBar {border-bottom: 1px solid lightgray;}");
    //l->addWidget(&tb);
   // l->addWidget(&logMdiArea);
//logMdiArea.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    //logMdiArea.setMaximumSize(1000,800);
    //QAction *a = new QAction;
    //a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &ecuManager::startAction);
    tb.addAction(QIcon( ":ico/connect.png" ), "Start");
}
