#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <QWidget>
#include <QMdiArea>
#include <QLayout>
#include <QToolBar>

#include <QGroupBox>

class loggerManager : public QGroupBox
{
    Q_OBJECT
public:
    explicit loggerManager(QWidget *parent = nullptr);

private:
    QGroupBox commonGrpBx;
    QMdiArea logMdiArea;
    QToolBar tb;

signals:

};

#endif // LOGGERMANAGER_H
