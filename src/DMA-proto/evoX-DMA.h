#ifndef EVOX_DMA_H
#define EVOX_DMA_H

//#include <QObject>
#include <QDebug>

#include "DMA-proto.h"
#include "src/comm-device-interface/j2534-comm.h"

class evoX_DMA : public DMA_proto
{
    Q_OBJECT
public:
    explicit evoX_DMA(p_comm_device_interface *p_devComm);
    ~evoX_DMA();

    //public slots:
    bool connect_()
    {
        qDebug() << "=========== evoX_DMA::connect ================ baudRate" << (*p_devComm)->getBaudRate();
        (*p_devComm)->open();

        if (!(*p_devComm)->connect(Protocol::ISO15765))
        {
            (*p_devComm)->close();
            return false;
        }
        return true;
    }

    QByteArray indirectDMAread(quint32 addr, int lenght);
    QByteArray directDMAread(quint32 addr, int lenght);
    void directDMAwrite(quint32 addr, char *buf, int lenght);

public slots:
    void startLog(ramMUT *ramMut);
    void startLog();
    void stopLog();

    void setLogRate(int freqRate);
    void RAMreset(quint32 var1, quint16 var2);

    void updateRAM(offsetMemory memory);

private slots:

private:

    void write(int lenght)
    {

        ((j2534_comm*)(*p_devComm))->tx_msg.setProtocolId(Protocol::ISO15765);
        ((j2534_comm*)(*p_devComm))->tx_msg.setTxFlags( Message::TxFlag::OutISO15765FramePad);
        (*p_devComm)->write( lenght );
    }

    char* getOutBuff()
    {
        return (*p_devComm)->p_out_buff;
    }

    // костыль для понимания сути
    PassThru::Status PassThruReadMsgs(int GlobalChannelID, Message pRecvMsg, uint pNumMsgs, int timeout)
    {

    }

    struct ISO15765_Mode23_Query
    {
        ISO15765_Mode23_Query() {}
    };

    QByteArray ISO15765_Mode23_Fetch(uint address, uint length)
    {
        uint num = 0;

        while (true)
        {
            uint num2 = length - num;
            if (num2 > 0x40)
            {
                num2 = 0x40;
            }
            QByteArray sourceArray = ISO15765_Mode23_Query(address + num, num2);
            if (sourceArray.isEmpty())
            {
                return QByteArray();
            }

            QByteArray data(num, (char)0x00);

            QByteArray destinationArray(data + sourceArray.left(num2) + QByteArray(length - num2, (char)0x00));

            // Array.ConstrainedCopy(sourceArray, 0, destinationArray, (int) num, (int) num2);
            num += num2;
            if (num >= length)
            {
                return destinationArray;
            }
        }
    }

    // read method
    QByteArray ISO15765_Mode23_Query(uint address, uint length)
    {

        {
            getOutBuff()[0] = 0;
            getOutBuff()[1] = 0;
            getOutBuff()[2] = 7;
            getOutBuff()[3] = 0xe0;
            getOutBuff()[4] = 0x23;
            getOutBuff()[5] = (uchar)(address >> 0x10);
            getOutBuff()[6] = (uchar)((address - ((address >> 0x10) << 0x10)) >> 8);
            getOutBuff()[7] = (uchar)(address - ((address >> 8) << 8));
            getOutBuff()[8] = (uchar)length;
        }
        QByteArray buffer2;
        //     // Connect();
        PassThru::Status status = PassThru::Status::NoError;
        uint readAttempts = 0;
        bool flag = false;
        Message pRecvMsg;
        while (!flag && (readAttempts < 3))
        {
            readAttempts++;

            write(9);            //         this.J2534_Send(6, 0x40, data, 9);

            bool devComError = false;
            int calculatedTimeout = (int) (15 * (1 + (length / 7)));
            int countTimeout = 0;
            while (!devComError && (countTimeout < calculatedTimeout))
            {
                uint pNumMsgs = 1;
                // try
                {
                    int GlobalChannelID = 0;                             //  костыли
                    status = PassThruReadMsgs(GlobalChannelID, pRecvMsg, pNumMsgs, 10); // костыли
                    if (status != PassThru::Status::NoError)
                    {
                        if ((status == PassThru::Status::Timeout) || (status == PassThru::Status::BufferEmpty))
                        {
                            countTimeout += 10;
                        }
                        else
                        {
                            if ((status != PassThru::Status::DeviceNotConnected) && (status != PassThru::Status::InvalidChannelID))
                            {
                                // throw new ApplicationException("Error: Mode23 - PassThruReadMsgs = " + this.J2534_Error());
                            }
                            devComError = true;
                        }
                    }
                    else
                    {
                        uint num6 = pRecvMsg.m_data[0];

                        if (
                            (num6 != 0x63) || ( pRecvMsg.m_dataSize != (length + 5) )
                            )
                        {
                            if ((num6 == 0x7f) && (pRecvMsg.m_data[2] == 0x78) )
                            {
                                QThread::sleep(2);
                            }
                        }
                        else
                        {
                            // buffer2 = new byte[length];
                            int index = 0;
                            while (true)
                            {
                                if (index >= length)
                                {
                                    devComError = true;
                                    flag = true;
                                    break;
                                }
                                buffer2[index] = pRecvMsg.m_data[index + 1];
                                index++;
                            }
                        }
                    }
                }
                //             catch (Exception)
                //             {
                //                 readAttempts++;
                //             }
            }
        }
        //     // Marshal.FreeHGlobal(pRecvMsg);
        //     if (readAttempts > 1)
        //     {
        //         // total_mode23_count += readAttempts - 1;
        //     }
        return buffer2;
    }

    // write method
    bool ISO15765_Mode3D_Query(uint address, char* Payload, uint length, uint requested_timeout)
    {
        int calculatedTimeout = (int) (0x20 + (length / 7));
        if (requested_timeout != 0)
        {
            calculatedTimeout = (int) requested_timeout;
        }
        char *data = getOutBuff();
        int status = 0;
        uint num2 = 0;
        bool flag = false;
        // this.Connect();
        while (!flag)
        {
            num2++;
            getOutBuff()[0] = 0;
            getOutBuff()[1] = 0;
            getOutBuff()[2] = 7;
            getOutBuff()[3] = 0xe0;
            getOutBuff()[4] = 0x3d;
            getOutBuff()[5] = (char) (address >> 0x10);
            getOutBuff()[6] = (char) ((address - ((address >> 0x10) << 0x10)) >> 8);
            getOutBuff()[7] = (char) (address - ((address >> 8) << 8));
            getOutBuff()[8] = (char) length;
            uint index = 0;
            while (index < length)
            {
                data[9 + index] = Payload[index];
                index++;
            }

            // this.J2534_Send(6, 0x40, data, ((uint) 9) + length);
            write(9 + length);

            bool flag2 = false;
            int timeout = 0;
            while (!flag2 && (timeout < calculatedTimeout))
            {
                uint pNumMsgs = 1;

                Message pRecvMsg;

                int GlobalChannelID = 0;                                               //  костыли
                status = PassThruReadMsgs(GlobalChannelID, pRecvMsg, pNumMsgs, 0);     // костыли

                if (status != PassThru::Status::NoError)
                {
                    if ((status != PassThru::Status::Timeout) && (status != PassThru::Status::BufferEmpty))
                    {
                        // throw new ApplicationException("Error: Mode3D - PassThruReadMsgs = " + this.J2534_Error());
                    }
                    QThread::sleep(1);
                    timeout++;
                }
                else
                {
                    uint num7 = pRecvMsg.m_data[0];
                    if (num7 == 0x7d)
                    {
                        flag2 = true;
                        flag = true;
                        break;
                    }
                    if ((num7 == 0x7f) && (pRecvMsg.m_data[2] == 0x78))
                    {
                        QThread::sleep(2);
                    }
                }
                // Marshal.FreeHGlobal(pRecvMsg);
            }
        }
        return flag;
    }

    bool ISO15765_Mode3D_Test_Query(uint e85, uint afr, uint fuel_temp)
    {
        getOutBuff()[0] = 0;
        getOutBuff()[1] = 0;
        getOutBuff()[2] = 7;
        getOutBuff()[3] = 0xe0;
        getOutBuff()[4] = 0x3d;
        getOutBuff()[5] = 0;
        getOutBuff()[6] = (char) e85;
        getOutBuff()[7] = (char) afr;
        getOutBuff()[8] = (char) fuel_temp;

        // this.Connect();
        // this.J2534_Send(6, 0x40, data, 9);
        write(9);
        return true;
    }

    int Write_and_Verify(uint address, QByteArray Payload, uint length)
    {
        int num = 0;
        uint num2 = 0;
        while (true)
        {
            bool flag = false;
            uint num3 = length - num2;
            if (num3 > 0x40)
            {
                num3 = 0x40;
            }
            // QByteArray destinationArray /*= new byte[length]*/;




            QByteArray data(num, (char)0x00);

            QByteArray destinationArray(Payload.mid(num2, num3) + QByteArray(length - (num3 - num2), (char)0x00));


            // Array.ConstrainedCopy(Payload, (int) num2, destinationArray, 0, (int) num3);

            if (!ISO15765_Mode3D_Query(address + num2, destinationArray.data(), num3, 0))
            {
                flag = true;
                num++;
            }
            else
            {
                QByteArray buffer2 = ISO15765_Mode23_Query(address + num2, num3);
                if (buffer2.isEmpty())
                {
                    flag = true;
                    num++;
                }
                else
                {
                    uint index = 0;
                    while (true)
                    {
                        if (index >= num3)
                        {
                            num2 += num3;
                            break;
                        }
                        if (buffer2[index] != Payload[num2 + index])
                        {
                            flag = true;
                            num++;
                        }
                        index++;
                    }
                }
            }
            if (!flag || (num >= 10))
            {
                // this.total_retry_count += (uint) num;
                if (num2 >= length)
                {
                    return ((num < 5) ? 0 : -1);
                }
            }
        }
    }

    uint ECU_ROM_checksum = 0xbfff0;
    uint Current_Alt_Map = 0x805018;
    // uint
    //         uint
    //         uint

    void Connect()
    {
        QByteArray buffer2;

        int index = 0;
        while (true)
        {
            if (index >= 4)
            {

                // Current_Alt_Map = ISO15765_Mode23_Query(0x805018, 1)[0];

                // Write_Debug_Log_Entry(Environment.NewLine + ".. current alt map: " + this.Current_Alt_Map.ToString());
                // this.Populate_MapSelector();
                // Write_Debug_Log_Entry(Environment.NewLine + ".. poplate_mapselector");
                uint num = 0;
                // foreach (string str in this.MapSelector.Items)
                // {
                //     if (!str.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
                //     {
                //         num += uint.Parse(((Hashtable) this.maps[str])["x-axis-elements"].ToString());
                //     }
                // }


                    // Write_Activity_Log_Entry("80501A: 69");
                    // Write_Debug_Log_Entry(Environment.NewLine + ".. writing live tuner flag");
                QByteArray payload(1, (char)0x45);
                    if (Write_and_Verify(0x80501a, payload, 1) != 0)
                    {
                        // throw new ApplicationException("Error: Retry rewrite exceeded for 0x80501A\n
                        // ECU Memory inconsistant!!\n
                        // You should immediatley stop tuning and power off the car to clear Live Tuning state");
                    }
                    // Write_Debug_Log_Entry(Environment.NewLine + ".. wrote live tuner flag");
                    foreach (string str2 in this.MapSelector.Items)
                    {
                        if (!str2.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
                        {
                            byte[] buffer3;
                            Write_Debug_Log_Entry("... uploading " + str2);
                            this.Write_Activity_Log_Entry("... uploading " + str2);
                            this.label1.Text = "... uploading " + str2;
                            Hashtable hashtable = (Hashtable) this.maps[str2];
                            if (hashtable["lt-memory-blk"] == null)
                            {
                                throw new ApplicationException("Error: No lt-memory-blk attribute setup for map \"" + str2 + "\"");
                            }
                            uint address = uint.Parse(hashtable["lt-memory-blk"].ToString(), NumberStyles.HexNumber);
                            uint num4 = (uint) hashtable["x-axis-elements"];
                            uint num5 = (uint) hashtable["y-axis-elements"];
                            uint num6 = (uint) hashtable["element-size"];
                            if (hashtable["header"] != null)
                            {
                                uint length = (uint) ((byte[]) hashtable["header"]).Length;
                                buffer3 = LocalROM_Query(uint.Parse(hashtable["address"].ToString(), NumberStyles.HexNumber) - length, length);
                                Write_Activity_Log_Entry(address.ToString("X") + ": " + BitConverter.ToString(buffer3));
                                if (Write_and_Verify(address, buffer3, length) != 0)
                                {
                                    throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
                                }
                                address += length;
                            }
                            for (int i = 0; i < num4; i++)
                            {
                                buffer3 = this.LocalROM_Query(uint.Parse(hashtable["address"].ToString(), NumberStyles.HexNumber) + ((uint) ((i * num5) * num6)), num6 * num5);
                                this.Write_Activity_Log_Entry(address.ToString("X") + ": " + BitConverter.ToString(buffer3));
                                if (this._j2534.Write_and_Verify(address, buffer3, num6 * num5) != 0)
                                {
                                    throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
                                }
                                address += num6 * num5;
                                this.progressBar1.PerformStep();
                                Thread.Sleep(1);
                                Application.DoEvents();
                            }
                        }
                    }


                    // Write_Activity_Log_Entry("... writing memory pointer overrides");
                    // Write_Debug_Log_Entry("... writing memory pointer overrides");
                    foreach (string str3 in this.MapSelector.Items)
                    {
                        if (!str3.Equals("---------------------- 'ALL' MAPS BELOW ----------------------"))
                        {
                            Hashtable hashtable2 = (Hashtable) this.maps[str3];
                            if (!hashtable2["type"].ToString().Equals("1D"))
                            {
                                if (hashtable2["lt-memory-ptr"] == null)
                                {
                                    throw new ApplicationException("Error: No lt-memory-ptr attribute setup for map \"" + str3 + "\"");
                                }
                                uint address = uint.Parse(hashtable2["lt-memory-ptr"].ToString(), NumberStyles.HexNumber);
                                uint num10 = uint.Parse(hashtable2["lt-memory-blk"].ToString(), NumberStyles.HexNumber);
                                byte[] buffer4 = new byte[] { (byte) (num10 >> 0x18), (byte) ((num10 - ((num10 >> 0x18) << 0x18)) >> 0x10), (byte) ((num10 - ((num10 >> 0x10) << 0x10)) >> 8), (byte) (num10 - ((num10 >> 8) << 8)) };
                                string[] textArray1 = new string[] { address.ToString("X"), " (", str3, "): ", BitConverter.ToString(buffer4) };
                                this.Write_Activity_Log_Entry(string.Concat(textArray1));
                                if (this._j2534.Write_and_Verify(address, buffer4, 4) != 0)
                                {
                                    throw new ApplicationException("Error: Retry rewrite exceeded for 0x" + address.ToString("X") + "\nECU Memory inconsistant!!\nYou should immediatley stop tuning and power off the car to clear Live Tuning state");
                                }
                            }
                        }
                    }

                break;
            }
            if (buffer[index] != buffer2[index])
            {
                throw new ApplicationException("Error: ECU Checksum doesn't match ROM checksum\nPlease try again with the actual ROM file you last flashed...");
            }
            index++;
        }
        this.b_Connect.Text = "Disconnect";
        this.b_Open_ROM_File.Enabled = false;
        this.b_RAM_ROM_Compare.Enabled = true;
        this.live_tuner_connected = true;
        this.button1.Enabled = false;
    }

};

#endif // EVOX_DMA_H
