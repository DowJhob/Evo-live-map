#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <QGroupBox>

//#include "comm-device-interface.h"
#include "../../DMA-proto/DMA-proto.h"
#include "../../DMA-proto/jcsbanksDMA.h"
#include "../../DMA-proto/stockDMA.h"
#include "../../DMA-proto/evoX-DMA.h"

Q_DECLARE_METATYPE( DMA_proto* )

namespace Ui {
class protoManager;
}

class protoManager : public QGroupBox
{
    Q_OBJECT

public:

    QThread *ecu_thread;

    explicit protoManager(QWidget *parent = nullptr);
    ~protoManager();

    void addProtos();

private:
    Ui::protoManager *ui;

private slots:
    void _protoSelected(int index);
    void _logRateChanged();

signals:
    void protoSelected(DMA_proto*);
    void logRateChanged(uint);

};

#endif // PROTOMANAGER_H
