#ifndef ECU_DEFINITION_H
#define ECU_DEFINITION_H

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QTranslator>

#include "mutparam.h"
#include "rammut.h"
#include "../map-decl/map.h"
#include "src/xmlmanager.h"

class ecuDefinition : public xmlManager
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

//    void _parser(QIODevice *device, QString includeID);
    void getMUTparam(const QDomElement &element);

    void getLivemap(const QDomElement &element);
    void getScaling(const QDomElement &el);

//    QDomElement getXMLDom(QIODevice *device);


    void serialize(QDomElement el);

    void subSerialize(QDomNode node);


};

#endif // ECU_DEFINITION_H
