#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>


//#include "comm-device-interface.h"
#include "DMA-proto.h"
#include "jcsbanksDMA.h"
#include "stockDMA.h"
#include "evoX-DMA.h"

Q_DECLARE_METATYPE( DMA_proto* )

class protoManager : public QGroupBox
{
    Q_OBJECT
public:
    protoManager(QWidget *parent = nullptr);
    void addProtos();

public slots:
    //void deviceEvent(device dev);

private:
    QGridLayout layout;
    QComboBox availProto;
    QLabel lgrt{"Logging rate, Hz:"};
    QLineEdit el_lograte{"20"};

private slots:
    void _protoSelected(int index);
    void _logRateChanged();

signals:
    void protoSelected(DMA_proto*);
    void logRateChanged(uint);

};

#endif // PROTOMANAGER_H
