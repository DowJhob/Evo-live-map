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
struct sub_tableDeclaration
{
    //   quint32 TagID;
    QString Name;                         //Имя таблицы-карты
    quint32 ram_addr;               //Адрес таблицы в
    quint32 rom_addr;
    int elements = 1;
    bool swapxy = false;
    Scaling scaling;
    //Scaling ram_scaling;
};
struct tableDeclaration             // характеристики карт в памяти контроллера
{
    int tableNum;               //порядковый номер
    sub_tableDeclaration Table;                      //Структура заголовка  таблицы
    sub_tableDeclaration X_axis;                     //Структура заголовка оси
    sub_tableDeclaration Y_axis;                     //Структура заголовка оси
};

class DomParser : public QObject
{
      Q_OBJECT
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
            sc ={};                                                                                      //избавимся от мусора в неиспользуемых полях
            if (node.toElement().tagName() == "scaling")                                                 //сохраним все скалинги
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
                    RAM_MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                if (node.toElement().attribute("name") == "DEAD var")                                    //
                    DEAD_var = node.toElement().attribute("address").toUInt(nullptr, 16);
                if (node.toElement().attribute("name") == "MUT Table")                                   //
                    MUT_addr = node.toElement().attribute("address").toUInt(nullptr, 16);
                if (!node.toElement().attribute("RAM_addr").isEmpty() ||                                 //  лайв таблица или нет
                        !scaling_qmap.value(node.toElement().attribute("scaling")).Patched.isEmpty())    // это таблица с патчем?
                {
                    mainTableDeclaration = {};
                    getTableDeclaration(node, &mainTableDeclaration.Table);                              // сохраняем заголовок таблицы
                    parseEntry(node.toElement());                                                        // парсим оси
                    mainTableDeclaration.tableNum = i;
                    TableDecl_qvector.insert(i, mainTableDeclaration);
                    i++;
                }
            }
            node = node.nextSibling();
        }
    }

    QVector<tableDeclaration> TableDecl_qvector;   //вектор таблиц
    quint32 MUT_addr;
    quint32 DEAD_var;
    quint32 RAM_MUT_addr;

private:
    Scaling sc;                                               //промежуточная структура для помещения в контейнер
    QMap<QString, Scaling> scaling_qmap;                      //контейнер скалингов
    sub_tableDeclaration axisDeclaration;                     //структура оси
    tableDeclaration mainTableDeclaration;                    //структура таблицы
    //mathParser2 mp;                                         //объект матпарсера
    void parseEntry(const QDomElement &element)
    {
        QDomNode node = element.firstChild();
        while (!node.isNull())
        {
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
        bool bStatus = false;
        node = node.toElement();
        _subTableDeclaration->scaling = scaling_qmap.value( node.toElement().attribute("scaling") ); //сохраним скалинг данных таблицы
        _subTableDeclaration->Name = node.toElement().attribute("name");						        // сохраним имя таблицы
        _subTableDeclaration->rom_addr = node.toElement().attribute("address").toUInt(&bStatus,16);  // сохраняем значение ROM адреса
        _subTableDeclaration->ram_addr = node.toElement().attribute("RAM_addr").toUInt(&bStatus,16); //получаем адрес  таблицы в оперативке
        _subTableDeclaration->scaling.ram_mut_number = node.toElement().attribute("RAM_mut_number").toUInt(&bStatus,16); //номер мут запроса из рам мут
        if (node.toElement().attribute("swapxy") == "true")                              //получаем swapxy
            _subTableDeclaration->swapxy = true;
        _subTableDeclaration->scaling.toexpr2 = *set_notation(_subTableDeclaration->scaling.toexpr);
        _subTableDeclaration->scaling.frexpr2 = *set_notation(_subTableDeclaration->scaling.frexpr);
        _subTableDeclaration->elements = node.toElement().attribute("elements").toInt(&bStatus);
    }
};

#endif // DOMPARSER_H
