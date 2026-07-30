#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <QGroupBox>

namespace Ui {
class protoManager;
}

class logRateManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    QThread *ecu_thread;

    explicit logRateManagerWidget(QWidget *parent = nullptr);
    ~logRateManagerWidget();

private:
    Ui::protoManager *ui;

private slots:
    void _logRateChanged();

signals:
    void logRateChanged(uint);

};

#endif // PROTOMANAGER_H
