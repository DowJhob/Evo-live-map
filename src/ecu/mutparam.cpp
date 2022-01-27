#include "mutparam.h"

mutParam::mutParam()
{

}

mutParam::mutParam(const QDomElement &el)
{
    fromXML(el);
}

void mutParam::fromXML(const QDomElement &el)
{
    //scaling.name = el.attribute("scaling");
    number = el.attribute("number").toInt(nullptr, 16);
}

void mutParam::setScaling(const Scaling _scaling)
{
    scaling = _scaling;
    //offset = scaling.getElementSize() * number;
}
