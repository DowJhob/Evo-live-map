#ifndef ECU_H
#define ECU_H

#include <QObject>

#include <mathparser2.h>


enum Storagetype { int8, int16, int32, uint8, uint16, uint32 };

struct Scaling                          //структура скалингов для помещения в контейнер
{
    Storagetype _storagetype = Storagetype::int8;
    fast_calc_struct toexpr2;
    fast_calc_struct frexpr2;
    QString toexpr;
    QString frexpr;
    bool endian = false;
    QString Original;
    QString Patched;
};
struct sub_tableDeclaration
{
    int ram_mut_number = -1;
    QString Name;                         //Имя таблицы-карты, или оси
    quint32 ram_addr;                     //Адрес таблицы в
    quint32 rom_addr;
    int elements = 1;
    bool swapxy = false;
    Scaling rom_scaling;
    Scaling RAM_MUT_scaling;
};
struct tableDeclaration             // характеристики карт в памяти контроллера
{
    int tableNum;               //порядковый номер
    sub_tableDeclaration Table;                      //Структура заголовка  таблицы
    sub_tableDeclaration X_axis;                     //Структура заголовка оси
    sub_tableDeclaration Y_axis;                     //Структура заголовка оси
};
struct mutParam
{
    Scaling ram_mut_param_scaling;
    int ram_mut_offset;
};
class ecu
{
public:
    quint32 RAM_MUT_addr;
    quint32 DEAD_var;
    QHash<QString, tableDeclaration> not_loggingRAMtables;
    QHash<QString, tableDeclaration> loggingRAMtables;
    QHash<QString, mutParam> RAM_MUT;
    ecu();
};

#endif // ECU_H
