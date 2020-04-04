#ifndef DOMPARSER_H
#define DOMPARSER_H
#include <QtXml/QDomDocument>
#include <mathparser2.h>
#include "ecu.h"
#include <math.h>

class DomParser : public QObject
{
    Q_OBJECT
public:

    DomParser()
    {}
    void _parser(QIODevice *device, ecu *_ecu, QTreeWidget *tr)
    {
        this->_ecu = _ecu;
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
            sc ={};                                                                                      //избавимся от мусора в неиспользуемых полях
            if (node.toElement().tagName() == "scaling")                                                 //сохраним все скалинги
            {
                QString name = node.toElement().attribute("name");
                QString storagetype = node.toElement().attribute("storagetype");
                if (storagetype == "bloblist")
                {
                    parseEntry(node.toElement());    //пропарсим вложенные тэги патча и оригинала
                }
                else
                {
                    sc.increment = node.toElement().attribute("inc").toFloat(nullptr);
                    sc.min = node.toElement().attribute("min").toInt(nullptr, 16);
                    sc.max = node.toElement().attribute("max").toInt(nullptr, 16);
                    sc.toexpr = node.toElement().attribute("toexpr");
                    sc.frexpr = node.toElement().attribute("frexpr");
                    if (node.toElement().attribute("endian") == "big")
                        sc.endian = true;
                    if (storagetype == "int8")
                        sc._storagetype = Storagetype::int8;
                    if (storagetype == "int16")
                        sc._storagetype = Storagetype::int16;
                    if (storagetype == "int32")
                        sc._storagetype = Storagetype::int32;
                    if (storagetype == "uint8")
                        sc._storagetype = Storagetype::uint8;
                    if (storagetype == "uint16")
                        sc._storagetype = Storagetype::uint16;
                    if (storagetype == "uint32")
                        sc._storagetype = Storagetype::uint32;
                }
                scaling_qmap.insert(name, sc);
            }
            if (node.toElement().tagName() == "table")                                                   // находим таблицу
            {
                QString nodeName = node.toElement().attribute("name");
                if (nodeName == "RAM_MUT")                                     //
                {
                    _ecu->RAM_MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                    _ecu->DEAD_var = node.toElement().attribute("DEAD_var").toUInt(nullptr, 16);
                    parseEntry(node.toElement());
                }
                if (nodeName == "DEAD var")                                    //
                    _ecu->DEAD_var = node.toElement().attribute("address").toUInt(nullptr, 16);
                if (!node.toElement().attribute("RAM_addr").isEmpty() ||                                 //  лайв таблица или
                        !scaling_qmap.value(node.toElement().attribute("scaling")).Patched.isEmpty())    // это таблица с патчем?
                {
                    mainTableDeclaration = {};
                    getTableDeclaration(node, &mainTableDeclaration.Table);                              // сохраняем заголовок таблицы
                    parseEntry(node.toElement());                                                        // парсим оси

                    _ecu->RAMtables.insert(mainTableDeclaration.Table.Name, mainTableDeclaration);
                    //                    if ( mainTableDeclaration.X_axis.ram_addr || mainTableDeclaration.X_axis.ram_addr )
                    //                        _ecu->loggingRAMtables.insert(mainTableDeclaration.Table.Name, mainTableDeclaration);
                    //                    else
                    //                        _ecu->not_loggingRAMtables.insert(mainTableDeclaration.Table.Name, mainTableDeclaration);
                }
            }
            node = node.nextSibling();
        }
    }

private:
    ecu *_ecu;
    Scaling sc;                                               //промежуточная структура для помещения в контейнер
    QHash<QString, Scaling> scaling_qmap;                      //контейнер скалингов
    sub_tableDeclaration axisDeclaration;                     //структура оси
    tableDeclaration mainTableDeclaration;                    //структура таблицы

    void parseEntry(const QDomElement &element)
    {
        QDomNode node = element.firstChild();
        while (!node.isNull())
        {
            if (node.toElement().tagName() == "ram_mut") //находим параметры мут таблицы
            {
                mutParam _mut_param;
                _mut_param.ram_mut_param_scaling = scaling_qmap.value(node.toElement().attribute("scaling"));
                _mut_param.ram_mut_offset = node.toElement().attribute("offset").toInt(nullptr, 16);
                _ecu->RAM_MUT.insert( node.toElement().attribute("name"), _mut_param );
            }
            if (node.toElement().tagName() == "table") //находим ось
            {
                if (node.toElement().attribute("type") == "X Axis")
                    getTableDeclaration(node, &mainTableDeclaration.X_axis);         // сохраняем заголовок субтаблицы - оси
                if (node.toElement().attribute("type") == "Y Axis")
                    getTableDeclaration(node, &mainTableDeclaration.Y_axis);         // сохраняем заголовок субтаблицы - оси
            }
            if (node.toElement().tagName() == "data") //находим патч
            {
                if (node.toElement().attribute("name") == "Original")
                    sc.Original = node.toElement().attribute("value");
                if (node.toElement().attribute("name") == "Patched")
                    sc.Patched = node.toElement().attribute("value");
            }
            node = node.nextSibling();
        }
    }
    void getTableDeclaration(QDomNode node, sub_tableDeclaration *_subTableDeclaration)  // сохраняем заголовок таблицы
    {
        node = node.toElement();
        _subTableDeclaration->rom_scaling = scaling_qmap.value( node.toElement().attribute("scaling") ); //сохраним скалинг данных таблицы
        _subTableDeclaration->Name = node.toElement().attribute("name");						        // сохраним имя таблицы
        _subTableDeclaration->rom_addr = node.toElement().attribute("address").toUInt(nullptr, 16);  // сохраняем значение ROM адреса
        _subTableDeclaration->ram_addr = node.toElement().attribute("RAM_addr").toUInt(nullptr, 16); //получаем адрес  таблицы в оперативке
        if (node.toElement().attribute("swapxy") == "true")                              //получаем swapxy
            _subTableDeclaration->swapxy = true;
        _subTableDeclaration->rom_scaling.toexpr2 = *set_notation(_subTableDeclaration->rom_scaling.toexpr);
        _subTableDeclaration->rom_scaling.frexpr2 = *set_notation(_subTableDeclaration->rom_scaling.frexpr);

        _subTableDeclaration->RAM_MUT_scaling = scaling_qmap.value( node.toElement().attribute("ram_mut_scaling") ); //сохраним RAM скалинг данных логгера
        _subTableDeclaration->RAM_MUT_scaling.toexpr2 = *set_notation(_subTableDeclaration->RAM_MUT_scaling.toexpr);
        _subTableDeclaration->RAM_MUT_scaling.frexpr2 = *set_notation(_subTableDeclaration->RAM_MUT_scaling.frexpr);

        QString RAM_mut_number = node.toElement().attribute("RAM_mut_number");
        if( !RAM_mut_number.isEmpty() )
            _subTableDeclaration->ram_mut_number = RAM_mut_number.toUInt(nullptr, 16); //номер мут запроса из рам мут

        _subTableDeclaration->elements = node.toElement().attribute("elements").toInt(nullptr);
    }
};

#endif // DOMPARSER_H
