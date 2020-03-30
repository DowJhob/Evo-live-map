#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <time.h>
#include <windows.h>
#include <qt_windows.h>
#include <QMainWindow>
#include <QQueue>
#include <QTimer>
#include "enumdev.h"
#include "dynamicwindow.h"
#include "mathparser2.h"
#include "ecu.h"
#include "qhexedit/qhexedit.h"

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

public slots:
    void logger_and_tableWidget_trace();
    //void timer_lock_unlock();
    void table_show_hide()
    {
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
    void Log(QString str);

    void on_inno_initButton_clicked();

    void on_start_addr_lineEdit_returnPressed();

    void on_count_lineEdit_returnPressed();

private:
    void evoX_Connect_Click()
    {
//        object obj2;
//       if (!this.b_Connect.Text.Equals("Connect and Live Tune"))
//        {
//            if (this.b_Connect.Text.Equals("Disconnect"))
//            {
//                this.b_Connect.Text = "Connect and Live Tune";
//                byte[] buffer5 = new byte[] { 0 };
//                obj2 = ECU_Communications;
//                lock (obj2)
//                {
//                    this.Write_Activity_Log_Entry("80501A: " + BitConverter.ToString(buffer5));
//                    if (this._j2534.Write_and_Verify(0x80_501a, buffer5, 1) != 0)
//                    {
//                        throw new ApplicationException("Error: Retry rewrite exceeded for 0x80501A\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
//                    }
//                }
//                this.b_RAM_ROM_Compare.Enabled = false;
//                this.b_Open_ROM_File.Enabled = true;
//                this.live_tuner_connected = false;
//                this.button1.Enabled = true;
//            }
//        }
//        else
//        {
//            try
//            {
//                byte[] buffer2;
//                byte[] buffer = this.LocalROM_Query(0xb_fff0, 4);
//                obj2 = ECU_Communications;
//                lock (obj2)
//                {
//                    buffer2 = this._j2534.ISO15765_Mode23_Query(0xb_fff0, 4);
//                }
//                if (buffer2 == null)
//                {
//                    throw new ApplicationException("Error: Couldn't retrieve the ECU Checksum\nPlease try again with the OP20 actually connected to the vehicle...");
//                }
//                int index = 0;
//                while (true)
//                {
//                    if (index >= 4)
//                    {
//                        obj2 = ECU_Communications;
//                        lock (obj2)
//                        {
//                            this.Current_Alt_Map = this._j2534.ISO15765_Mode23_Query(0x80_5018, 1)[0];
//                        }
//                        this.Populate_MapSelector();
//                        uint num = 0;
//                        foreach (string str , this.MapSelector.Items)
//                        {
//                            if (!str.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
//                            {
//                                num += uint.Parse(((Hashtable) this.maps[str])["x-axis-elements"].ToString());
//                            }
//                        }
//                        this.panel1.Dock = DockStyle.Fill;
//                        this.panel1.Show();
//                        this.panel1.BringToFront();
//                        this.progressBar1.BringToFront();
//                        this.progressBar1.Maximum = (int) num;
//                        this.progressBar1.Step = 1;
//                        this.progressBar1.Minimum = 0;
//                        this.progressBar1.Value = 0;
//                        obj2 = ECU_Communications;
//                        lock (obj2)
//                        {
//                            this.Write_Activity_Log_Entry("80501A: 69");
//                            byte[] payload = new byte[] { 0x45 };
//                            if (this._j2534.Write_and_Verify(0x80_501a, payload, 1) != 0)
//                            {
//                                throw new ApplicationException("Error: Retry rewrite exceeded for 0x80501A\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
//                            }
//                            foreach (string str2 , this.MapSelector.Items)
//                            {
//                                if (!str2.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
//                                {
//                                    byte[] buffer3;
//                                    this.Write_Activity_Log_Entry("... uploading " + str2);
//                                    this.label1.Text = "... uploading " + str2;
//                                    Hashtable hashtable = (Hashtable) this.maps[str2];
//                                    if (hashtable["lt-memory-blk"] == null)
//                                    {
//                                        throw new ApplicationException("Error: No lt-memory-blk attribute setup for map \"" + str2 + "\"");
//                                    }
//                                    uint address = uint.Parse(hashtable["lt-memory-blk"].ToString(), NumberStyles.HexNumber);
//                                    uint num4 = (uint) hashtable["x-axis-elements"];
//                                    uint num5 = (uint) hashtable["y-axis-elements"];
//                                    uint num6 = (uint) hashtable["element-size"];
//                                    if (hashtable["header"] != null)
//                                    {
//                                        uint length = (uint) ((byte[]) hashtable["header"]).Length;
//                                        buffer3 = this.LocalROM_Query(uint.Parse(hashtable["address"].ToString(), NumberStyles.HexNumber) - length, length);
//                                        this.Write_Activity_Log_Entry(address.ToString("X") + ": " + BitConverter.ToString(buffer3));
//                                        if (this._j2534.Write_and_Verify(address, buffer3, length) != 0)
//                                        {
//                                            throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
//                                        }
//                                        address += length;
//                                    }
//                                    for (int i = 0; i < num4; i++)
//                                    {
//                                        buffer3 = this.LocalROM_Query(uint.Parse(hashtable["address"].ToString(), NumberStyles.HexNumber) + ((uint) ((i * num5) * num6)), num6 * num5);
//                                        this.Write_Activity_Log_Entry(address.ToString("X") + ": " + BitConverter.ToString(buffer3));
//                                        if (this._j2534.Write_and_Verify(address, buffer3, num6 * num5) != 0)
//                                        {
//                                            throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
//                                        }
//                                        address += num6 * num5;
//                                        this.progressBar1.PerformStep();
//                                        Thread.Sleep(1);
//                                        Application.DoEvents();
//                                    }
//                                }
//                            }
//                            this.label1.Text = "";
//                            this.panel1.SendToBack();
//                            this.panel1.Hide();
//                            this.panel1.Dock = DockStyle.None;
//                            this.Write_Activity_Log_Entry("... writing memory pointer overrides");
//                            foreach (string str3 , this.MapSelector.Items)
//                            {
//                                if (!str3.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
//                                {
//                                    Hashtable hashtable2 = (Hashtable) this.maps[str3];
//                                    if (!hashtable2["type"].ToString().Equals("1D"))
//                                    {
//                                        if (hashtable2["lt-memory-ptr"] == null)
//                                        {
//                                            throw new ApplicationException("Error: No lt-memory-ptr attribute setup for map \"" + str3 + "\"");
//                                        }
//                                        uint address = uint.Parse(hashtable2["lt-memory-ptr"].ToString(), NumberStyles.HexNumber);
//                                        uint num10 = uint.Parse(hashtable2["lt-memory-blk"].ToString(), NumberStyles.HexNumber);
//                                        byte[] buffer4 = new byte[] { (byte) (num10 >> 0x18), (byte) ((num10 - ((num10 >> 0x18) << 0x18)) >> 0x10), (byte) ((num10 - ((num10 >> 0x10) << 0x10)) >> 8), (byte) (num10 - ((num10 >> 8) << 8)) };
//                                        string[] textArray1 = new string[] { address.ToString("X"), " (", str3, "): ", BitConverter.ToString(buffer4) };
//                                        this.Write_Activity_Log_Entry(string.Concat(textArray1));
//                                        if (this._j2534.Write_and_Verify(address, buffer4, 4) != 0)
//                                        {
//                                            throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        break;
//                    }
//                    if (buffer[index] != buffer2[index])
//                    {
//                        throw new ApplicationException("Error: ECU Checksum doesn't match ROM checksum\nPlease try again with the actual ROM file you last flashed...");
//                    }
//                    index++;
//                }
//                this.b_Connect.Text = "Disconnect";
//                this.b_Open_ROM_File.Enabled = false;
//                this.b_RAM_ROM_Compare.Enabled = true;
//                this.live_tuner_connected = true;
//                this.button1.Enabled = false;
//            }
//            catch (Exception exception)
//            {
//                this.panel1.SendToBack();
//                this.panel1.Hide();
//                this.panel1.Dock = DockStyle.None;
//                MessageBox.Show("Connect error: " + exception.Message);
//            }
//        }
    }

    ecu *_ecu;

    QString CurrDir;
    QString xml_filename;
    bool  debug = false;
    Ui::MainWindow *ui;
    void OperateButtonsLockUnlock();
    QTimer* timer = new QTimer(this);
    enumerator Enumerator;
    dma DMA;
    bool save_trace = false;
    DomParser xmlParser;
    tableDeclaration Table_Decl;                       //Описание одной таблицы
    QByteArray *binarray;                                  // массив с бинарником
QHexEdit *hexEdit;
    void get_table(tableDeclaration *tab)
    {
   //     if ( tableQueue->isEmpty() )
   //         return;
   //     tab = tableQueue->dequeue();

        quint32 X_ax_ram_addr = 1;

        quint32 Y_ax_ram_addr = 1;

        quint32 tab_ram_addr = tab->Table.ram_addr;
        DMA.read_direct(tab_ram_addr, 11111111);


        if ( tab->X_axis.elements > 1 )
        {
            X_ax_ram_addr = tab->X_axis.ram_addr;
            DMA.read_direct(X_ax_ram_addr, tab->X_axis.elements);
        }
        if ( tab->Y_axis.elements > 1 )
        {
            Y_ax_ram_addr = tab->Y_axis.ram_addr;
            DMA.read_direct(Y_ax_ram_addr, tab->Y_axis.elements);
        }

    }

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
