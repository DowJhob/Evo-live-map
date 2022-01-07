#include "abstract-memory.h"

abstractMemoryScaled::abstractMemoryScaled()
{}

abstractMemoryScaled::abstractMemoryScaled(QByteArray a):abstractMemory(a)
{
}

abstractMemoryScaled::abstractMemoryScaled(Scaling *scaling, float value2):abstractMemory(fromFloat(scaling, value2))
{}

QByteArray abstractMemoryScaled::fromFloat(Scaling *scaling, float value2)
{
    quint64 value = qRound64(scaling->frexpr2.fast_calc(value2));

    QByteArray a(8, 0);
    int size=1;
    if (scaling->endian) {
        switch (scaling->_storagetype) {
        case Storagetype::int8:   size = 1;         a.data()[0] = value;                             break;
        case Storagetype::int16:  size = 2; qToBigEndian<qint16>(value, a.data());  break;
        case Storagetype::int32:  size = 4; qToBigEndian<qint32>(value, a.data());  break;
        case Storagetype::uint8:  size = 1;           a.data()[0] = value;                            break;
        case Storagetype::uint16: size = 2; qToBigEndian<quint16>(value, a.data()); break;
        case Storagetype::uint32: size = 4; qToBigEndian<quint32>(value, a.data()); break;
        default: break;
        }
    }
    else
        switch (scaling->_storagetype) {
        case Storagetype::int8:   size = 1;           a.data()[0] = value ;                          break;
        case Storagetype::int16:  size = 2; qToLittleEndian<qint16>(value, a.data());  break;
        case Storagetype::int32:  size = 4; qToLittleEndian<qint32>(value, a.data());  break;
        case Storagetype::uint8:  size = 1;            a.data()[0] = value;                          break;
        case Storagetype::uint16: size = 2; qToLittleEndian<quint16>(value, a.data()); break;
        case Storagetype::uint32: size = 4; qToLittleEndian<quint32>(value, a.data()); break;
        default: break;
        }
    a.resize(size);
    return a;
}

float abstractMemoryScaled::toFloat(Scaling *scaling)          //кастуем данные к определенному типу
{
    return toFloat(scaling, 0);
}

float abstractMemoryScaled::toFloat(Scaling *scaling, int index)          //кастуем данные к определенному типу
{
    char* c = data();
    qint64 typedValue = 0;
    if (scaling->endian)
        switch (scaling->_storagetype) {
        case Storagetype::int8:   typedValue = (qint8)data()[index];                break;
        case Storagetype::int16:  typedValue = qFromBigEndian<qint16>(&c[index]);  break;
        case Storagetype::int32:  typedValue = qFromBigEndian<qint32>(&c[index]);  break;
        case Storagetype::uint8:  typedValue = (quint8)c[index];  break;
        case Storagetype::uint16: typedValue = qFromBigEndian<quint16>(&c[index]);
//typedValue = qFromLittleEndian<quint16>(&c[index]);
//qDebug() << "hop "<<typedValue<<(quint8)c[index]<<(quint8)c[index+1];
            break;
        case Storagetype::uint32: typedValue = qFromBigEndian<quint32>(&c[index]);break;
        default: break;
        }
    else
        switch (scaling->_storagetype) {
        case Storagetype::int8:   typedValue = (qint8)c[index];           break;
        case Storagetype::int16:  typedValue = qFromLittleEndian<qint16>(&c[index]); break;
        case Storagetype::int32:  typedValue = qFromLittleEndian<qint32>(&c[index]); break;
        case Storagetype::uint8:  typedValue = (quint8)c[index];      break;
        case Storagetype::uint16: typedValue = qFromLittleEndian<quint16>(&c[index]); break;
        case Storagetype::uint32: typedValue = qFromLittleEndian<quint32>(&c[index]); break;
        default: break;
        }
    return scaling->toexpr2.fast_calc( typedValue);
}

float abstractMemoryScaled::toFloatOffset(Scaling *scaling, int index)
{

    char* c = data()+index;
    qint64 typedValue = 0;
    if (scaling->endian)
        switch (scaling->_storagetype) {
        case Storagetype::int8:   typedValue = (qint8)(*c);                break;
        case Storagetype::int16:  typedValue = qFromBigEndian<qint16>(*c);  break;
        case Storagetype::int32:  typedValue = qFromBigEndian<qint32>(*c);  break;
        case Storagetype::uint8:  typedValue = (quint8)(*c);  break;
        case Storagetype::uint16: typedValue = qFromBigEndian<quint16>(*c);
            break;
        case Storagetype::uint32: typedValue = qFromBigEndian<quint32>(*c);break;
        default: break;
        }
    else
        switch (scaling->_storagetype) {
        case Storagetype::int8:   typedValue = (qint8)(*c);           break;
        case Storagetype::int16:  typedValue = qFromLittleEndian<qint16>(*c); break;
        case Storagetype::int32:  typedValue = qFromLittleEndian<qint32>(*c); break;
        case Storagetype::uint8:  typedValue = (quint8)(*c);      break;
        case Storagetype::uint16: typedValue = qFromLittleEndian<quint16>(*c); break;
        case Storagetype::uint32: typedValue = qFromLittleEndian<quint32>(*c); break;
        default: break;
        }
    return scaling->toexpr2.fast_calc( typedValue);
}

QVector<float> abstractMemoryScaled::fromMemoryA(Scaling *scaling, int count)
{
    int elem_size = scaling->getElementSize();
    QVector<float> r(count);
    for (int i = 0; i < count; i++)
    {
        r[i] = toFloat(scaling, i * elem_size );
    }
    return r;
}
