#ifndef MAP_REQUEST_H
#define MAP_REQUEST_H

#include <QObject>
#include "read-request.h"

class mapRequest:public QObject //readRequest
{
    Q_OBJECT
public:
    mapRequest();
    //readRequest X_axis;
    //readRequest Y_axis;
};

#endif // MAP-REQUEST_H
