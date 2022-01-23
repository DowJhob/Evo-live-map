#include "hexeditor.h"

hexEditor::hexEditor(QWidget *parent):QWidget (parent)
{
    QGridLayout *lay = new QGridLayout(this);
    setLayout(lay);
    QLabel *addr = new QLabel("Start address in hex:", this);
    addr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QLabel *count = new QLabel("Count", this);
    count->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    addrEdit = new QLineEdit("0x00000000", this);
    addrEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    countEdit = new QLineEdit("0", this);
    countEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    he = new QHexEdit(this);
    he->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    lay->addWidget(addr, 0, 0);
    lay->addWidget(count, 0, 1);
    lay->addWidget(addrEdit, 1, 0);
    lay->addWidget(countEdit, 1, 1);
    lay->addWidget(he, 2, 0, 1, 2);

    connect(addrEdit, &QLineEdit::returnPressed, this, &hexEditor::_getData);
    connect(countEdit, &QLineEdit::returnPressed, this, &hexEditor::_getData);

    he->setAddressWidth(8);
    he->setAddressOffset(0);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void hexEditor::setData(QByteArray data)
{
    he->setData(data);
}

void hexEditor::_getData()
{
    quint16 count = countEdit->text().toUInt(nullptr);
    quint32 addr = addrEdit->text().toUInt(nullptr, 16);
    he->setAddressOffset(addr);
    emit getData(addr, count);
}
