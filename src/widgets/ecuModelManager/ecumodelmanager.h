#ifndef ECUMODELMANAGER_H
#define ECUMODELMANAGER_H

#include "src/ECU-model/ecu-model.h"
#include <QGroupBox>

#include "../../DMA-proto/DMA-proto.h"

namespace Ui {
class ecuModelManager;
}

class ecuModelManager : public QGroupBox
{
    Q_OBJECT

public:
    QThread *ecu_thread;

    explicit ecuModelManager(QWidget *parent = nullptr);
    ~ecuModelManager();

    // Заполняем после подключения, тогда при добавлении буду сигналы
    void fillModels();

private:
    Ui::ecuModelManager *ui;

    ECU_model *model = nullptr;
    DMA_proto* proto = nullptr;

private slots:
    void _modelSelected(int index);

    void _protoSelected(int index);

    void fillAvailModelProtos();

signals:
    void modelSelected(ECU_model*);

    void protoSelected(DMA_proto*);

};

#endif // ECUMODELMANAGER_H
