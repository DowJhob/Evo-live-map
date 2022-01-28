#ifndef MAP_H
#define MAP_H

#include "submap.h"

class Map : public subMap
{
public:
    Map();

    subMap X_axis;                     //Структура заголовка оси
    subMap Y_axis;                     //Структура заголовка оси

    void fromXML(const QDomElement &element);
    void setScaling(QHash<QString, Scaling> *scalingsConteiner);
    void setMUT_number();
    int getMapSize();
    int byteSize();

private:
    void getSubMapDeclaration(QDomElement node, subMap *_subTableDeclaration);

};

#endif // MAP_H
