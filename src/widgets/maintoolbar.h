#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QToolBar>
#include <QObject>

class mainToolBar : public QToolBar
{
    Q_OBJECT
public:
    mainToolBar(QWidget *parent);

public slots:
    void lockConnect(bool lockFlag);

    void lockReset(bool lockFlag);

private:
    QAction *a_start_action;
    QAction *a_ramReset;

    QAction *a_logger;

    QAction *a_debug_action;

signals:
    void s_connect();
    void s_ramReset();

};

#endif // MAINTOOLBAR_H
