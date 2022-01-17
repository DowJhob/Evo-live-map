#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QObject>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "qhexedit/qhexedit.h"


class hexEditor : public QWidget
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
