#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QMainWindow>

#include "dynamicwindow.h"
#include "mathparser2.h"

#include "windows.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);
    void ReadConfig(QString filename);
    void CreateTable(QString filename);
    //bool VechicleInterfaceState;
    void TableDelete();
public slots:
//void logger_and_tableWidget_trace();
//void timer_lock_unlock();

signals:
   void timer_lock();
   void timer_unlock();

protected :
    //
bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void on_BaudRatelineEdit_textChanged(const QString &arg1);
    void on_StartButton_clicked();
    void on_RAM_reset_Button_clicked();
    void on_read_RAM_Button_clicked();
    void on_logger_rate_textedit_editingFinished();
    void on_debugButton_clicked();
    void on_loadbinbutton_clicked();
    void on_stop_live_clicked();

    void on_verticalSlider_2_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    void OperateButtonsLockUnlock();
};






#endif // MAINWINDOW_H
