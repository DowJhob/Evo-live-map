#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QMainWindow>
#include "enumdev.h"
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
    QObjectList list_window = {};    //список для динамически созданных таблиц что бы разгрузить событие таймера
    QObjectList list_button = {};
    QObjectList list_widget = {};
    QStringList listFiles;                                 //
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);
    void ReadConfig(QString filename);
    void CreateTable(QString filename);
    //bool VechicleInterfaceState;
    void TableDelete();
public slots:
    void logger_and_tableWidget_trace();
    //void timer_lock_unlock();
    void table_show_hide()
    {
        /* Определяем объект, который вызвал сигнал*/
        QPushButton *tableButton = qobject_cast<QPushButton*>( sender() );
        /* Ищем объект, в списке*/
        foreach (QObject *o, list_window)
        {
            DynamicWindow *tablewindow = qobject_cast<DynamicWindow*>(o);
            if (tableButton->property("tag") ==  tablewindow->Tag)
                tablewindow->setVisible( !tablewindow->isVisible());
        }
    }

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
void fpssss()
{
    qDebug() << "FPS = " << fps;
    fps =0;
}


private:
    Ui::MainWindow *ui;
    void OperateButtonsLockUnlock();
    float x = 0;
    float y = 0;
    Timer* timer = new Timer(this);
    bool debug;
     enumerator Enumerator;
     dma DMA;

     mathParser2 math;
     DomParser *xmlParser;

     TableProperty_fr_xml Table_Decl;                       //Описание одной таблицы

     QByteArray *binarray;                                  // массив с бинарником

    int fps = 0;
    QTimer fpss;
    void SearchFiles(QString path, QString CalID)       // Для поиска файлов в каталоге
    {
        // Пытаемся найти правильные файлы, в текущем каталоге

        //   QApplication::processEvents();    //что бы не замирал интерфейс при овер дохуа файлов в каталоге

        listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
        if (listFiles.size()  == 0)            // если файл не найдем вернем пустую строку
            listFiles[0] = "error";                               // если файл не найдем вернем error
    }
    float read_and_cast(bool ram_scaling_storagetype, QString storagetype, quint32 mut_number, bool scaling_endian, fast_calc_struct scaling_frexpr2 )
    {
        float x = 0;
        if (!ram_scaling_storagetype)
        {
            x = math.typed(storagetype,
                            DMA.MUT_In_buffer,
                            mut_number,                //номер запроса рам мут
                            scaling_endian);


            x = math.fast_calc(scaling_frexpr2, x);
        }
        return x;
    }

};






#endif // MAINWINDOW_H
