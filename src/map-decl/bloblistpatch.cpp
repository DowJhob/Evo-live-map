#include "bloblistpatch.h"

bloblistPatch::bloblistPatch(QObject *parent) : QObject{parent}
{}

bloblistPatch::bloblistPatch(const QDomElement &element)
{
    Name = element.attribute("name").toUtf8();
    addr = element.attribute("address").toUInt();
    scaling.name = element.attribute("scaling").toUtf8();
}
