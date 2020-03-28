#ifndef EVOX_H
#define EVOX_H

#include <QObject>

class evoX
{
public:
    evoX();
    const int ISO15765 = 6;
    const int ISO15765_FRAME_PAD = 0x40;
    const int BLOCKING = 0x1_0000;
    uint GlobalDeviceID;
    uint GlobalChannelID;
    uint total_mode23_count;
    uint total_retry_count;
    void Connect()
    {
        if ((GlobalDeviceID == 0) | (GlobalChannelID == 0))
        {
            uint num3;
            uint globalDeviceID = GlobalDeviceID;
            uint globalChannelID = GlobalChannelID;
            this.Disconnect();
            if (PassThruOpen(nullptr, out globalDeviceID) != 0)
            {
                throw new ApplicationException("Error: PassThruOpen - " + J2534_Error());
            }
            this.GlobalDeviceID = globalDeviceID;
            if (PassThruConnect(GlobalDeviceID, 6, 0, 0x7_a120, out globalChannelID) != 0)
            {
                throw new ApplicationException("Error: PassThruConnect - " + J2534_Error());
            }
            GlobalChannelID = globalChannelID;
            if (PassThruIoctl(GlobalChannelID, 7, IntPtr.Zero, IntPtr.Zero) != 0)
            {
                throw new ApplicationException("Error: PassThruIoctl - " + J2534_Error());
            }
            if (PassThruIoctl(GlobalChannelID, 8, IntPtr.Zero, IntPtr.Zero) != 0)
            {
                throw new ApplicationException("Error: PassThruIoctl - " + J2534_Error());
            }
            if (PassThruIoctl(GlobalChannelID, 10, IntPtr.Zero, IntPtr.Zero) != 0)
            {
                throw new ApplicationException("Error: PassThruIoctl - " + J2534_Error());
            }
            IntPtr ptr = Marshal.AllocHGlobal(0x10);
            Marshal.WriteInt32(ptr, 0, 30);
            Marshal.WriteInt32(ptr, 4, 0x20);
            Marshal.WriteInt32(ptr, 8, 0x1f);
            Marshal.WriteInt32(ptr, 12, 0);
            IntPtr ptr2 = Marshal.AllocHGlobal(8);
            Marshal.WriteInt32(ptr2, 0, 2);
            Marshal.WriteInt32(ptr2, 4, ptr.ToInt32());
            PassThruIoctl(GlobalChannelID, 2, ptr2, IntPtr.Zero);
            IntPtr ptr3 = Marshal.AllocHGlobal(0x1c);
            Marshal.WriteInt32(ptr3, 0, 6);
            Marshal.WriteInt32(ptr3, 4, 0);
            Marshal.WriteInt32(ptr3, 8, 0x40);
            Marshal.WriteInt32(ptr3, 12, 0);
            Marshal.WriteInt32(ptr3, 0x10, 4);
            Marshal.WriteInt32(ptr3, 20, 0);
            Marshal.WriteByte(ptr3, 0x18, 0xff);
            Marshal.WriteByte(ptr3, 0x19, 0xff);
            Marshal.WriteByte(ptr3, 0x1a, 0xff);
            Marshal.WriteByte(ptr3, 0x1b, 0xff);
            IntPtr ptr4 = Marshal.AllocHGlobal(0x1c);
            Marshal.WriteInt32(ptr4, 0, 6);
            Marshal.WriteInt32(ptr4, 4, 0);
            Marshal.WriteInt32(ptr4, 8, 0x40);
            Marshal.WriteInt32(ptr4, 12, 0);
            Marshal.WriteInt32(ptr4, 0x10, 4);
            Marshal.WriteInt32(ptr4, 20, 0);
            Marshal.WriteByte(ptr4, 0x18, 0);
            Marshal.WriteByte(ptr4, 0x19, 0);
            Marshal.WriteByte(ptr4, 0x1a, 7);
            Marshal.WriteByte(ptr4, 0x1b, 0xe8);
            IntPtr ptr5 = Marshal.AllocHGlobal(0x1c);
            Marshal.WriteInt32(ptr5, 0, 6);
            Marshal.WriteInt32(ptr5, 4, 0);
            Marshal.WriteInt32(ptr5, 8, 0x40);
            Marshal.WriteInt32(ptr5, 12, 0);
            Marshal.WriteInt32(ptr5, 0x10, 4);
            Marshal.WriteInt32(ptr5, 20, 0);
            Marshal.WriteByte(ptr5, 0x18, 0);
            Marshal.WriteByte(ptr5, 0x19, 0);
            Marshal.WriteByte(ptr5, 0x1a, 7);
            Marshal.WriteByte(ptr5, 0x1b, 0xe0);
            if (PassThruStartMsgFilter(GlobalChannelID, 3, ptr3, ptr4, ptr5, out num3) != 0)
            {
                throw new ApplicationException("Error: PassThruStartMsgFilter - " + J2534_Error());
            }
            Marshal.FreeHGlobal(ptr3);
            Marshal.FreeHGlobal(ptr4);
            Marshal.FreeHGlobal(ptr5);
            Thread.Sleep(100);
        }
    }
    void Disconnect()
    {
        if (this.GlobalChannelID > 0)
        {
            PassThruDisconnect(this.GlobalChannelID);
        }
        this.GlobalChannelID = 0;
        if (this.GlobalDeviceID > 0)
        {
            PassThruClose(this.GlobalDeviceID);
        }
        this.GlobalDeviceID = 0;
    }
    QByteArray ISO15765_Mode23_Fetch(uint address, uint length)
    {
        uint num = 0;
        uchar* destinationArray = new uchar[length];
        while (true)
        {
            uint num2 = length - num;
            if (num2 > 0x40)
            {
                num2 = 0x40;
            }
            uchar* sourceArray = ISO15765_Mode23_Query(address + num, num2);
            if (sourceArray == null)
            {
                return null;
            }
            Array.ConstrainedCopy(sourceArray, 0, destinationArray, (int) num, (int) num2);
            num += num2;
            if (num >= length)
            {
                return destinationArray;
            }
        }
    }
    QByteArray ISO15765_Mode23_Query(uint address, uint length)
    {
        uchar* data = new uchar[15]{ 0, 0, 7, 0xe0, 0x23, (uchar) (address >> 0x10), (uchar) ((address - ((address >> 0x10) << 0x10)) >> 8), (uchar) (address - ((address >> 8) << 8)), (uchar) length };
        uchar* buffer2 = nullptr;
        Connect();
        int num = 0;
        uint num2 = 0;
        bool flag = false;
        IntPtr pRecvMsg = Marshal.AllocHGlobal(0x418);
        while (!flag && (num2 < 5))
        {
            num2++;
            this.J2534_Send(6, 0x40, data, 9);
            bool flag2 = false;
            int num3 = (int) (15 * (1 + (length / 7)));
            int num4 = 0;
            while (!flag2 && (num4 < num3))
            {
                uint pNumMsgs = 1;
                try
                {
                    num = PassThruReadMsgs(GlobalChannelID, pRecvMsg, out pNumMsgs, 10);
                    if (num != 0)
                    {
                        if ((num == 9) || (num == 0x10))
                        {
                            num4 += 10;
                        }
                        else
                        {
                            if ((num != 8) && (num != 2))
                            {
                                throw new ApplicationException("Error: Mode23 - PassThruReadMsgs = " + this.J2534_Error());
                            }
                            flag2 = true;
                        }
                    }
                    else
                    {
                        uint num6 = Marshal.ReadByte(pRecvMsg, 0x1c);
                        if ((num6 != 0x63) || (Marshal.ReadInt32(pRecvMsg, 0x10) != (length + 5)))
                        {
                            if ((num6 == 0x7f) && (Marshal.ReadByte(pRecvMsg, 30) == 120))
                            {
                                Thread.Sleep(2);
                            }
                        }
                        else
                        {
                            buffer2 = new byte[length];
                            int index = 0;
                            while (true)
                            {
                                if (index >= length)
                                {
                                    flag2 = true;
                                    flag = true;
                                    break;
                                }
                                buffer2[index] = Marshal.ReadByte(pRecvMsg, index + 0x1d);
                                index++;
                            }
                        }
                    }
                }
                catch (Exception)
                {
                    num2++;
                }
            }
        }
        Marshal.FreeHGlobal(pRecvMsg);
        if (num2 > 1)
        {
            this.total_mode23_count += num2 - 1;
        }
        return buffer2;
    }
    bool ISO15765_Mode3D_Query(uint address, uchar* Payload, uint length, uint requested_timeout)
    {
        uchar* data = new uchar[9 + length];
        int num = 0;
        uint num2 = 0;
        bool flag = false;
        Connect();
        while (!flag)
        {
            num2++;
            data[0] = 0;
            data[1] = 0;
            data[2] = 7;
            data[3] = 0xe0;
            data[4] = 0x3d;
            data[5] = (uchar) (address >> 0x10);
            data[6] = (uchar) ((address - ((address >> 0x10) << 0x10)) >> 8);
            data[7] = (uchar) (address - ((address >> 8) << 8));
            data[8] = (uchar) length;
            int index = 0;
            while (true)
            {
                if (index >= length)
                {
                    J2534_Send(6, 0x40, data, 9 + length);
                    bool flag2 = false;
                    int num3 = 0;
                    int num4 = (int) (0x20 + (length / 7));
                    if (requested_timeout != 0)
                    {
                        num4 = (int) requested_timeout;
                    }
                    while (!flag2 && (num3 < num4))
                    {
                        uint pNumMsgs = 1;
                        IntPtr pRecvMsg = Marshal.AllocHGlobal(0x1018);
                        num = PassThruReadMsgs(GlobalChannelID, pRecvMsg, out pNumMsgs, 0);
                        if (num != 0)
                        {
                            if ((num != 9) && (num != 0x10))
                            {
                                throw new ApplicationException("Error: Mode3D - PassThruReadMsgs = " + this.J2534_Error());
                            }
                            Thread.Sleep(1);
                            num3++;
                        }
                        else
                        {
                            uint num7 = Marshal.ReadByte(pRecvMsg, 0x1c);
                            if (num7 == 0x7d)
                            {
                                flag2 = true;
                                flag = true;
                                break;
                            }
                            if ((num7 == 0x7f) && (Marshal.ReadByte(pRecvMsg, 30) == 120))
                            {
                                Thread.Sleep(2);
                            }
                        }
                        Marshal.FreeHGlobal(pRecvMsg);
                    }
                    break;
                }
                data[9 + index] = Payload[index];
                index++;
            }
        }
        return flag;
    }
    bool ISO15765_Mode3D_Test_Query(uint e85, uint afr, uint fuel_temp)
    {
        byte[] data = new byte[] { 0, 0, 7, 0xe0, 0x3d, 0, (byte) e85, (byte) afr, (byte) fuel_temp };
        this.Connect();
        this.J2534_Send(6, 0x40, data, 9);
        return true;
    }
    QString J2534_Detect_OP20()
    {
        IntPtr pOutput = Marshal.AllocHGlobal(0x400);
        return ((PassThruIoctl(0, 0x7_0004, IntPtr.Zero, pOutput) != 0) ? "" : Marshal.PtrToStringAuto(pOutput));
    }
    QString J2534_Error()
    {
        IntPtr error = Marshal.AllocHGlobal(0x100);
        PassThruGetLastError(error);
        string str = Marshal.PtrToStringAnsi(error);
        Marshal.FreeHGlobal(error);
        return str;
    }
    int J2534_Send(uint ProtocolID, uint Flags, uchar* Data, uint DataSize)
    {
        uint pNumMsgs = 1;
        IntPtr ptr = Marshal.AllocHGlobal((int) (0x18 + DataSize));
        Marshal.WriteInt32(ptr, 0, (int) ProtocolID);
        Marshal.WriteInt32(ptr, 4, 0);
        Marshal.WriteInt32(ptr, 8, (int) Flags);
        Marshal.WriteInt32(ptr, 12, 0);
        Marshal.WriteInt32(ptr, 0x10, (int) DataSize);
        Marshal.WriteInt32(ptr, 20, 0);
        for (int i = 0; i < DataSize; i++)
        {
            Marshal.WriteByte(ptr, 0x18 + i, Data[i]);
        }
        if (PassThruWriteMsgs(this.GlobalChannelID, ptr, out pNumMsgs, 0) != 0)
        {
            throw new ApplicationException("Error: PassThruWriteMsgs - " + this.J2534_Error());
        }
        Marshal.FreeHGlobal(ptr);
        return 0;
    }
    int Write_and_Verify(uint address, uchar* Payload, uint length)
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
            uchar* destinationArray = new uchar[length];
//            Array.ConstrainedCopy(Payload, (int) num2, destinationArray, 0, (int) num3);
            if (!ISO15765_Mode3D_Query(address + num2, Payload, num3, 0))
            {
                flag = true;
                num++;
            }
            else
            {
                uchar* buffer2 = ISO15765_Mode23_Query(address + num2, num3);
                if (buffer2 == nullptr)
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
                total_retry_count += (uint) num;
                if (num2 >= length)
                {
                    return ((num < 5) ? 0 : -1);
                }
            }
        }
    }

private:
        const int CAN = 5;
        const int SET_CONFIG = 2;
        const int ISO15765_BS = 30;
        const int ISO15765_STMIN = 0x1f;
        const int ISO15765_FIRST_FRAME = 1;
        const int CLEAR_TX_BUFFER = 7;
        const int CLEAR_RX_BUFFER = 8;
        const int CLEAR_MSG_FILTERS = 10;
        const int PASS_FILTER = 1;
        const int FLOW_CONTROL_FILTER = 3;
        const int ERR_TIMEOUT = 9;
        const int ERR_BUFFER_EMPTY = 0x10;
        const int ERR_INVALID_CHANNEL_ID = 2;
        const int ERR_DEVICE_NOT_CONNECTED = 8;
        const int ERR_DEVICE_IN_USE = 14;
        const uint TX_IOCTL_BASE = 0x7_0000;
        const uint TX_IOCTL_GET_DEVICE_INSTANCES = 0x7_0004;
        const uint TX_IOCTL_SET_DLL_DEBUG_FLAGS = 0x7_0001;
        const uint TX_IOCTL_DLL_DEBUG_FLAG_J2534_CALLS = 1;
        const uint max_pdu_length = 0x40;

};

#endif // EVOX_H
