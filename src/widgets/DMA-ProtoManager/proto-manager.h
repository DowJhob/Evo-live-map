#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <QGroupBox>

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
    void _logRateChanged();

signals:
    void logRateChanged(uint);

};

#endif // PROTOMANAGER_H
