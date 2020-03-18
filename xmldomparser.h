#ifndef DOMPARSER_H
#define DOMPARSER_H
#include <QtXml/QDomDocument>
//#include <QtWidgets>
#include <mathparser2.h>
//#include <qtablewidget.h>

#include <math.h>


struct Scaling                          //структура скалингов для помещения в контейнер
{
    QString storagetype;
    fast_calc_struct toexpr2;
    fast_calc_struct frexpr2;
    QString toexpr;
    QString frexpr;
    bool endian;
    quint32 ram_mut_number;
    QString Original;
    QString Patched;
};
struct xml_table
{
    //   quint32 TagID;
    QString Name;                         //Имя таблицы-карты
    quint32 ram_addr;               //Адрес таблицы в
    quint32 rom_addr;
    uint elements;
    bool swapxy;
    Scaling scaling;
    Scaling ram_scaling;

};
struct TableProperty_fr_xml             // характеристики карт в памяти контроллера
{
    int tableNum;               //порядковый номер
    xml_table Table;                      //Структура заголовка  таблицы
    xml_table X_axis;                     //Структура заголовка оси
    xml_table Y_axis;                     //Структура заголовка оси
};

class DomParser
{
    //  Q_OBJECT
public:
    //mathParser2 *m;
    DomParser(QIODevice *device)
    {
        //this->m = m;
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
        int i = 0;
        while (!node.isNull())
        {
            sc ={};                                                                      //избавимся от мусора в неиспользуемых полях
            if (node.toElement().tagName() == "scaling")                                 //сохраним все скалинги
            {
                QString name = node.toElement().attribute("name");

                sc.storagetype = node.toElement().attribute("storagetype");
                if (sc.storagetype == "bloblist")
                {
                    parseEntry(node.toElement());    //пропарсим вложенные тэги патча и оригинала
                }
                else
                {
                    sc.toexpr = node.toElement().attribute("toexpr");
                    sc.frexpr = node.toElement().attribute("frexpr");

                    if (node.toElement().attribute("endian") == "big")
                        sc.endian = true;
                    else
                        if (node.toElement().attribute("endian") == "little")
                            sc.endian = false;
                        else
                            sc.endian = true;
                }
                scaling_qmap.insert(name, sc);
            }
            if (node.toElement().tagName() == "table")                                                   // находим таблицу
            {
                if (node.toElement().attribute("name") == "RAM_MUT")                                     //
                {
                    RAM_MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                }
                if (node.toElement().attribute("name") == "DEAD var")                                    //
                {
                    DEAD_var = node.toElement().attribute("address").toUInt(nullptr, 16);
                }
                if (node.toElement().attribute("name") == "MUT Table")                                   //
                {
                    MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                }
                if (!node.toElement().attribute("RAM_addr").isEmpty() ||                                 //  лайв таблица или нет
                        !scaling_qmap.value(node.toElement().attribute("scaling")).Patched.isEmpty())    // это таблица с патчем?
                {
                    TableDecl = {};
                    SetTableDecl(node);                                                  // сохраняем заголовок таблицы
                    TableDecl.Table = Table;
                    parseEntry(node.toElement());                                        // парсим оси
                    TableDecl.tableNum = i;
                    TableDecl_qvector.insert(i, TableDecl);
                    i++;
                }

            }
            node = node.nextSibling();
        }
    }

    QVector<TableProperty_fr_xml> TableDecl_qvector;   //вектор таблиц
    quint32 MUT_addr;
    quint32 DEAD_var;
    quint32 RAM_MUT_addr;

private:
    Scaling sc;                                               //промежуточная структура для помещения в контейнер
    QMap<QString, Scaling> scaling_qmap;                      //контейнер скалингов
    xml_table Table;                                          //структура оси
    TableProperty_fr_xml TableDecl;                           //структура таблиц
    //mathParser2 mp;                                         //объект матпарсера
    void parseEntry(const QDomElement &element)
    {
        QDomNode node = element.firstChild();
        while (!node.isNull())
        {
            if (node.toElement().tagName() == "table") //находим ось
            {
                if (node.toElement().attribute("type") == "X Axis")
                {
                    SetTableDecl(node);         // сохраняем заголовок таблицы
                    TableDecl.X_axis = Table;
                }
                if (node.toElement().attribute("type") == "Y Axis")
                {
                    SetTableDecl(node);         // сохраняем заголовок таблицы
                    TableDecl.Y_axis = Table;
                }
            }
            if (node.toElement().tagName() == "data") //находим патч
            {
                if (node.toElement().attribute("name") == "Original")
                {
                    sc.Original = node.toElement().attribute("value");
                }
                if (node.toElement().attribute("name") == "Patched")
                {
                    sc.Patched = node.toElement().attribute("value");
                }
            }
            node = node.nextSibling();
        }
    }

    void SetTableDecl(QDomNode node)  // сохраняем заголовок таблицы
    {
        Table = {};
        bool bStatus = false;

        node = node.toElement();
        Table.Name = node.toElement().attribute("name");						// сохраним имя таблицы
        //-------------------------------------------// сохраняем значение ROM адреса
        Table.rom_addr = node.toElement().attribute("address").toUInt(&bStatus,16);

        //-------------------------------------------//получаем адрес  таблицы в оперативке
        Table.ram_addr = node.toElement().attribute("RAM_addr").toUInt(&bStatus,16);
        //-------------------------------------------//
        Table.ram_scaling.ram_mut_number = node.toElement().attribute("RAM_mut_number").toUInt(&bStatus,16); //номер мут запроса из рам мут
        Table.ram_scaling.storagetype = node.toElement().attribute("RAM_mut_storagetype");
        Table.ram_scaling.frexpr = node.toElement().attribute("RAM_mut_frexpr");

        if (node.toElement().attribute("RAM_mut_endian") == "big")
            Table.ram_scaling.endian = true;
        else
            if (node.toElement().attribute("RAM_mut_endian") == "little")
                Table.ram_scaling.endian = false;
            else
                Table.ram_scaling.endian = true;

        //-------------------------------------------//получаем swapxy

        if (node.toElement().attribute("swapxy") == "true")
        {
            Table.swapxy =   true;
        }
        else
        {
            Table.swapxy = false;
        }

        Table.scaling = scaling_qmap.value(node.toElement().attribute("scaling")) ;//Получаем скалинг данных таблицы

        Table.scaling.toexpr2 = //*m->
                *set_notation(Table.scaling.toexpr);

        Table.scaling.frexpr2 = //*m->
                *set_notation(Table.scaling.frexpr);

        Table.elements = node.toElement().attribute("elements").toUInt(&bStatus);

    }

};

#endif // DOMPARSER_H
