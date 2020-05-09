#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>
//#include <windows.h>
//#include <qt_windows.h>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include "enumdev.h"
#include "custom_tablewidget.h"
#include <QLCDNumber>



#include "mathparser2.h"
//#include "ecu.h"
#include "qhexedit/qhexedit.h"
#include "xmldomparser.h"
#include "ecu_interface.h"
#include "op13.h"
#include "op20.h"
#include "gauge_widget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QHash<QString, CustomTableWidget*> ptrRAMtables;
    enumerator Enumerator;
    QStringList listFiles;
    QString FirstFile_by_Name = {};
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);


public slots:
    void logger_and_tableWidget_trace(QByteArray in);
    void updateRAM(int row, int column)
    {
        CustomTableWidget *table = qobject_cast<CustomTableWidget*>( sender() );

        uint pos;
        if (table->Table_Decl.Table.swapxy)
        {
            pos = column * table->Table_Decl.Y_axis.elements + row;
        }
        else
        {
            pos = row * table->Table_Decl.X_axis.elements + column;
        }
        char Out[4];
        qint64 out = qRound64(fast_calc(table->Table_Decl.Table.rom_scaling.frexpr2, table->item(row, column)->text().toDouble()));
        switch (table->Table_Decl.Table.rom_scaling._storagetype) {
        case Storagetype::int8:
        case Storagetype::uint8:            memcpy(Out, (char*)&out, 1); break;
        case Storagetype::int16:
        case Storagetype::uint16: pos *= 2; memcpy(Out, (char*)&out, 2); break;
        case Storagetype::int32:
        case Storagetype::uint32: pos *= 4; memcpy(Out, (char*)&out, 4); break;
        default: break;
        }
        ecu_comm->sendDMAcomand(0xE2, table->Table_Decl.Table.ram_addr + pos, 1, Out);
    }


protected :

private slots:
    void dll_connect(int VechicleInterfaceType)                //по сигналу перечислителя
    {
        if (ecu_comm == nullptr  )
        {
            if (VechicleInterfaceType == 13  )
                ecu_comm = new OP13(Enumerator.DllLibraryPath);
            if (VechicleInterfaceType == 20  )
                ecu_comm = new OP20(Enumerator.DllLibraryPath);

            connect(this, SIGNAL(startLogger(quint32, quint16)), ecu_comm, SLOT(startLogger(quint32, quint16)));
            connect(this, &MainWindow::stopLogger, ecu_comm, &ECU_interface::stopLogger);
            connect(this, SIGNAL(setLoggingInterval(int)), ecu_comm, SLOT(setLoggingInterval(int)));

            connect(ecu_comm, SIGNAL(readyRead(QByteArray)), this, SLOT(logger_and_tableWidget_trace(QByteArray)));
            connect(ecu_comm, SIGNAL(Log(QString)), this, SLOT(Log(QString)));

            if ( afr_lcd != nullptr )
                connect(ecu_comm, SIGNAL(AFR(QString)), afr_lcd, SLOT(display(QString)));
            connect(ecu_comm, &ECU_interface::interfaceReady, this, &MainWindow::interfaceUnlock);
            //=============================================================================
            connect(&interface_thread, &QThread::started, ecu_comm, &ECU_interface::init);
            ecu_comm->moveToThread(&interface_thread);
            interface_thread.start();

        }
    }
    void dll_disconnect()
    {
        if (ecu_comm != nullptr)
        {
            interfaceLock();
            ecu_comm->deleteLater();
            interface_thread.quit();
            interface_thread.wait(1000);
            ecu_comm = nullptr;
        }
    }
    void interfaceUnlock()
    {
        interfaceThumbler(false);
    }
    void interfaceLock()
    {
        interfaceThumbler(true);
    }
    void interfaceThumbler(bool lockFlag);

    void on_BaudRatelineEdit_textChanged(const QString &arg1);
    void StartButton_slot();
    void RAM_reset_slot();
    void on_logger_rate_textedit_editingFinished();
    void debugButton_slot();
    void on_loadbinbutton_clicked();

    void on_save_trace_pushButton_clicked();
    void Log(QString str);
    void on_start_addr_lineEdit_returnPressed();
    void on_count_lineEdit_returnPressed();
    void itemChecks(QTreeWidgetItem *item, int column)
    {
        CustomTableWidget *table = ptrRAMtables.value( item->text(column).split(" RAM").at(0), nullptr );
        if (table == nullptr)
            return;
        if ( item->checkState(column) )
            table->parentWidget()->show();
        else
            table->parentWidget()->hide();
    }

private:
    bool ReadConfig(QString filename);
    bool getECU(QString filename);
    void TableDelete();
    QString load_bin(QString bin_filename)
    {
        QFile binfile(bin_filename);
        if (!binfile.open(QIODevice::ReadWrite ))
            return "";
        if (binarray != nullptr)
            delete binarray;
        binarray = new QByteArray( binfile.read(binfile.size()));     // новый массив

        //uint romIDnum = qFromBigEndian<quint32>(binarray->mid(0xf52, 4)); // номер калибровки
        //return QString::number( romIDnum, 16 );
    }
    QString SearchFiles(QString path, QString CalID)       // Для поиска файлов в каталоге
    {
        // Пытаемся найти правильные файлы, в текущем каталоге
        listFiles = QDir(path).entryList((CalID + "*.xml ").split(" "), QDir::Files);  //выборка файлов по маскам
        if (listFiles.size()  == 0)            // если файл не найдем вернем егог
            return QFileDialog::getOpenFileName(this, tr("Open xml"), path, tr("xml files (*.xml)"));
        else
            return path + listFiles.at(0);
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
    int  axis_lookup2(float in, int axis_lenght, QVector<float> axis)    //возвращает меньший индекс
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
    void create_table(tableDeclaration *tab);
    void create_tree(tableDeclaration *tab);
    void create_gauge(QString name, mutParam *param);
    void axread(sub_tableDeclaration *sub_tab, QVector<float> *axis, bool rom);

    QAction *start_action;
    QAction *debug_action;
    QAction *ram_reset;
    QThread interface_thread;
    ecu *_ecu;
    ECU_interface *ecu_comm = nullptr;
    QString CurrDir;
    QString xml_filename;
    bool  debug = false;
    Ui::MainWindow *ui;
//    QTimer* timer = new QTimer(this);
    bool save_trace = false;
    DomParser xmlParser;
    QByteArray *binarray;                                  // массив с бинарником
    //======================== widget's =================================
    QHexEdit *hexEdit;
    gauge_widget *afr_lcd;
    //======================== widget lists =================================
    QSet<gauge_widget*> gauge_set;
    QSet<QWidget*> widget_set;
    QSet<CustomTableWidget*> table_set;

signals:
    void startLogger(quint32, quint16);
    void stopLogger();
    void setLoggingInterval(int);
};
#endif // MAINWINDOW_H
