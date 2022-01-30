#ifndef MUTPARAM_H
#define MUTPARAM_H

#include "../map-decl/scaling.h"

class mutParam
{
public:
    //float scaledValue;
    Scaling scaling;
    int number;
    int offset;

    mutParam();
    mutParam(const QDomElement &el);

    void fromXML(const QDomElement &el);
    void setScaling(const Scaling _scaling);

};

#endif // MUTPARAM_H
