#ifndef WBPROTO_H
#define WBPROTO_H

#include <QObject>
#include <QByteArray>

class wbProto : public QObject
{
    Q_OBJECT
public:
    int baudRate = 0;
    virtual QString handleWB(QByteArray a) = 0;

private:

signals:
//    void logReady(QString);
};

#endif // WBPROTO_H
