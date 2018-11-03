#pragma once
#include "J2534.h"
#include "init.h"
#include "Global_var.h"

using namespace System;
//using namespace System::Data;

unsigned char  MUT(unsigned char request) {

			
		 //	memset(&tx_msg, 0, sizeof(tx_msg));

		 //	tx_msg[1].ProtocolID = protocol;
	
		//	rx_msg[0] = tx_msg[1];

		 //	tx_msg[1].DataSize = 1;

		//	tx_msg[1].Data[0] = request;

		//	j2534.PassThruWriteMsgs(chanID, &tx_msg[1], &NumMsgs, 500);

	   //		NumMsgs = 10;
	
	   //		j2534.PassThruReadMsgs(chanID, &rx_msg[0], &NumMsgs, 500);

 return 1;//	rx_msg[0].Data[0];
}