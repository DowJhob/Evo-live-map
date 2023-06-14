#ifndef BLOBLISTPATCH_H
#define BLOBLISTPATCH_H

#include <QObject>
#include "scaling.h"

class bloblistPatch : public QObject
{
    Q_OBJECT
public:

    Scaling scaling;
    quint32 addr = 0;

    QString Name;

 bloblistPatch(QObject *parent = nullptr);;
 bloblistPatch(const QDomElement &element);

signals:

};

#endif // BLOBLISTPATCH_H
