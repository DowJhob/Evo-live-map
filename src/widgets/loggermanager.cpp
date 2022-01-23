#include "loggermanager.h"

loggerManager::loggerManager(QWidget *parent) : QMdiArea{parent}
{
    setDisabled(true);
    tileSubWindows();
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

