#ifndef ECU_DEFINITION_H
#define ECU_DEFINITION_H

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QtXml/QDomDocument>

#include "mutparam.h"
#include "rammut.h"
#include "../map-decl/map.h"

class ecuDefinition
{
public:
    ramMUT ramMut;
    QHash<QString, Map*> RAMtables;

    QString lastError;

    ecuDefinition();
    ~ecuDefinition();

    void reset();

    bool fromFile(QString filename);
    bool fromROMID(QString ROMID);
    QString getFile(QString path, QString CalID);

private:
    QHash<QString, Scaling> scalingsMaps;                     //контейнер скалингов

    void _parser(QIODevice *device);
    void getMUTparam(const QDomElement &element);

    void getLivemap(const QDomElement &element);
    void getScaling(const QDomElement &el);

};

#endif // ECU_DEFINITION_H
