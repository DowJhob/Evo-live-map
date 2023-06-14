#ifndef ECUMANAGER2_H
#define ECUMANAGER2_H

#include <QGroupBox>

namespace Ui {
class ecuManager2;
}

class ecuManager2 : public QGroupBox
{
    Q_OBJECT

public:
    explicit ecuManager2(QWidget *parent = nullptr);
    ~ecuManager2();

private:
    Ui::ecuManager2 *ui;
};

#endif // ECUMANAGER2_H
