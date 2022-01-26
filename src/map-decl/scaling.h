#ifndef SCALING_H
#define SCALING_H

#include <QDebug>
#include <QtXml/QDomDocument>
#include "../fast-notation.h"

enum Storagetype
{
    undef,
    bloblist,
    int8,
    int16,
    int32,
    uint8,
    uint16,
    uint32
};

class Scaling
{
public:
    Scaling();
    Scaling(const QDomElement &el);                        //структура скалингов для помещения в контейнер

    QString name;
    Storagetype _storagetype = Storagetype::undef;
    fast_notation toexpr2;
    fast_notation frexpr2;
    QString toexpr;
    QString frexpr;
    QString format;
    bool endian = false;
    float min = 0.0;
    float max = 0.0;
    float increment = 0;
    QString Original;
    QString Patched;

    void fromXML(const QDomElement &el);

    int getElementSize();

    void setFastNotation();

private:


};

#endif // SCALING_H
