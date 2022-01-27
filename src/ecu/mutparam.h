#ifndef MUTPARAM_H
#define MUTPARAM_H

#include "../map-decl/scaling.h"

class mutParam
{
public:
    mutParam();
    mutParam(const QDomElement &el);

    Scaling scaling;
    int number;
    int offset;
    void fromXML(const QDomElement &el);
    void setScaling(const Scaling _scaling);

};

#endif // MUTPARAM_H
