/*******************************************************************************

	UBEC (Uniband Electronic Corp.)
	

	Project : UZ2400 Simple RF API

	File : type.h
	
	Description: Data type defitions
	
	Platform: U-Zig Power Module with IAR C compiler
	
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

#ifndef TRUE
#define 	TRUE 0x1
#endif

#ifndef FALSE
#define	FALSE 0x0
#endif

//Define external memory data type
#define XUINT32	unsigned long __xdata 		
#define XUINT16	unsigned int __xdata	
#define XUINT8	unsigned char __xdata
#define XINT32	long __xdata 
#define XINT16	int __xdata
#define XINT8	char __xdata			
 
#define UINT32	unsigned long  		
#define UINT16	unsigned int 	
#define UINT8	unsigned char 		
#define INT32	long  
#define INT16	int
#define INT8	 	char 			
#define PUINT32	unsigned long *

#define u8   unsigned char
#define   uchar unsigned char

//define the BOOLN type
typedef	UINT8	BOOL;


