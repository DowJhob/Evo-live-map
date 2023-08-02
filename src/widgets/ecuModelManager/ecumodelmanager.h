#ifndef ECUMODELMANAGER_H
#define ECUMODELMANAGER_H

#include "src/ECU-model/ecu-model.h"
#include <QGroupBox>

namespace Ui {
class ecuModelManager;
}

class ecuModelManager : public QGroupBox
{
    Q_OBJECT

public:
    explicit ecuModelManager(QWidget *parent = nullptr);
    ~ecuModelManager();

    void addModels();

private:
    Ui::ecuModelManager *ui;

private slots:
    void _modelSelected(int index);

signals:
    void modelSelected(ECU_model*);

};

#endif // ECUMODELMANAGER_H
