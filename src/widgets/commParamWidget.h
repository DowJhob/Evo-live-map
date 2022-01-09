#ifndef COMMPARAMWIDGET_H
#define COMMPARAMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLayout>
#include <QLineEdit>

#include "../enumdev.h"

class commParamWidget : public QWidget
{
    Q_OBJECT
public:
    uint baudRate;
    explicit commParamWidget(QWidget *parent = nullptr, uint defaultBaudRate = 62500);

public slots:
    void addDevice(device dev);
    void removeDevice(device dev);
    void deviceSelected(int index);

    void addWB(device dev);
    void removeWB(device dev);
    void WBSelected(int index);
    void WBprotoSelected(int index);
    //void protoSelected(int index);

private:
    QHash<QString, device> dev;
    QComboBox *commListBox;
    QComboBox *protoListBox;
    QComboBox *availWB;
    QComboBox *protoWB;

    QLineEdit *el_baudRate;
    QLineEdit *el_logRate;

    void baudChng();

    void logchng();

signals:
    void interfaceSelected(device);
    //void interfaceRemoved(device);
    void protoSelected(int);
    void wbSelected(int);
    void wbProtoSelected(int);

    void baudChanged(int);
    void logChanged(int);

};

#endif // COMMPARAMWIDGET_H
