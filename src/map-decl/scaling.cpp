#include "scaling.h"

Scaling::Scaling()
{

}

Scaling::Scaling(const QDomElement &el)
{
    fromXML(el);
}

void Scaling::fromXML(const QDomElement &el)
{
    name = el.attribute("name");
    QString storagetype = el.attribute("storagetype");
    if (storagetype == "int8")
        _storagetype = Storagetype::int8;
    else if (storagetype == "int16")
        _storagetype = Storagetype::int16;
    else if (storagetype == "int32")
        _storagetype = Storagetype::int32;
    else if (storagetype == "uint8")
        _storagetype = Storagetype::uint8;
    else if (storagetype == "uint16")
        _storagetype = Storagetype::uint16;
    else if (storagetype == "uint32")
        _storagetype = Storagetype::uint32;
    else if (storagetype == "bloblist")
    {
        getBloblist(el);    //пропарсим вложенные тэги патча и оригинала
        return;
    }
    //else
    {
        format = el.attribute("format");
        increment = el.attribute("inc").toFloat(nullptr);
        min = el.attribute("min").toInt(nullptr);
        max = el.attribute("max").toInt(nullptr);
        toexpr = el.attribute("toexpr");
        frexpr = el.attribute("frexpr");
        if (el.attribute("endian") == "big")
            endian = true;
    }
    setFastNotation();
}

int Scaling::getElementSize()
{
    switch (_storagetype) {
    case Storagetype::int8:
    case Storagetype::uint8:  return 1; break;
    case Storagetype::int16:
    case Storagetype::uint16: return 2; break;
    case Storagetype::int32:
    case Storagetype::uint32: return 4; break;
    default: return 1; break;
    }
}

void Scaling::setFastNotation()
{
    frexpr2.setFastNotation(frexpr);
    toexpr2.setFastNotation(toexpr);
}

void Scaling::getBloblist(const QDomElement &element)
{
    _storagetype = Storagetype::bloblist;

    QDomNode node = element.firstChild();
    while (!node.isNull())
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "data")                                       //находим блобы
        {
            if (el.attribute("name") == "Original")
                Original = QByteArray::fromHex(el.attribute("value").toUtf8());
            else if (el.attribute("name") == "Patched")
                Patched = QByteArray::fromHex(el.attribute("value").toUtf8());
        }
    }
}
