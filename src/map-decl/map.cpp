#include "map.h"



Map::Map()
{

}

void Map::fromXML(const QDomElement &element)
{
    //Map mainTableDeclaration;
    getSubMapDeclaration(element, this);                              // сохраняем заголовок таблицы

    QDomNode node = element.firstChild();
    while (!node.isNull())                                                       // парсим оси
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "table") //находим ось
        {
            if (el.attribute("type") == "X Axis")
                getSubMapDeclaration(el, &X_axis);         // сохраняем заголовок субтаблицы - оси
            if (el.attribute("type") == "Y Axis")
                getSubMapDeclaration(el, &Y_axis);         // сохраняем заголовок субтаблицы - оси
            elements = X_axis.elements * Y_axis.elements;
        }
        node = node.nextSibling();
    }
    //        RAMtables.insert(mainTableDeclaration.Name, mainTableDeclaration);
}

void Map::setScaling(QHash<QString, Scaling> *scaling)
{
    _setScaling(scaling);
    X_axis._setScaling( scaling);
    Y_axis._setScaling( scaling);
}

int Map::getMapSize()
{
    return X_axis.elements * Y_axis.elements;
}

int Map::byteSize()
{
    return getMapSize() * rom_scaling.getElementSize();
}

void Map::getSubMapDeclaration(QDomElement node, subMap *_subTableDeclaration)  // сохраняем заголовок таблицы
{
    QDomElement el = node.toElement();
    _subTableDeclaration->fromXML(el);

}
