#include "submap.h"

subMap::subMap()
{

}

void subMap::fromXML(QDomElement el)
{
    Name = el.attribute("name");			        			        // сохраним имя таблицы
    QString atr = el.attribute("RAM_addr");
    if(!atr.isEmpty())
        addr = atr.toUInt(nullptr, 16);                                // сохраняем значение ROM адреса
    else
        addr = el.attribute("address").toUInt(nullptr, 16);
    QString RAM_mut_number = el.attribute("RAM_mut_number");
    if( !RAM_mut_number.isEmpty() )
        ram_mut_number = RAM_mut_number.toUInt(nullptr, 16);           //номер мут запроса из рам мут
    if (el.attribute("swapxy") == "true")                              //получаем swapxy
        swapxy = true;
    elements = el.attribute("elements").toInt(nullptr);


    rom_scaling.name =  el.attribute("scaling") ; //сохраним скалинг данных таблицы
    //RAM_MUT_scaling.name =  el.attribute("ram_mut_scaling") ; //сохраним RAM скалинг данных логгера
}

int subMap::byteSize()
{
    return elements * rom_scaling.getElementSize();
}

void subMap::_setScaling(QHash<QString, Scaling> *scalingsConteiner)
{
    rom_scaling = scalingsConteiner->value( rom_scaling.name );
    //RAM_MUT_scaling = scalingsConteiner->value( RAM_MUT_scaling.name );
}

void subMap::_setMUT_number()
{
    ram_mut_number = 0;
}
