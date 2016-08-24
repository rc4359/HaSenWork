/*******************************************************************************

	UBEC (Uniband Electronic Corp.)
	

	Project : UZ2400 Simple RF API

	File : data.h
	
	Usage : Definitions for Sample Code
	
	Platform: U-Zig Power Module with IAR 8051 C compiler
	
	Reference: 

		Silicon Laboratories: C8051F124
		UBEC: UZ2400
	
	Note : 	


	Uniband Electronic Corporation
	3F-3, No. 192 Dongguang Rd., Hsinchu, 300 Taiwan
	Tel : 886-3-5729898, Fax : 886-3-5718599
	http: //www.ubec.com.tw


		Copyright (C) 2005 Uniband Electronic Corporation, All rights reserved
					
********************************************************************************/

//Address Mode
#define ADDR_NONE 0x00
#define ADDR_16 0x02 
#define ADDR_64 0x03

// Frame Control of IEEE 802.15.4
typedef union _mac_frame_control_{
	UINT16 FrmCtrl;

	struct{
		UINT8 SrcAddrMode:2;
		UINT8 Reserved1:2;
		UINT8 DestAddrMode:2;
		UINT8 Reserved0:3;
		UINT8 IntraPan:1;
		UINT8 AckReq:1;
		UINT8 Pending:1;
		UINT8 Security:1;
		UINT8 FrameType:3;
	}FrmCtrl_bit;
}MAC_FRM_CTRL;

typedef union _addr_info_{
	UINT16 Short;
	UINT8 Long[8];
}ADDR_INFO;

typedef struct _mac_frame_{
	MAC_FRM_CTRL Header;
	UINT8 Sequence;
	UINT16 DstPanId;
	ADDR_INFO DstAddr;
	UINT16 SrcPanId;
	ADDR_INFO SrcAddr;
	UINT8 *Payload;
}MAC_FRAME;

//MCPS-DATA.request
typedef struct _mcps_data_req_{

	UINT8 SrcAddrMode;
	UINT16 SrcPANId;
	ADDR_INFO SrcAddr;
	UINT8 DstAddrMode;
	UINT16 DstPANId;
	ADDR_INFO DstAddr;
	UINT32 msduLength;
	UINT8 msduHandle;
	UINT8 TxOptions;
	UINT8 SecureMode;
	BOOL ACKREQ;
	UINT8 msdu[32];
}DATA_FRM;

typedef struct _interrupt_flag_{
    UINT8 TxN		:1;
    UINT8 TxG1   	:1;
    UINT8 TxG2   	:1;
    UINT8 Rx     	:1;
    UINT8 Sec    	:1;
    UINT8 Timer  	:1;
    UINT8 Wakeup 	:1;
    UINT8 Sleep  	:1;
}INTERRUPT_STS;



typedef struct
{
  unsigned char PANIDHigh;
  unsigned char PANIDLow;
  unsigned char AddressHigh; 
  unsigned char AddressLow;
  unsigned char DstAdd[20];
}FlashInfo;

typedef struct
{

	int ADX_X;
	int ADX_Y;
	int ADX_Z;
	int L3G_X;
	int L3G_Y;
	int L3G_Z;
	int HMC_INT_X;
	int HMC_INT_Y;
	int HMC_INT_Z;
	int HmAng;
}SENSORDATA_T;
