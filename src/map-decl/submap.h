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
    int elements = 1;
    bool swapxy = false;
    int ram_mut_number = -1;
    Scaling rom_scaling;

    void fromXML(QDomElement el);
    int byteSize();

    void _setScaling(QHash<QString, Scaling> *scalingsConteiner);
    void _setMUT_number();
};

#endif // SUBMAP_H
