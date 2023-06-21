#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include "3rdparty/qhexedit2/src/qhexedit.h"
#include <QObject>
#include <QGroupBox>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class hexEditor : public QGroupBox
{
    Q_OBJECT
public:
    hexEditor(QWidget *parent = nullptr);

public slots:
    void setData(QByteArray data);

private slots:
    void _getData();

private:
    QLineEdit *addrEdit;
    QLineEdit *countEdit;
    QHexEdit *he;

signals:
    void getData(quint32, quint32);

};

#endif // HEXEDITOR_H
