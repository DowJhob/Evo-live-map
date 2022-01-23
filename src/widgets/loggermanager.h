#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <QWidget>
#include <QMdiArea>

class loggerManager : public QMdiArea
{
    Q_OBJECT
public:
    explicit loggerManager(QWidget *parent = nullptr);

private:
    //QMdiArea logMdiArea;

signals:

};

#endif // LOGGERMANAGER_H
