/*******************************************************************************

       UBEC (Uniband Electronic Corp.)

       Project: Uz2400D, platform

       File: Uz2500.h

       Reference:
               Silicon Laboratories: C8051F124
               UBEC: UZ2500

       Note : 
               Copyright (C) 2009 Uniband Electronic Corporation, All rights reserved

********************************************************************************/
///////////////////////// uz2400 /////////////////////////
#if 0
unsigned char UzSecureTx(unsigned char SecMode, unsigned char *SecKey, unsigned char *DataPtr, unsigned char Length, unsigned char HeaderLength);
void InitSpi(void);
unsigned char UzTx(unsigned char *DataPtr, unsigned char Length);
unsigned char UzRx(unsigned char *RecvBuff);
unsigned char UzSecureRx(unsigned char SecMode, unsigned char *SecKey, unsigned char *InBuff);
void UzSleep(void);
void UzDisablePA(void);
void WakeUpUz2400(void);
void TestTx(unsigned int BatteryValue);
UINT8 UzTxMAC(DATA_FRM *FRMCTRL);
void InitUz2400(void);
void UzEnablePA(void);
void UzSetMacAddress(unsigned char *MacAddress);
void UzSetNwkAddr(unsigned int NwkAddr);
void UzSetChannel(unsigned char NewChannel);
void UzSetPanId(unsigned int PanId);
void InitZ2400(void);

void spi_lw (unsigned int Address, unsigned char Value);
void spi_sw(unsigned char Address, unsigned char Value);
UINT8 UzRxMAC(DATA_FRM *InPtr);
void TestRx(void);
unsigned char spi_lr(unsigned int address);
void SaveTxValue( unsigned int Humid,unsigned int tempr , unsigned long MsxxPress,unsigned int BatValue);
void UzWakUpDelay (unsigned int DealyCnt);

unsigned char AllRx(DATA_FRM *RxstruBuf);

#endif

#define MAX_BUFF_LENGTH	132
#define MAX_QUEUE_NUM	5

//Define the address
#define TXBASE  	0x0
#define TX_N 		0x0
#define TX_N_HDR    0x0
#define TX_N_LEN    0x1
#define TX_N_BASE   0x2
#define TX_BCN		0x80
#define TX_G1		0x100
#define TX_G2		0x180
                    
#define REGBASE     0x200
#define RXBASE      0x300
#define RX_N		0x300
#define KEYBASE     0x280
#define KEY_TX_N	0x280
#define KEY_TX_B	0x2a0
#define KEY_TX_G1	0x290
#define KEY_TX_G2	0x2a0
#define KEY_RX_N	0x2b0

// short-address registers (SREGXX)
#define	RXMCR		0x00
#define	PANIDL		0x01
#define	PANIDH		0x02
#define	SADRL		0x03
#define	SADRH		0x04
#define	EADR0		0x05
#define	EADR1		0x06
#define	EADR2		0x07
#define	EADR3		0x08
#define	EADR4		0x09
#define	EADR5		0x0a
#define	EADR6		0x0b
#define	EADR7		0x0c
#define	RXFLUSH		0x0d
#define TXSTATE0    0x0e
#define TXSTATE1    0x0f
#define ORDER		0x10
#define TXMCR		0x11
#define ACKTMOUT 	0x12
#define SLALLOC		0x13
#define SYMTICKL	0x14
#define SYMTICKH	0x15
#define	PAONTIME	0x16 
#define PAONSETUP  	0x17
#define FIFOEN		0x18
#define CSMACR		0x19
#define	TXBCNTRIG	0x1a
#define	TXNMTRIG	0x1b
#define	TXG1TRIG	0x1c
#define	TXG2TRIG	0x1d
#define	ESLOTG23	0x1e
#define	ESLOTG45	0x1f
#define	ESLOTG67	0x20
#define	TXPEND		0x21
#define	TXBCNINTL	0x22
#define	ALIGNOFFSET	0x23	//FRMOFFSET Ori
#define	TXSR		0x24
#define	TXLNERR 	0x25
#define GATE_CLK	0x26
#define	TXPRM		0x27
#define	HSYMTMR0	0x28
#define	HSYMTMR1	0x29
#define	SOFTRST		0x2a
#define BISTCR      0x2b
#define	SECCR0		0x2c
#define	SECCR1		0x2d
#define TXPEMISP   	0x2e
#define	SECISR		0x2f
#define	RXSR		0x30
#define	ISRSTS		0x31
#define	INTMSK		0x32
#define LASTRXSATUS 0x33	//GPIO Ori
#define SPIRXF     	0x34	//GPIO_DIR Ori
#define SLPACK      0x35
#define RFCTL       0x36
#define SECCR2      0x37
#define	BBREG0		0x38
#define	BBREG1		0x39
#define	BBREG2		0x3a
#define	BBREG3		0x3b
#define	BBREG4		0x3c
#define	BBREG5		0x3d
#define BBREG6    	0x3e	
#define RSSITHCCA   0x3f

// long-address registers (LREGXX)
#define RFCTRL0     0x200
#define RFCTRL1    	0x201
#define RFCTRL2     0x202
#define	RFCTRL3     0x203
#define RFCTRL4     0x204
#define RFCTRL5		0x205
#define RFCTRL6     0x206
#define RFCTRL7		0x207
#define RFCTRL8     0x208
#define SLPCAL1		0x209	//CAL1 Ori
#define SLPCAL2		0x20a	//CAL2 Ori
#define SLPCAL3		0x20b
#define SFCNTRH     0x20c
#define SFCNTRM     0x20d
#define SFCNTRL     0x20e
#define RFSTATE     0x20f
#define RSSI        0x210
#define CLKIRQCR	0x211
#define	SRCADRMODE 	0x212
#define	SRCADDR0	0x213
#define	SRCADDR1	0x214
#define	SRCADDR2	0x215
#define	SRCADDR3	0x216
#define	SRCADDR4	0x217
#define	SRCADDR5	0x218
#define	SRCADDR6	0x219
#define	SRCADDR7	0x21a
#define RXFRAMESTATE 	0x21b
#define SECSTATUS   	0x21c
#define STCCMP      	0x21d
#define HLEN        	0x21e
#define FLEN        	0x21f
#define SCLKDIV  		0x220
#define RXMBCNT			0x221
#define WAKETIMEL		0x222
#define WAKETIMEH		0x223
#define	TXREMCNTL		0x224
#define	TXREMCNTH		0x225
#define	TXMAINCNTL 		0x226
#define	TXMAINCNTM 		0x227
#define	TXMAINCNTH0 	0x228
#define	TXMAINCNTH1 	0x229
#define	RFMANUALCTRLEN 	0x22a
#define	RFMANUALCTRL 	0x22b
#define TxDACMANUALCTRL 0x22c
#define	RFMANUALCTRL2 	0x22d
#define TESTRSSI		0x22e
#define	TESTMODE 		0x22f
#define	ASSO_BCN_LADR0 	0x230
#define	ASSO_BCN_LADR1 	0x231
#define	ASSO_BCN_LADR2 	0x232
#define	ASSO_BCN_LADR3 	0x233
#define	ASSO_BCN_LADR4 	0x234
#define	ASSO_BCN_LADR5 	0x235
#define	ASSO_BCN_LADR6 	0x236
#define	ASSO_BCN_LADR7 	0x237
#define	ASSO_BCN_SADR0 	0x238
#define	ASSO_BCN_SADR1 	0x239
#define	ASSO_BCN_CR		0x23a
#define	SEC_PAR_ORDER	0x23b

//for 2500
#define RX_FRM_TYPE		0x23c
#define GPIO_DIR		0x23d
#define GPIO			0x23e
#define UPPER_NONCE_0	0x240
#define UPPER_NONCE_1	0x241
#define UPPER_NONCE_2	0x242
#define UPPER_NONCE_3	0x243
#define UPPER_NONCE_4	0x244
#define UPPER_NONCE_5	0x245
#define UPPER_NONCE_6	0x246
#define UPPER_NONCE_7	0x247
#define UPPER_NONCE_8	0x248
#define UPPER_NONCE_9	0x249
#define UPPER_NONCE_A	0x24a
#define UPPER_NONCE_B	0x24b
#define UPPER_NONCE_C	0x24c
#define SEC_APP			0x24d
#define ENC_FLG			0x24e
#define RFCTL50			0x250
#define RFCTL51			0x251
#define RFCTL52			0x252
#define RFCTL53			0x253
#define RFCTL54			0x254
#define RFCTL55			0x255
#define RFCTL59			0x259
#define RF_STATUS0		0x25e
#define RFCTL73			0x273
#define RFCTL74			0x274
#define RFCTL75			0x275
#define RFCTL76			0x276
#define RFCTL77			0x277
#define INT_CTR0		0x27a
#define INT_CTR1		0x27b


typedef struct _TXSR_s_ {
	unsigned char	Txns:1,
					Txg1s:1,
					Txg2s:1,
					Txg1fnt:1,
					Txg2fnt:1,
					ccafail:1,
					txnretryn:2;
}TXSR_s;
typedef struct _ISRSTS_s_ {
	unsigned char	Txnr:1,
					Txg1r:1,
					Txg2r:1,
					RXOKIrq:1,
					SecIrq:1,
					HSymTmrIrq:1,
					WakAltIrq:1,
					SlpAltIrq:1;
}ISRSTS_s;	

typedef struct _RXFLUSH_s_ {
	unsigned char	RxFlush:1,
					OnlyBeacon:1,
					OnlyData:1,
					OnlyCmd:1,
					RXFF_test:1,
					Wakefpm:1,
					WakePol:1,
					resv:1;
}RXFLUSH_s;	

typedef struct _RXMCR_s_ {
	unsigned char 	Promi_mode:1,
					errpkt:1,
					isCord:1,
					isPANCord:1,
					MAC_lpbk:1,
					noRspACK:1,
					BB_lpbk:1,
					NoCRC:1;
}RXMCR_s;	


