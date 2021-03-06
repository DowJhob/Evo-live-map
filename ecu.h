#ifndef ECU_H
#define ECU_H

//#include <QObject>

#include <mathparser2.h>

enum Storagetype { int8, int16, int32, uint8, uint16, uint32 };

typedef struct                          //структура скалингов для помещения в контейнер
{
    Storagetype _storagetype = Storagetype::int8;
    fast_calc_struct toexpr2;
    fast_calc_struct frexpr2;
    QString toexpr;
    QString frexpr;
    bool endian = false;
    int min = 0;
    int max = 0;
    float increment = 0;
    QString Original;
    QString Patched;
} Scaling;
typedef struct
{
    int ram_mut_number = -1;
    QString Name;                         //Имя таблицы-карты, или оси
    quint32 ram_addr = 0;                     //Адрес таблицы в
    quint32 rom_addr = 0;
    int elements = 1;
    bool swapxy = false;
    Scaling rom_scaling;
    Scaling RAM_MUT_scaling;
}sub_tableDeclaration;
typedef struct                                       // характеристики карт в памяти контроллера
{
    sub_tableDeclaration Table;                      //Структура заголовка  таблицы
    sub_tableDeclaration X_axis;                     //Структура заголовка оси
    sub_tableDeclaration Y_axis;                     //Структура заголовка оси
} tableDeclaration;
typedef struct
{
    Scaling ram_mut_param_scaling;
    int ram_mut_offset;
} mutParam;
class ecu
{
public:
    quint32 RAM_MUT_addr;
    quint32 DEAD_var;

    QHash<QString, tableDeclaration> RAMtables;

    QHash<QString, mutParam> RAM_MUT;
    ecu(){}
//public slots:

    qint64 mem_cast(Scaling *scaling, uchar *in_buf, uint offset)   //кастуем данные к определенному типу
    {
        switch (scaling->_storagetype) {
        case Storagetype::int8:
        case Storagetype::uint8:  return type_cast(scaling, in_buf + offset); break;
        case Storagetype::int16:
        case Storagetype::uint16: return type_cast(scaling, in_buf + 2*offset); break;
        case Storagetype::int32:
        case Storagetype::uint32: return type_cast(scaling, in_buf + 4*offset); break;
        default: break;
        }
    }
    qint64 type_cast(Scaling *scaling, uchar *in_buf)          //кастуем данные к определенному типу
    {
        if (scaling->endian)
            switch (scaling->_storagetype) {
            case Storagetype::int8:   return (qint8)in_buf[0];                break;
            case Storagetype::int16:  return qFromBigEndian<qint16>(in_buf);  break;
            case Storagetype::int32:  return qFromBigEndian<qint32>(in_buf);  break;
            case Storagetype::uint8:  return (quint8)in_buf[0];  break;
            case Storagetype::uint16: return qFromBigEndian<quint16>(in_buf); break;
            case Storagetype::uint32: return qFromBigEndian<quint32>(in_buf);break;
            default: break;
            }
        else
            switch (scaling->_storagetype) {
            case Storagetype::int8:   return (qint8)in_buf[0];           break;
            case Storagetype::int16:  return qFromLittleEndian<qint16>(in_buf); break;
            case Storagetype::int32:  return qFromLittleEndian<qint32>(in_buf); break;
            case Storagetype::uint8:  return (quint8)in_buf[0];      break;
            case Storagetype::uint16: return qFromLittleEndian<quint16>(in_buf); break;
            case Storagetype::uint32: return qFromLittleEndian<quint32>(in_buf); break;
            default: break;
            }
    }
    float mut_cast( uchar* in_buf, Scaling *scaling, int mut_offset )
    {
        float x = type_cast(scaling, in_buf + mut_offset );
        x = fast_calc(scaling->toexpr2, x);
        return x;
    }
};

#endif // ECU_H
