#ifndef COMMPARAMWIDGET_H
#define COMMPARAMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLayout>

#include "../enumdev.h"

class commParamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit commParamWidget(QWidget *parent = nullptr);

public slots:
    void addDevice(device dev);
    void removeDevice(device dev);

    void deviceSelected(int index);
    //void protoSelected(int index);

private:
    QHash<QString, device> dev;
    QComboBox *commListBox;
    QComboBox *protoListBox;

    QLineEdit *baudRate;
QLineEdit *logRate;
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
