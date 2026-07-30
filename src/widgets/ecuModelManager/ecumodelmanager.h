#ifndef ECUMODELMANAGER_H
#define ECUMODELMANAGER_H

#include "src/ECU-model/ecu-model.h"
#include <QGroupBox>

#include "../../DMA-proto/DMA-proto.h"

namespace Ui {
class ecuModelManager;
}

class ecuModelManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    // QThread *ecu_thread;

    explicit ecuModelManagerWidget(QWidget *parent = nullptr);
    ~ecuModelManagerWidget();

public slots:
    // Заполняем после подключения, тогда при добавлении буду сигналы
    void fillECU_Models(QMap<ecuModelType, QString>*availECUmodels);
    void fillAvailProtos(QMap<DMA_ProtoType, QString> *availProtos);

private:
    Ui::ecuModelManager *ui;

private slots:
    void _modelSelected(int index);
    void _protoSelected(int index);

signals:
    void modelSelected(ecuModelType);
    void protoSelected(DMA_ProtoType);

};

#endif // ECUMODELMANAGER_H
