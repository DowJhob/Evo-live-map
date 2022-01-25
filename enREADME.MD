# EVO livemap

[Discord channel](https://discord.gg/MSFSn5ywRv)

[ru README](README.MD)

Application for editing mitsubishi controller tables on the 
fly in RAM. Works with OpenPort1.3 and OpenPort2.0. 
All descriptions of RAM tables are stored in xml files 
as in ecuflash, new attributes have been added to table 
descriptions indicating the location of tables in the 
controller's RAM.


Sample:

    <table name="High Octane Fuel Map" category="Fuel" address="37b4d" type="3D" level="4" swapxy="true" scaling="AFR" RAM_addr="FFFFA04D"> 
        <table name="Load" address="4fdc" type="X Axis" elements="18" scaling="Load" RAM_mut_number="0x00"/>
        <table name="RPM" address="4fb4" type="Y Axis" elements="15" scaling="RPM" RAM_mut_number="0x02"/>
    </table>
RAM_addr адрес таблы в ОЗУ,
RAM_mut_number номер в таблице MUT в ОЗУ

The program itself detects the type of interface J2534 or
 FTDI adapter, if there are installed drivers. When the
 start button is pressed, an attempt is made to connect 
 to the controller using an alternative protocol and, if 
 successful, the internal ID is read, then the application
 by the mask of the ID number tries to find and read the first
 found xml calibration file containing the ID number in the 
 name, if not, the file search window opens.
 And then all tables available for editing in RAM will be opened.

The xdf directory contains patches for tunerpto https://www.tunerpro.net/

Short description:

The Mitsu controller has a built-in DMA protocol, its 
implementation was recently written by Orion4096 aka Nanner55
 GitHub, but the first was jcsbanks in 2008, these topics are
 on evo, also in the evo10 controller there is a DMA protocol
 that modified tephra (I have not yet finished it implemented).
 The jcsbanks protocol is selected by default, all DMA tephra v7
 use it. At the moment, 90550001, 80770001, 96262709, 
 and 96532706, 88592715 have been tested on jcsbanks, 
 I will correct it when there are testers, 90550001 and 88590015
 have been tested on nanner55.

To get started, you need to specify the connection speed and click connect.

there is a bug and if the connection speed or log has changed, you need to poke
m into another input window for the changes to be applied
