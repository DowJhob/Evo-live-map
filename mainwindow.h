#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>
#include <QLCDNumber>


#include "custom_tablewidget.h"
#include "mathparser2.h"
#include "qhexedit/qhexedit.h"
#include "xmlparser.h"
#include "ecu_interface.h"
#include "op13.h"
#include "op20.h"
#include "gauge_widget.h"
//#include "graph_logger.h"


#include "logger.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QHash<QString, CustomTableWidget*> ptrRAMtables;
//    enumerator Enumerator;
    QStringList listFiles;
    QString FirstFile_by_Name = {};
    ~MainWindow();
    explicit MainWindow(QWidget *parent = nullptr);

public slots:

    void logger_and_tableWidget_trace2();
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
        vehicle_ecu_comm->sendDMAcomand(0xE2, table->Table_Decl.Table.ram_addr + pos, 1, Out);
    }
    void dll_connect(int VechicleInterfaceType, TCHAR* DllLibraryPath, bool isTactrix);
    void dll_disconnect();

protected :

private slots:

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
    void create_table(tableDeclaration *tab);
    void create_tree(tableDeclaration *tab);
    void create_gauge(QString name, mutParam *param);
    void axread(sub_tableDeclaration *sub_tab, QVector<float> *axis, bool rom);

    QAction *start_action;
    QAction *debug_action;
    QAction *ram_reset;

    QThread vehicle_ecu_interface_thread;
    QThread logger_thread;

    ecu *_ecu;
    ECU_interface *vehicle_ecu_comm = nullptr;
    QString CurrDir;
    QString xml_filename;
    bool  debug = false;
    Ui::MainWindow *ui;

    QToolBar *loggerWidgetBar = nullptr;

    bool save_trace = false;
    xmlParser xmlParser;
    QByteArray *binarray;                                  // массив с бинарником
    //======================== widget's =================================
    QHexEdit *hexEdit;
    gauge_widget *tactrix_afr_lcd = nullptr;
    //======================== widget lists =================================
    QSet<gauge_widget*> gauge_widget_set;
    QSet<QWidget*> widget_set;
    QSet<CustomTableWidget*> table_set;

    Logger _logger;

signals:
    void startLogger(quint32, quint16);
    void _delete();
    void start_WB();
    void stopLogger();
    void setLoggingInterval(int);
};
#endif // MAINWINDOW_H
