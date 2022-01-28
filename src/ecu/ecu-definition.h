#ifndef ECU_DEFINITION_H
#define ECU_DEFINITION_H

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QtEndian>
#include <QList>
#include <QStack>
#include <QChar>
#include <QMap>
#include <QtXml/QDomDocument>

//#include "../types.h"
#include "mutparam.h"
#include "../map-decl/map.h"
#include "../abstract-memory.h"
#include "../DMA-proto/DMA-proto.h"

typedef struct                                       // Содержимое таблицы
{
    Map *declMap;
    abstractMemoryScaled Map;
    abstractMemoryScaled X_axis;
    abstractMemoryScaled Y_axis;
} mapDefinition;

class ecuDefinition //: public QObject
{
    //Q_OBJECT
public:
    comm_device_interface *devComm = nullptr;
    DMA_proto *ECUproto = nullptr;

    quint32 DEAD_var;
    quint32 RAM_MUT_addr;
    quint16 RAM_MUT_size;
    QVector<mutParam> RAM_MUT;
    QHash<QString, mutParam> RAM_MUTh;

    QHash<QString, Map*> RAMtables;

    ecuDefinition();
    ~ecuDefinition();

    bool fromFile(QString filename);
    bool fromROMID(QString ROMID);
    QString getFile(QString path, QString CalID);

private:
    QHash<QString, Scaling> scaling_qmap;                     //контейнер скалингов

    void _parser(QIODevice *device);
    void getMUTparam(const QDomElement &element);

    void getLivemap(const QDomElement &element);
    void getScaling(const QDomElement &el);

};

#endif // ECU_DEFINITION_H
