#ifndef FLASHWIDGET_H
#define FLASHWIDGET_H

#include <QGroupBox>

namespace Ui {
class flashWidget;
}

class flashWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit flashWidget(QWidget *parent = nullptr);
    ~flashWidget();

private slots:
    void _readECU();
    void _writeECU();
    void addFlashModel();

private:
    Ui::flashWidget *ui;
};

#endif // FLASHWIDGET_H
