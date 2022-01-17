#ifndef OP20WBREADER_H
#define OP20WBREADER_H

#include <QObject>

class op20wbReader : public QObject
{
    Q_OBJECT
public:
    explicit op20wbReader(QObject *parent = nullptr);

signals:

};

#endif // OP20WBREADER_H
