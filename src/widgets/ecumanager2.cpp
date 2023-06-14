#include "ecumanager2.h"
#include "ui_ecumanager2.h"

ecuManager2::ecuManager2(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ecuManager2)
{
    ui->setupUi(this);
}

ecuManager2::~ecuManager2()
{
    delete ui;
}
