#include "ecu-definition.h"

ecu_definition::ecu_definition(){}

ecu_definition::~ecu_definition(){
    for(auto c : qAsConst(RAMtables))
    {
        delete c;
    }
}

bool ecu_definition::fromFile(QString filename)
{
    // Открываем конфиг:
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    _parser(file);        // парсим файл
    delete file;
    return true;
}

void ecu_definition::_parser(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    //открываем документ
    if (!doc.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        qWarning("Line %d, column %d: %s", errorLine, errorColumn, "errorStr.ascii()");
        return;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "rom") {
        qWarning("The file is not a rom xml");
        return;
    }
    QDomNode node = root.firstChild();
    while (!node.isNull())
    {
        const QDomElement el = node.toElement();
        if (el.tagName() == "scaling")                                                 //сохраним все скалинги
            getScaling(el);
        if (el.tagName() == "table")                                                   // находим таблицу
        {
            if (!el.attribute("RAM_addr").isEmpty() )
                getLivemap(el);
            QString nodeName = el.attribute("name");
            if (nodeName == "RAM_MUT")                                     //
            {
                RAM_MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                DEAD_var = node.toElement().attribute("DEAD_var").toUInt(nullptr, 16);
                getMUTparam(node.toElement());
            }
            if (nodeName == "DEAD var")                                    //
                DEAD_var = el.attribute("address").toUInt(nullptr, 16);
        }
        node = node.nextSibling();
    }
    // проставим скалинги
    for(auto c : qAsConst(RAMtables))
    {
        c->setScaling(&scaling_qmap);
    }

}

void ecu_definition::getMUTparam(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull())
    {
        QDomElement el = node.toElement();
        if (el.tagName() == "ram_mut") //находим параметры мут таблицы
        {
            mutParam _mut_param;
            _mut_param.scaling = scaling_qmap.value(el.attribute("scaling"));

            _mut_param.number = el.attribute("number").toInt(nullptr, 16);
            ++RAM_MUT_count;
            RAM_MUT.insert(_mut_param.number, _mut_param );
        }
        node = node.nextSibling();
    }
}


void ecu_definition::getLivemap(const QDomElement &element)
{
    Map *mainTableDeclaration = new Map;
    mainTableDeclaration->fromXML(element);
    RAMtables.insert(mainTableDeclaration->Name, mainTableDeclaration);
}

void ecu_definition::getScaling(const QDomElement &el)
{
    Scaling sc(el);
    scaling_qmap.insert(sc.name, sc);
}
