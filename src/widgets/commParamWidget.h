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
    //uint baudRate;
    QLineEdit *el_baudRate;
    explicit commParamWidget(QWidget *parent = nullptr);

public slots:
    void addDevice(device dev);
    void removeDevice(device dev);

    void deviceSelected(int index);
    //void protoSelected(int index);

    void lockInterface(bool lockFlag);

private:
    QHash<QString, device> dev;
    QComboBox *commListBox;
    QComboBox *protoListBox;

    QLineEdit *el_logRate;

    void baudChng();

    void logchng();

signals:
    void interfaceSelected(device);
    //void interfaceRemoved(device);
    void protoSelected(int);

    void baudChanged(int);
    void logChanged(int);

};

#endif // COMMPARAMWIDGET_H
