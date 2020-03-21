#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QMainWindow>
#include "enumdev.h"
#include "dynamicwindow.h"
#include "mathparser2.h"

#include "windows.h"
#include <QTimer>

#include <time.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<mapWidget*> list_window;    //список для динамически созданных таблиц
    QList<QPushButton*> list_button = {};
    QStringList listFiles;
    QString FirstFile_by_Name = {};
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);
    bool ReadConfig(QString filename);
    bool CreateTable(QString filename);
    //bool VechicleInterfaceState;
    void TableDelete();

    long utime()
    {
        struct timespec t;
        clock_gettime(CLOCK_REALTIME, &t);
        return t.tv_nsec;
    }

public slots:
    void logger_and_tableWidget_trace();
    //void timer_lock_unlock();
    void table_show_hide()
    {
        /* Определяем объект, который вызвал сигнал*/
        QPushButton *tableButton = qobject_cast<QPushButton*>( sender() );
        mapWidget *window = list_window.at(tableButton->property("tag").toInt());
        window->setVisible( !window->isVisible());
    }

signals:
    void timer_lock();
    void timer_unlock();

protected :

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

    void on_save_trace_pushButton_clicked();

private:
    int x, y;
    QString CurrDir;
    QString xml_filename;
    bool  debug = false;
    Ui::MainWindow *ui;
    void OperateButtonsLockUnlock();
    Timer* timer = new Timer(this);
    enumerator Enumerator;
    dma DMA;
    bool save_trace = false;
    DomParser *xmlParser;
    tableDeclaration Table_Decl;                       //Описание одной таблицы
    QByteArray *binarray;                                  // массив с бинарником
    void SearchFiles(QString path, QString CalID)       // Для поиска файлов в каталоге
    {
        // Пытаемся найти правильные файлы, в текущем каталоге
        listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
        if (listFiles.size()  == 0)            // если файл не найдем вернем егог
            xml_filename = QFileDialog::getOpenFileName(this, tr("Open xml"), path, tr("xml files (*.xml)"));
        else
            xml_filename =  CurrDir + listFiles.at(0);
    }
    int modul(int a, int b)
    {
        int mod = qRound(sqrt(pow(a, 2) + pow(b, 2)));
        if (mod > 255)
            mod = 255;

        mod = (255 - mod);
        if (mod < 30)
            mod = 30;
        return mod;
    }

    int  axis_lookup2(int in, int axis_lenght, QVector<int> axis)    //возвращает меньший индекс
    {
        if (axis_lenght <= 1 )
            return 0;
        if (in < axis[0])
            return 0;
        if (in > axis[axis_lenght - 1])
            return axis_lenght - 1;
        for (int i = 0; i < axis_lenght-1; i++)
            if (  in >= axis[i] && in < axis[i+1])
                return i;
        return axis_lenght - 1;
    }

    float read_and_cast(bool ram_scaling_storagetype, QString storagetype, quint32 mut_number, bool scaling_endian, fast_calc_struct scaling_frexpr2 )
    {
        float x = 0;
        if (!ram_scaling_storagetype)
        {
            x = typed(storagetype,
                      DMA.rx_msg[1].Data,
                      mut_number,                //номер запроса рам мут
                      scaling_endian);
            x = fast_calc(scaling_frexpr2, x);
        }
        return x;
    }

};
#endif // MAINWINDOW_H
