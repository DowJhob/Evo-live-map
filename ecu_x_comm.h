#ifndef ECU_X_COMM_H
#define ECU_X_COMM_H

#include <QObject>
#include "op20.h"

class _X_class: public OP20
{
    Q_OBJECT
public:
    _X_class();
    //    string J2534_Detect_OP20()
    //            {
    //                IntPtr intPtr = Marshal.AllocHGlobal(1024);
    //                if (PassThruIoctl(0u, 458756u, IntPtr.Zero, intPtr) == 0)
    //                {
    //                    return Marshal.PtrToStringAuto(intPtr);
    //                }
    //                return "";
    //            }


    void ISO15765_Mode23_Query(uint address, uint length)
    {
        byte array[12];
        byte *array2;
//        _connect();
        out_buff[0] = 0;
        out_buff[1] = 0;
        out_buff[2] = 7;
        out_buff[3] = 0xE0;
        out_buff[4] = 0x23;
        out_buff[5] = (byte)(address >> 16);
        out_buff[6] = (byte)((address - (address >> 16 << 16)) >> 8);
        out_buff[7] = (byte)(address - (address >> 8 << 8));
        out_buff[8] = (byte)length;
        int num = 0;
        uint num2 = 0;
        bool flag = false;
        //    IntPtr intPtr = Marshal.AllocHGlobal(1048);
        while (!flag && num2 < 5)
        {
            num2++;
            write( 9);   //----------------------------------------------------------   // Зашлем адрес и длинну
            bool flag2 = false;
            int num3 = 15 * (1 + length / 7);
            int num4 = 0;
            while (!flag2 && num4 < num3)
            {
                ulong pNumMsgs = 1;
//                try
                {
                    if ((num = j2534->PassThruReadMsgs(chanID, &rx_msg[0], &pNumMsgs, 10)) != 0)
                //    read(); //----------------------------------------------------------   // Прочитаем ответ контроллера (результат будет в in_buf)
                    {
                        switch (num)
                        {
                        case ERR_TIMEOUT:
                        case ERR_BUFFER_EMPTY: num4 += 10; break;
                        case ERR_INVALID_CHANNEL_ID:
                        case ERR_DEVICE_NOT_CONNECTED: flag2 = true; break;
                        default:;
 //                           throw new ApplicationException("Error: Mode23 - PassThruReadMsgs = " + J2534_Error());
                        }
                    }
                  else
                    {
                        uint num5 = rx_msg[0].Data[0];                                //rx_msg.Data[0]  ???
                        if (num5 == 0x63 && rx_msg[0].Timestamp == length + 5)          //rx_msg.Timestamp  ???
                        {
                            array2 = new byte[length];
                            for (int i = 0; i < length; i++)
                            {
                                array2[i] = rx_msg[0].Data[i + 1];
                            }
                            flag2 = true;
                            flag = true;
                        }
                        else if (num5 == 127 && rx_msg[0].Data[2] == 120)
                        {
                            QThread::msleep(2);
                        }
                    }
                }
     //           catch(1) //(Exception)
                {
                    num2++;
                }
            }
        }
//        Marshal.FreeHGlobal(intPtr);
        if (num2 > 1)
        {
//////            total_mode23_count += num2 - 1;
        }
//        return array2;
    }

    bool ISO15765_Mode3D_Query(uint address, uchar* Payload, uint length, uint requested_timeout)
    {
        byte* array = new byte[9 + length];
        int num = 0;
        uint num2 = 0u;
        bool flag = false;
        //Connect();
        while (!flag)
        {
            num2++;
            array[0] = 0;
            array[1] = 0;
            array[2] = 7;
            array[3] = 0xE0;
            array[4] = 0x3D;
            array[5] = (byte)(address >> 16);
            array[6] = (byte)((address & 0xFF00) >> 8);
            array[7] = (byte)(address & 0xFF);
            array[8] = (byte)length;
            for (int i = 0; i < length; i++)
            {
                array[9 + i] = Payload[i];
            }
            J2534_Send(6u, 64u, array, 9 + length);
            bool flag2 = false;
            int num3 = 0;
            int num4 = (int)(32 + length / 7u);
            if (requested_timeout != 0)
            {
                num4 = (int)requested_timeout;
            }
//            IntPtr intPtr;
            for (; !flag2 && num3 < num4; )
            {
                uint pNumMsgs = 1;
//                intPtr = Marshal.AllocHGlobal(4120);
                if ((num = PassThruReadMsgs(chanID, rx_msg, &pNumMsgs, 0)) != 0)
                {
                    if (num == ERR_TIMEOUT || num == ERR_BUFFER_EMPTY)
                    {
                        QThread::msleep(1);
                        num3++;
                        continue;
                    }
                    throw new ApplicationException("Error: Mode3D - PassThruReadMsgs = " + J2534_Error());
                }
                switch (rx_msg->Data[4])
                {
                case 0x7D: break;
                case 0x7F:
                    if (rx_msg->Data[6] == 0x78)
                    {
                        QThread::msleep(2);
                    }
                    continue;
                default:
                    continue;
                }
                flag2 = true;
                flag = true;
                break;
            }
        }
        return flag;
    }

    bool ISO15765_Mode3D_Test_Query(uint e85, uint afr, uint fuel_temp)
    {
        byte* array = new byte[9];
        //Connect();
        array[0] = 0;
        array[1] = 0;
        array[2] = 7;
        array[3] = 0xE0;
        array[4] = 0x3D;
        array[5] = 0;
        array[6] = (byte)e85;
        array[7] = (byte)afr;
        array[8] = (byte)fuel_temp;
        J2534_Send(6u, 64, array, 9);
        return true;
    }

   void ISO15765_Mode23_Fetch(uint address, uint length)  // Делим на чанки по 64 байта?
    {
        uint ptr_num = 0;
        byte* fetched_array = new byte[length];
        do
        {
            uint current_chunk_size = length - ptr_num;
            if (current_chunk_size > 64)
                current_chunk_size = 64;
            ISO15765_Mode23_Query(address + ptr_num, current_chunk_size);
            //if (array2 == null)
                //return null;
            memcpy(fetched_array + ptr_num, rx_msg[0].Data, current_chunk_size);
            ptr_num += current_chunk_size;
        }
        while (ptr_num < length);
 //       return fetched_array;
    }

    int Write_and_Verify(uint address, byte* Payload, uint length)
    {
        int num = 0;
        uint num2 = 0;
        while (true)
        {
            bool flag = false;
            uint num3 = length - num2;
            if (num3 > 64)
            {
                num3 = 64;
            }
            byte* array = new byte[length];
            Array.ConstrainedCopy(Payload, (int)num2, array, 0, (int)num3);
            if (!ISO15765_Mode3D_Query(address + num2, array, num3, 0))
            {
                flag = true;
                num++;
            }
            else
            {
                byte* array2 = ISO15765_Mode23_Query(address + num2, num3);
                if (array2 == null)
                {
                    flag = true;
                    num++;
                }
                else
                {
                    for (uint num4 = 0; num4 < num3; num4++)
                    {
                        if (array2[num4] != Payload[num2 + num4])
                        {
                            flag = true;
                            num++;
                        }
                    }
                    num2 += num3;
                }
            }
            if (!flag || num >= 10)
            {
                total_retry_count += (uint)num;
                if (num2 >= length)
                {
                    break;
                }
            }
        }
        if (num >= 5)
        {
            return -1;
        }
        return 0;
    }

};

#endif // ECU_X_COMM_H
