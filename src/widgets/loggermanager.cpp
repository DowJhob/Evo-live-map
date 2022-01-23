#include "loggermanager.h"

loggerManager::loggerManager(QWidget *parent) : QMdiArea{parent}
{
    setDisabled(true);
    tileSubWindows();
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //QBoxLayout *l = new QBoxLayout(QBoxLayout::Direction::Down, this);


setMaximumSize(1000,800);
    //setLayout(l);
    //l->addWidget(&logMdiArea);
}

