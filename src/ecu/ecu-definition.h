#ifndef ECU_DEFINITION_H
#define ECU_DEFINITION_H

#include <QFile>
#include <QFileDialog>

#include <QtXml/QDomDocument>
#include <src/abstract-memory.h>

#include <QtEndian>
#include <QList>
#include <QStack>
#include <QChar>
#include <QMap>
#include <QDebug>



#include <math.h>
#include "../types.h"
#include "../map-decl/map.h"


typedef struct                                       // Содержимое таблицы
{
    Map *declMap;
    abstractMemoryScaled Map;
    abstractMemoryScaled X_axis;
    abstractMemoryScaled Y_axis;
} mapDefinition;

class ecu_definition
{
public:
    quint32 DEAD_var;
    quint32 RAM_MUT_addr;
    QVector<mutParam> RAM_MUT;
    quint32 RAM_MUT_count = 0;

    QHash<QString, Map*> RAMtables;

    ecu_definition();
    ~ecu_definition();
public:
    bool fromFile(QString filename);
    //bool fromROMID(QString ROMID);
    QString SearchFiles(QString path, QString CalID);

private:
    QHash<QString, Scaling> scaling_qmap;                     //контейнер скалингов

    void _parser(QIODevice *device);
    void getMUTparam(const QDomElement &element);

    void getLivemap(const QDomElement &element);
    void getScaling(const QDomElement &el);
};

#endif // ECU_DEFINITION_H
