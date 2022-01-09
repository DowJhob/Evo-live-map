#include "maintoolbar.h"

mainToolBar::mainToolBar(QWidget *parent):QToolBar(parent)
{
    //=============================================================================
    a_start_action = addAction( QIcon( ":ico/connect.png" ), "Start", this, &mainToolBar::s_connect);
    a_start_action->setDisabled(true);
    addSeparator();
    a_ramReset = addAction(QIcon( ":ico/Memory-Freer-icon.png" ), "RAM refresh", this, &mainToolBar::s_ramReset);
    a_ramReset->setDisabled(true);
    //a_logger = addAction( "Start", this, &MainWindow::logger_slot);
    //a_logger->setDisabled(true);

    addSeparator();
    //QWidget* empty = new QWidget(this);
    //empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    //addWidget(empty);
    //debug_action = ui->toolBar->addAction(QIcon( ":ico/screwdriver.png" ), "Debug", this, &MainWindow::debugButton_slot);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    setIconSize(QSize(200, 200));
}

void mainToolBar::lockConnect(bool lockFlag)
{
    a_start_action->setDisabled(lockFlag);
    if(lockFlag)
    {
        a_ramReset->setDisabled(lockFlag);
    }
}

void mainToolBar::lockReset(bool lockFlag)
{
    a_ramReset->setDisabled(lockFlag);
}
