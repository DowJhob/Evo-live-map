#ifndef READ_REQUEST_H
#define READ_REQUEST_H


#include <QByteArray>

class readRequest:QByteArray
{
public:
    readRequest(quint32 addr, int lenght,QByteArray *dest):addr(addr), lenght(lenght)
    {

    }
    quint32 addr;
    int lenght;
    void readyRead()
    {

    }
};

#endif // READ_REQUEST_H
