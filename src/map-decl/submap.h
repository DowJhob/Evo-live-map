#ifndef SUBMAP_H
#define SUBMAP_H

#include <QtXml/QDomDocument>
#include "scaling.h"

// map or axis
class subMap//:public Scaling
{
public:
    subMap();
    QString Name;                         //Имя таблицы-карты, или оси
    quint32 addr = 0;                     //Адрес таблицы в
    //quint32 ram_addr = 0;
    //quint32 rom_addr = 0;
    int elements = 1;
    bool swapxy = false;
    int ram_mut_number = -1;
    Scaling rom_scaling;
    Scaling RAM_MUT_scaling;

    void fromXML(QDomElement el);
    int byteSize();
    void _setScaling(QHash<QString, Scaling> *scaling);
};

#endif // SUBMAP_H
