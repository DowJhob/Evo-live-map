#ifndef FTDI_TYPES_H
#define FTDI_TYPES_H
#include <windows.h>

typedef PVOID	FT_HANDLE;
typedef ULONG	FT_STATUS;
typedef struct _ft_device_list_info_node {
DWORD Flags;
DWORD Type;
DWORD ID;
DWORD LocId;
char SerialNumber[16];
char Description[64];
FT_HANDLE ftHandle;
} FT_DEVICE_LIST_INFO_NODE;




/////////////////
// API Functions
/////////////////

#ifdef FTD2XX_LIB
#define PT_CALL
#define PT_API EXTERN_C
#else
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#define PT_CALL __stdcall
#else
#define PT_CALL
#endif
#define PT_API EXTERN_C
#endif

typedef FT_STATUS (PT_CALL PF_FT_Open)(int, FT_HANDLE *);
typedef FT_STATUS (PT_CALL PF_FT_OpenEx)(PVOID , DWORD , FT_HANDLE *);
typedef FT_STATUS (PT_CALL PF_FT_CreateDeviceInfoList)(LPDWORD);
typedef FT_STATUS (PT_CALL PF_FT_GetDeviceInfoList)(FT_DEVICE_LIST_INFO_NODE*,LPDWORD);
typedef FT_STATUS (PT_CALL PF_FT_ListDevices)(PVOID,PVOID,DWORD);
typedef FT_STATUS (PT_CALL PF_FT_Close)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_Read)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
typedef FT_STATUS (PT_CALL PF_FT_Write)(FT_HANDLE,LPVOID,DWORD,LPDWORD);
typedef FT_STATUS (PT_CALL PF_FT_IoCtl)(FT_HANDLE,DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD,LPOVERLAPPED);
typedef FT_STATUS (PT_CALL PF_FT_SetBaudRate)(FT_HANDLE,ULONG);
typedef FT_STATUS (PT_CALL PF_FT_SetDivisor)(FT_HANDLE,USHORT);
typedef FT_STATUS (PT_CALL PF_FT_SetDataCharacteristics)(FT_HANDLE,UCHAR,UCHAR,UCHAR);
typedef FT_STATUS (PT_CALL PF_FT_SetFlowControl)(FT_HANDLE,USHORT,UCHAR,UCHAR);
typedef FT_STATUS (PT_CALL PF_FT_ResetDevice)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_SetDtr)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_ClrDtr)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_SetRts)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_ClrRts)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_GetModemStatus)(FT_HANDLE,ULONG *);
typedef FT_STATUS (PT_CALL PF_FT_SetChars)(FT_HANDLE,UCHAR,UCHAR,UCHAR,UCHAR);
typedef FT_STATUS (PT_CALL PF_FT_Purge)(FT_HANDLE,ULONG);
typedef FT_STATUS (PT_CALL PF_FT_SetTimeouts)(FT_HANDLE,ULONG,ULONG);
typedef FT_STATUS (PT_CALL PF_FT_GetQueueStatus)(FT_HANDLE,DWORD *);
typedef FT_STATUS (PT_CALL PF_FT_SetEventNotification)(FT_HANDLE,DWORD,PVOID);
typedef FT_STATUS (PT_CALL PF_FT_GetStatus)(FT_HANDLE,DWORD *,DWORD *,DWORD *);
typedef FT_STATUS (PT_CALL PF_FT_SetBreakOn)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_SetBreakOff)(FT_HANDLE);
typedef FT_STATUS (PT_CALL PF_FT_SetWaitMask)(FT_HANDLE,DWORD);
typedef FT_STATUS (PT_CALL PF_FT_WaitOnMask)(FT_HANDLE,DWORD *);
typedef FT_STATUS (PT_CALL PF_FT_GetEventStatus)(FT_HANDLE,DWORD *);
typedef FT_STATUS (PT_CALL PF_FT_ReadEE)(FT_HANDLE,DWORD,LPWORD);
typedef FT_STATUS (PT_CALL PF_FT_WriteEE)(FT_HANDLE,DWORD,WORD);
typedef FT_STATUS (PT_CALL PF_FT_EraseEE)(FT_HANDLE);
//------------------------------------------
typedef FT_STATUS (PT_CALL PF_FT_SetLatencyTimer)(FT_HANDLE, UCHAR  );
typedef FT_STATUS (PT_CALL PF_FT_GetLatencyTimer)(FT_HANDLE, PUCHAR );
typedef FT_STATUS (PT_CALL PF_FT_SetBitMode)( FT_HANDLE, UCHAR, UCHAR );
typedef FT_STATUS (PT_CALL PF_FT_GetBitMode)( FT_HANDLE, PUCHAR );
typedef FT_STATUS (PT_CALL PF_FT_SetUSBParameters)(FT_HANDLE, ULONG, ULONG );
typedef FT_STATUS (PT_CALL PF_FT_SetDeadmanTimeout)( FT_HANDLE, ULONG);

typedef void (*PF_StatusCallback)(const char *,int,int);












//
// Device status
//
enum {
    FT_OK,
    FT_INVALID_HANDLE,
    FT_DEVICE_NOT_FOUND,
    FT_DEVICE_NOT_OPENED,
    FT_IO_ERROR,
    FT_INSUFFICIENT_RESOURCES,
    FT_INVALID_PARAMETER,
    FT_INVALID_BAUD_RATE,

    FT_DEVICE_NOT_OPENED_FOR_ERASE,
    FT_DEVICE_NOT_OPENED_FOR_WRITE,
    FT_FAILED_TO_WRITE_DEVICE,
    FT_EEPROM_READ_FAILED,
    FT_EEPROM_WRITE_FAILED,
    FT_EEPROM_ERASE_FAILED,
    FT_EEPROM_NOT_PRESENT,
    FT_EEPROM_NOT_PROGRAMMED,
    FT_INVALID_ARGS,
    FT_NOT_SUPPORTED,
    FT_OTHER_ERROR,
    FT_DEVICE_LIST_NOT_READY,
};


#define FT_SUCCESS(status) ((status) == FT_OK)

//
// FT_OpenEx Flags
//

#define FT_OPEN_BY_SERIAL_NUMBER	1
#define FT_OPEN_BY_DESCRIPTION		2
#define FT_OPEN_BY_LOCATION			4

#define FT_OPEN_MASK (FT_OPEN_BY_SERIAL_NUMBER | \
                      FT_OPEN_BY_DESCRIPTION | \
                      FT_OPEN_BY_LOCATION)

//
// FT_ListDevices Flags (used in conjunction with FT_OpenEx Flags
//

#define FT_LIST_NUMBER_ONLY			0x80000000
#define FT_LIST_BY_INDEX			0x40000000
#define FT_LIST_ALL					0x20000000

#define FT_LIST_MASK (FT_LIST_NUMBER_ONLY|FT_LIST_BY_INDEX|FT_LIST_ALL)

//
// Baud Rates
//

#define FT_BAUD_300			300
#define FT_BAUD_600			600
#define FT_BAUD_1200		1200
#define FT_BAUD_2400		2400
#define FT_BAUD_4800		4800
#define FT_BAUD_9600		9600
#define FT_BAUD_14400		14400
#define FT_BAUD_19200		19200
#define FT_BAUD_38400		38400
#define FT_BAUD_57600		57600
#define FT_BAUD_115200		115200
#define FT_BAUD_230400		230400
#define FT_BAUD_460800		460800
#define FT_BAUD_921600		921600

//
// Word Lengths
//

#define FT_BITS_8			(UCHAR) 8
#define FT_BITS_7			(UCHAR) 7

//
// Stop Bits
//

#define FT_STOP_BITS_1		(UCHAR) 0
#define FT_STOP_BITS_2		(UCHAR) 2

//
// Parity
//

#define FT_PARITY_NONE		(UCHAR) 0
#define FT_PARITY_ODD		(UCHAR) 1
#define FT_PARITY_EVEN		(UCHAR) 2
#define FT_PARITY_MARK		(UCHAR) 3
#define FT_PARITY_SPACE		(UCHAR) 4

//
// Flow Control
//

#define FT_FLOW_NONE		0x0000
#define FT_FLOW_RTS_CTS		0x0100
#define FT_FLOW_DTR_DSR		0x0200
#define FT_FLOW_XON_XOFF	0x0400

//
// Purge rx and tx buffers
//
#define FT_PURGE_RX			1
#define FT_PURGE_TX			2

//
// Events
//

typedef void (*PFT_EVENT_HANDLER)(DWORD,DWORD);

#define FT_EVENT_RXCHAR			1
#define FT_EVENT_MODEM_STATUS	2
#define FT_EVENT_LINE_STATUS	4

//
// Timeouts
//

#define FT_DEFAULT_RX_TIMEOUT	300
#define FT_DEFAULT_TX_TIMEOUT	300

//
// Device types
//

typedef ULONG	FT_DEVICE;

enum {
    FT_DEVICE_BM,
    FT_DEVICE_AM,
    FT_DEVICE_100AX,
    FT_DEVICE_UNKNOWN,
    FT_DEVICE_2232C,
    FT_DEVICE_232R,
    FT_DEVICE_2232H,
    FT_DEVICE_4232H,
    FT_DEVICE_232H,
    FT_DEVICE_X_SERIES,
    FT_DEVICE_4222H_0,
    FT_DEVICE_4222H_1_2,
    FT_DEVICE_4222H_3,
    FT_DEVICE_4222_PROG,
    FT_DEVICE_900,
    FT_DEVICE_930,
    FT_DEVICE_UMFTPD3A,
};

//
// Bit Modes
//

#define FT_BITMODE_RESET					0x00
#define FT_BITMODE_ASYNC_BITBANG			0x01
#define FT_BITMODE_MPSSE					0x02
#define FT_BITMODE_SYNC_BITBANG				0x04
#define FT_BITMODE_MCU_HOST					0x08
#define FT_BITMODE_FAST_SERIAL				0x10
#define FT_BITMODE_CBUS_BITBANG				0x20
#define FT_BITMODE_SYNC_FIFO				0x40

//
// FT232R CBUS Options EEPROM values
//

#define FT_232R_CBUS_TXDEN					0x00	//	Tx Data Enable
#define FT_232R_CBUS_PWRON					0x01	//	Power On
#define FT_232R_CBUS_RXLED					0x02	//	Rx LED
#define FT_232R_CBUS_TXLED					0x03	//	Tx LED
#define FT_232R_CBUS_TXRXLED				0x04	//	Tx and Rx LED
#define FT_232R_CBUS_SLEEP					0x05	//	Sleep
#define FT_232R_CBUS_CLK48					0x06	//	48MHz clock
#define FT_232R_CBUS_CLK24					0x07	//	24MHz clock
#define FT_232R_CBUS_CLK12					0x08	//	12MHz clock
#define FT_232R_CBUS_CLK6					0x09	//	6MHz clock
#define FT_232R_CBUS_IOMODE					0x0A	//	IO Mode for CBUS bit-bang
#define FT_232R_CBUS_BITBANG_WR				0x0B	//	Bit-bang write strobe
#define FT_232R_CBUS_BITBANG_RD				0x0C	//	Bit-bang read strobe

//
// FT232H CBUS Options EEPROM values
//

#define FT_232H_CBUS_TRISTATE				0x00	//	Tristate
#define FT_232H_CBUS_TXLED					0x01	//	Tx LED
#define FT_232H_CBUS_RXLED					0x02	//	Rx LED
#define FT_232H_CBUS_TXRXLED				0x03	//	Tx and Rx LED
#define FT_232H_CBUS_PWREN					0x04	//	Power Enable
#define FT_232H_CBUS_SLEEP					0x05	//	Sleep
#define FT_232H_CBUS_DRIVE_0				0x06	//	Drive pin to logic 0
#define FT_232H_CBUS_DRIVE_1				0x07	//	Drive pin to logic 1
#define FT_232H_CBUS_IOMODE					0x08	//	IO Mode for CBUS bit-bang
#define FT_232H_CBUS_TXDEN					0x09	//	Tx Data Enable
#define FT_232H_CBUS_CLK30					0x0A	//	30MHz clock
#define FT_232H_CBUS_CLK15					0x0B	//	15MHz clock
#define FT_232H_CBUS_CLK7_5					0x0C	//	7.5MHz clock

//
// FT X Series CBUS Options EEPROM values
//

#define FT_X_SERIES_CBUS_TRISTATE			0x00	//	Tristate
#define FT_X_SERIES_CBUS_TXLED				0x01	//	Tx LED
#define FT_X_SERIES_CBUS_RXLED				0x02	//	Rx LED
#define FT_X_SERIES_CBUS_TXRXLED			0x03	//	Tx and Rx LED
#define FT_X_SERIES_CBUS_PWREN				0x04	//	Power Enable
#define FT_X_SERIES_CBUS_SLEEP				0x05	//	Sleep
#define FT_X_SERIES_CBUS_DRIVE_0			0x06	//	Drive pin to logic 0
#define FT_X_SERIES_CBUS_DRIVE_1			0x07	//	Drive pin to logic 1
#define FT_X_SERIES_CBUS_IOMODE				0x08	//	IO Mode for CBUS bit-bang
#define FT_X_SERIES_CBUS_TXDEN				0x09	//	Tx Data Enable
#define FT_X_SERIES_CBUS_CLK24				0x0A	//	24MHz clock
#define FT_X_SERIES_CBUS_CLK12				0x0B	//	12MHz clock
#define FT_X_SERIES_CBUS_CLK6				0x0C	//	6MHz clock
#define FT_X_SERIES_CBUS_BCD_CHARGER		0x0D	//	Battery charger detected
#define FT_X_SERIES_CBUS_BCD_CHARGER_N		0x0E	//	Battery charger detected inverted
#define FT_X_SERIES_CBUS_I2C_TXE			0x0F	//	I2C Tx empty
#define FT_X_SERIES_CBUS_I2C_RXF			0x10	//	I2C Rx full
#define FT_X_SERIES_CBUS_VBUS_SENSE			0x11	//	Detect VBUS
#define FT_X_SERIES_CBUS_BITBANG_WR			0x12	//	Bit-bang write strobe
#define FT_X_SERIES_CBUS_BITBANG_RD			0x13	//	Bit-bang read strobe
#define FT_X_SERIES_CBUS_TIMESTAMP			0x14	//	Toggle output when a USB SOF token is received
#define FT_X_SERIES_CBUS_KEEP_AWAKE			0x15	//


// Driver types
#define FT_DRIVER_TYPE_D2XX		0
#define FT_DRIVER_TYPE_VCP		1


//
// structure to hold program data for FT_EE_Program, FT_EE_ProgramEx, FT_EE_Read
// and FT_EE_ReadEx functions
//
typedef struct ft_program_data {

    DWORD Signature1;			// Header - must be 0x00000000
    DWORD Signature2;			// Header - must be 0xffffffff
    DWORD Version;				// Header - FT_PROGRAM_DATA version
    //			0 = original
    //			1 = FT2232 extensions
    //			2 = FT232R extensions
    //			3 = FT2232H extensions
    //			4 = FT4232H extensions
    //			5 = FT232H extensions

    WORD VendorId;				// 0x0403
    WORD ProductId;				// 0x6001
    char *Manufacturer;			// "FTDI"
    char *ManufacturerId;		// "FT"
    char *Description;			// "USB HS Serial Converter"
    char *SerialNumber;			// "FT000001" if fixed, or NULL
    WORD MaxPower;				// 0 < MaxPower <= 500
    WORD PnP;					// 0 = disabled, 1 = enabled
    WORD SelfPowered;			// 0 = bus powered, 1 = self powered
    WORD RemoteWakeup;			// 0 = not capable, 1 = capable
    //
    // Rev4 (FT232B) extensions
    //
    UCHAR Rev4;					// non-zero if Rev4 chip, zero otherwise
    UCHAR IsoIn;				// non-zero if in endpoint is isochronous
    UCHAR IsoOut;				// non-zero if out endpoint is isochronous
    UCHAR PullDownEnable;		// non-zero if pull down enabled
    UCHAR SerNumEnable;			// non-zero if serial number to be used
    UCHAR USBVersionEnable;		// non-zero if chip uses USBVersion
    WORD USBVersion;			// BCD (0x0200 => USB2)
    //
    // Rev 5 (FT2232) extensions
    //
    UCHAR Rev5;					// non-zero if Rev5 chip, zero otherwise
    UCHAR IsoInA;				// non-zero if in endpoint is isochronous
    UCHAR IsoInB;				// non-zero if in endpoint is isochronous
    UCHAR IsoOutA;				// non-zero if out endpoint is isochronous
    UCHAR IsoOutB;				// non-zero if out endpoint is isochronous
    UCHAR PullDownEnable5;		// non-zero if pull down enabled
    UCHAR SerNumEnable5;		// non-zero if serial number to be used
    UCHAR USBVersionEnable5;	// non-zero if chip uses USBVersion
    WORD USBVersion5;			// BCD (0x0200 => USB2)
    UCHAR AIsHighCurrent;		// non-zero if interface is high current
    UCHAR BIsHighCurrent;		// non-zero if interface is high current
    UCHAR IFAIsFifo;			// non-zero if interface is 245 FIFO
    UCHAR IFAIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IFAIsFastSer;			// non-zero if interface is Fast serial
    UCHAR AIsVCP;				// non-zero if interface is to use VCP drivers
    UCHAR IFBIsFifo;			// non-zero if interface is 245 FIFO
    UCHAR IFBIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IFBIsFastSer;			// non-zero if interface is Fast serial
    UCHAR BIsVCP;				// non-zero if interface is to use VCP drivers
    //
    // Rev 6 (FT232R) extensions
    //
    UCHAR UseExtOsc;			// Use External Oscillator
    UCHAR HighDriveIOs;			// High Drive I/Os
    UCHAR EndpointSize;			// Endpoint size
    UCHAR PullDownEnableR;		// non-zero if pull down enabled
    UCHAR SerNumEnableR;		// non-zero if serial number to be used
    UCHAR InvertTXD;			// non-zero if invert TXD
    UCHAR InvertRXD;			// non-zero if invert RXD
    UCHAR InvertRTS;			// non-zero if invert RTS
    UCHAR InvertCTS;			// non-zero if invert CTS
    UCHAR InvertDTR;			// non-zero if invert DTR
    UCHAR InvertDSR;			// non-zero if invert DSR
    UCHAR InvertDCD;			// non-zero if invert DCD
    UCHAR InvertRI;				// non-zero if invert RI
    UCHAR Cbus0;				// Cbus Mux control
    UCHAR Cbus1;				// Cbus Mux control
    UCHAR Cbus2;				// Cbus Mux control
    UCHAR Cbus3;				// Cbus Mux control
    UCHAR Cbus4;				// Cbus Mux control
    UCHAR RIsD2XX;				// non-zero if using D2XX driver
    //
    // Rev 7 (FT2232H) Extensions
    //
    UCHAR PullDownEnable7;		// non-zero if pull down enabled
    UCHAR SerNumEnable7;		// non-zero if serial number to be used
    UCHAR ALSlowSlew;			// non-zero if AL pins have slow slew
    UCHAR ALSchmittInput;		// non-zero if AL pins are Schmitt input
    UCHAR ALDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR AHSlowSlew;			// non-zero if AH pins have slow slew
    UCHAR AHSchmittInput;		// non-zero if AH pins are Schmitt input
    UCHAR AHDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BLSlowSlew;			// non-zero if BL pins have slow slew
    UCHAR BLSchmittInput;		// non-zero if BL pins are Schmitt input
    UCHAR BLDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BHSlowSlew;			// non-zero if BH pins have slow slew
    UCHAR BHSchmittInput;		// non-zero if BH pins are Schmitt input
    UCHAR BHDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR IFAIsFifo7;			// non-zero if interface is 245 FIFO
    UCHAR IFAIsFifoTar7;		// non-zero if interface is 245 FIFO CPU target
    UCHAR IFAIsFastSer7;		// non-zero if interface is Fast serial
    UCHAR AIsVCP7;				// non-zero if interface is to use VCP drivers
    UCHAR IFBIsFifo7;			// non-zero if interface is 245 FIFO
    UCHAR IFBIsFifoTar7;		// non-zero if interface is 245 FIFO CPU target
    UCHAR IFBIsFastSer7;		// non-zero if interface is Fast serial
    UCHAR BIsVCP7;				// non-zero if interface is to use VCP drivers
    UCHAR PowerSaveEnable;		// non-zero if using BCBUS7 to save power for self-powered designs
    //
    // Rev 8 (FT4232H) Extensions
    //
    UCHAR PullDownEnable8;		// non-zero if pull down enabled
    UCHAR SerNumEnable8;		// non-zero if serial number to be used
    UCHAR ASlowSlew;			// non-zero if A pins have slow slew
    UCHAR ASchmittInput;		// non-zero if A pins are Schmitt input
    UCHAR ADriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BSlowSlew;			// non-zero if B pins have slow slew
    UCHAR BSchmittInput;		// non-zero if B pins are Schmitt input
    UCHAR BDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR CSlowSlew;			// non-zero if C pins have slow slew
    UCHAR CSchmittInput;		// non-zero if C pins are Schmitt input
    UCHAR CDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR DSlowSlew;			// non-zero if D pins have slow slew
    UCHAR DSchmittInput;		// non-zero if D pins are Schmitt input
    UCHAR DDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ARIIsTXDEN;			// non-zero if port A uses RI as RS485 TXDEN
    UCHAR BRIIsTXDEN;			// non-zero if port B uses RI as RS485 TXDEN
    UCHAR CRIIsTXDEN;			// non-zero if port C uses RI as RS485 TXDEN
    UCHAR DRIIsTXDEN;			// non-zero if port D uses RI as RS485 TXDEN
    UCHAR AIsVCP8;				// non-zero if interface is to use VCP drivers
    UCHAR BIsVCP8;				// non-zero if interface is to use VCP drivers
    UCHAR CIsVCP8;				// non-zero if interface is to use VCP drivers
    UCHAR DIsVCP8;				// non-zero if interface is to use VCP drivers
    //
    // Rev 9 (FT232H) Extensions
    //
    UCHAR PullDownEnableH;		// non-zero if pull down enabled
    UCHAR SerNumEnableH;		// non-zero if serial number to be used
    UCHAR ACSlowSlewH;			// non-zero if AC pins have slow slew
    UCHAR ACSchmittInputH;		// non-zero if AC pins are Schmitt input
    UCHAR ACDriveCurrentH;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ADSlowSlewH;			// non-zero if AD pins have slow slew
    UCHAR ADSchmittInputH;		// non-zero if AD pins are Schmitt input
    UCHAR ADDriveCurrentH;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR Cbus0H;				// Cbus Mux control
    UCHAR Cbus1H;				// Cbus Mux control
    UCHAR Cbus2H;				// Cbus Mux control
    UCHAR Cbus3H;				// Cbus Mux control
    UCHAR Cbus4H;				// Cbus Mux control
    UCHAR Cbus5H;				// Cbus Mux control
    UCHAR Cbus6H;				// Cbus Mux control
    UCHAR Cbus7H;				// Cbus Mux control
    UCHAR Cbus8H;				// Cbus Mux control
    UCHAR Cbus9H;				// Cbus Mux control
    UCHAR IsFifoH;				// non-zero if interface is 245 FIFO
    UCHAR IsFifoTarH;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IsFastSerH;			// non-zero if interface is Fast serial
    UCHAR IsFT1248H;			// non-zero if interface is FT1248
    UCHAR FT1248CpolH;			// FT1248 clock polarity - clock idle high (1) or clock idle low (0)
    UCHAR FT1248LsbH;			// FT1248 data is LSB (1) or MSB (0)
    UCHAR FT1248FlowControlH;	// FT1248 flow control enable
    UCHAR IsVCPH;				// non-zero if interface is to use VCP drivers
    UCHAR PowerSaveEnableH;		// non-zero if using ACBUS7 to save power for self-powered designs

} FT_PROGRAM_DATA, *PFT_PROGRAM_DATA;




typedef struct ft_eeprom_header {
    FT_DEVICE deviceType;		// FTxxxx device type to be programmed
    // Device descriptor options
    WORD VendorId;				// 0x0403
    WORD ProductId;				// 0x6001
    UCHAR SerNumEnable;			// non-zero if serial number to be used
    // Config descriptor options
    WORD MaxPower;				// 0 < MaxPower <= 500
    UCHAR SelfPowered;			// 0 = bus powered, 1 = self powered
    UCHAR RemoteWakeup;			// 0 = not capable, 1 = capable
    // Hardware options
    UCHAR PullDownEnable;		// non-zero if pull down in suspend enabled
} FT_EEPROM_HEADER, *PFT_EEPROM_HEADER;


// FT232B EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_232b {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
} FT_EEPROM_232B, *PFT_EEPROM_232B;


// FT2232 EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_2232 {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR AIsHighCurrent;		// non-zero if interface is high current
    UCHAR BIsHighCurrent;		// non-zero if interface is high current
    // Hardware options
    UCHAR AIsFifo;				// non-zero if interface is 245 FIFO
    UCHAR AIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR AIsFastSer;			// non-zero if interface is Fast serial
    UCHAR BIsFifo;				// non-zero if interface is 245 FIFO
    UCHAR BIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR BIsFastSer;			// non-zero if interface is Fast serial
    // Driver option
    UCHAR ADriverType;			//
    UCHAR BDriverType;			//
} FT_EEPROM_2232, *PFT_EEPROM_2232;


// FT232R EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_232r {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR IsHighCurrent;		// non-zero if interface is high current
    // Hardware options
    UCHAR UseExtOsc;			// Use External Oscillator
    UCHAR InvertTXD;			// non-zero if invert TXD
    UCHAR InvertRXD;			// non-zero if invert RXD
    UCHAR InvertRTS;			// non-zero if invert RTS
    UCHAR InvertCTS;			// non-zero if invert CTS
    UCHAR InvertDTR;			// non-zero if invert DTR
    UCHAR InvertDSR;			// non-zero if invert DSR
    UCHAR InvertDCD;			// non-zero if invert DCD
    UCHAR InvertRI;				// non-zero if invert RI
    UCHAR Cbus0;				// Cbus Mux control
    UCHAR Cbus1;				// Cbus Mux control
    UCHAR Cbus2;				// Cbus Mux control
    UCHAR Cbus3;				// Cbus Mux control
    UCHAR Cbus4;				// Cbus Mux control
    // Driver option
    UCHAR DriverType;			//
} FT_EEPROM_232R, *PFT_EEPROM_232R;


// FT2232H EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_2232h {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR ALSlowSlew;			// non-zero if AL pins have slow slew
    UCHAR ALSchmittInput;		// non-zero if AL pins are Schmitt input
    UCHAR ALDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR AHSlowSlew;			// non-zero if AH pins have slow slew
    UCHAR AHSchmittInput;		// non-zero if AH pins are Schmitt input
    UCHAR AHDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BLSlowSlew;			// non-zero if BL pins have slow slew
    UCHAR BLSchmittInput;		// non-zero if BL pins are Schmitt input
    UCHAR BLDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BHSlowSlew;			// non-zero if BH pins have slow slew
    UCHAR BHSchmittInput;		// non-zero if BH pins are Schmitt input
    UCHAR BHDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    // Hardware options
    UCHAR AIsFifo;				// non-zero if interface is 245 FIFO
    UCHAR AIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR AIsFastSer;			// non-zero if interface is Fast serial
    UCHAR BIsFifo;				// non-zero if interface is 245 FIFO
    UCHAR BIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR BIsFastSer;			// non-zero if interface is Fast serial
    UCHAR PowerSaveEnable;		// non-zero if using BCBUS7 to save power for self-powered designs
    // Driver option
    UCHAR ADriverType;			//
    UCHAR BDriverType;			//
} FT_EEPROM_2232H, *PFT_EEPROM_2232H;


// FT4232H EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_4232h {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR ASlowSlew;			// non-zero if A pins have slow slew
    UCHAR ASchmittInput;		// non-zero if A pins are Schmitt input
    UCHAR ADriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BSlowSlew;			// non-zero if B pins have slow slew
    UCHAR BSchmittInput;		// non-zero if B pins are Schmitt input
    UCHAR BDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR CSlowSlew;			// non-zero if C pins have slow slew
    UCHAR CSchmittInput;		// non-zero if C pins are Schmitt input
    UCHAR CDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR DSlowSlew;			// non-zero if D pins have slow slew
    UCHAR DSchmittInput;		// non-zero if D pins are Schmitt input
    UCHAR DDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    // Hardware options
    UCHAR ARIIsTXDEN;			// non-zero if port A uses RI as RS485 TXDEN
    UCHAR BRIIsTXDEN;			// non-zero if port B uses RI as RS485 TXDEN
    UCHAR CRIIsTXDEN;			// non-zero if port C uses RI as RS485 TXDEN
    UCHAR DRIIsTXDEN;			// non-zero if port D uses RI as RS485 TXDEN
    // Driver option
    UCHAR ADriverType;			//
    UCHAR BDriverType;			//
    UCHAR CDriverType;			//
    UCHAR DDriverType;			//
} FT_EEPROM_4232H, *PFT_EEPROM_4232H;


// FT232H EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_232h {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR ACSlowSlew;			// non-zero if AC bus pins have slow slew
    UCHAR ACSchmittInput;		// non-zero if AC bus pins are Schmitt input
    UCHAR ACDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ADSlowSlew;			// non-zero if AD bus pins have slow slew
    UCHAR ADSchmittInput;		// non-zero if AD bus pins are Schmitt input
    UCHAR ADDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    // CBUS options
    UCHAR Cbus0;				// Cbus Mux control
    UCHAR Cbus1;				// Cbus Mux control
    UCHAR Cbus2;				// Cbus Mux control
    UCHAR Cbus3;				// Cbus Mux control
    UCHAR Cbus4;				// Cbus Mux control
    UCHAR Cbus5;				// Cbus Mux control
    UCHAR Cbus6;				// Cbus Mux control
    UCHAR Cbus7;				// Cbus Mux control
    UCHAR Cbus8;				// Cbus Mux control
    UCHAR Cbus9;				// Cbus Mux control
    // FT1248 options
    UCHAR FT1248Cpol;			// FT1248 clock polarity - clock idle high (1) or clock idle low (0)
    UCHAR FT1248Lsb;			// FT1248 data is LSB (1) or MSB (0)
    UCHAR FT1248FlowControl;	// FT1248 flow control enable
    // Hardware options
    UCHAR IsFifo;				// non-zero if interface is 245 FIFO
    UCHAR IsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IsFastSer;			// non-zero if interface is Fast serial
    UCHAR IsFT1248	;			// non-zero if interface is FT1248
    UCHAR PowerSaveEnable;		//
    // Driver option
    UCHAR DriverType;			//
} FT_EEPROM_232H, *PFT_EEPROM_232H;


// FT X Series EEPROM structure for use with FT_EEPROM_Read and FT_EEPROM_Program
typedef struct ft_eeprom_x_series {
    // Common header
    FT_EEPROM_HEADER common;	// common elements for all device EEPROMs
    // Drive options
    UCHAR ACSlowSlew;			// non-zero if AC bus pins have slow slew
    UCHAR ACSchmittInput;		// non-zero if AC bus pins are Schmitt input
    UCHAR ACDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ADSlowSlew;			// non-zero if AD bus pins have slow slew
    UCHAR ADSchmittInput;		// non-zero if AD bus pins are Schmitt input
    UCHAR ADDriveCurrent;		// valid values are 4mA, 8mA, 12mA, 16mA
    // CBUS options
    UCHAR Cbus0;				// Cbus Mux control
    UCHAR Cbus1;				// Cbus Mux control
    UCHAR Cbus2;				// Cbus Mux control
    UCHAR Cbus3;				// Cbus Mux control
    UCHAR Cbus4;				// Cbus Mux control
    UCHAR Cbus5;				// Cbus Mux control
    UCHAR Cbus6;				// Cbus Mux control
    // UART signal options
    UCHAR InvertTXD;			// non-zero if invert TXD
    UCHAR InvertRXD;			// non-zero if invert RXD
    UCHAR InvertRTS;			// non-zero if invert RTS
    UCHAR InvertCTS;			// non-zero if invert CTS
    UCHAR InvertDTR;			// non-zero if invert DTR
    UCHAR InvertDSR;			// non-zero if invert DSR
    UCHAR InvertDCD;			// non-zero if invert DCD
    UCHAR InvertRI;				// non-zero if invert RI
    // Battery Charge Detect options
    UCHAR BCDEnable;			// Enable Battery Charger Detection
    UCHAR BCDForceCbusPWREN;	// asserts the power enable signal on CBUS when charging port detected
    UCHAR BCDDisableSleep;		// forces the device never to go into sleep mode
    // I2C options
    WORD I2CSlaveAddress;		// I2C slave device address
    DWORD I2CDeviceId;			// I2C device ID
    UCHAR I2CDisableSchmitt;	// Disable I2C Schmitt trigger
    // FT1248 options
    UCHAR FT1248Cpol;			// FT1248 clock polarity - clock idle high (1) or clock idle low (0)
    UCHAR FT1248Lsb;			// FT1248 data is LSB (1) or MSB (0)
    UCHAR FT1248FlowControl;	// FT1248 flow control enable
    // Hardware options
    UCHAR RS485EchoSuppress;	//
    UCHAR PowerSaveEnable;		//
    // Driver option
    UCHAR DriverType;			//
} FT_EEPROM_X_SERIES, *PFT_EEPROM_X_SERIES;









#endif // FTDI_TYPES_H
