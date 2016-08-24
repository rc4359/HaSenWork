#include  <msp430x22x2.h>
#include "../Header/Uer.h"
#include "../Spi/SPI.h"
#include "uz2400Header.h"

#ifdef UZ2400D
#include "uz2400D.h"
#else
#include "uz2400.h"
#endif

extern UINT8 SecKeyPool[16];

#define ContlFarmLow 0
#define ContlFarmHigh 1







//////////////////////////////////////////////////////////////////////////
///
///
///  ªì©l¤Æ chip
///
///////////////////////////////////////////////////////////////////////////


void InitZ2400(void)
{
#ifdef UZ2400D
    _NOP();
	//do
	//	spi_sw(GATE_CLK, 0x20);
	//while((spi_sr(GATE_CLK)&0x20)!=0x20);
	spi_sw(PAONSETUP, 0x08);
	spi_sw(FIFOEN, 0x94);
	spi_sw(TXPEMISP, 0x95);
	spi_sw(BBREG3, 0x50);
	spi_sw(BBREG5, 0x07);
    spi_sw(BBREG6, 0x40);
	spi_lw(RFCTRL0,0x03);
	spi_lw(RFCTRL1,0x02);
	spi_lw(RFCTRL2, 0x66);
	spi_lw(RFCTRL4,0x07);
	spi_lw(RFCTRL6, 0x30);
	spi_lw(RFCTRL7, 0xe0);
	spi_lw(RFCTRL8, 0x8C);
	spi_lw(GPIO_DIR,0x00);
	spi_lw(SEC_APP,0x20);
	spi_lw(RFCTL50,0x07);
	spi_lw(RFCTL51,0xC0);
	spi_lw(RFCTL52,0x01);
	spi_lw(RFCTL59,0x00);
	spi_lw(RFCTL73,0x40);
	spi_lw(RFCTL74,0xc6);
	spi_lw(RFCTL75,0x13);
	spi_lw(RFCTL76,0x07);
	spi_sw(INTMSK, 0xf7);
	spi_sw(SLPACK, 0x7f);
	spi_sw(TXMCR,0x12);
	spi_sw(TXPEND,0x1c);
	spi_sw(SOFTRST, 0x02);
	spi_sw(RFCTL, 0x04);
	spi_sw(RFCTL, 0x00);
	//while(spi_lr (RFSTATE) != 0x50);
	spi_lw(TESTMODE, 0x29);
	//spi_lw(RFCTRL3, 0xF8);
#if PA2206
	spi_lw(RFCTRL3, 0xF8);
#endif
	
#else
   	spi_sw(SOFTRST, 0x03);
	spi_sw(BBREG2, 0x80); //Set CCA mode to ED
        
	spi_sw(BBREG6, 0x40);	
	spi_sw(RSSITHCCA, 0x60); //ED threshold for CCAs
	spi_sw(SLPACK, 0x5f); //20Mhz clock recovery time
	spi_lw(RFCTRL2, 0x80); //RF-PLL Control
	spi_lw(RFCTRL6, 0x90);
	spi_lw(RFCTRL7, 0x80); //Sleep clock selection, [6:7] = 10 : use internal ring oscillator
	spi_lw(RFCTRL8, 0x10); // RF VCO control
	spi_lw(SCLKDIV, 0x01); //Sleep clock frequency control
	spi_sw(INTMSK,  0xf7);  //Enable the Interrupt
	//Errata Issue
	spi_sw(FFOEN, 0x98);		//?
	spi_sw(TXPEMISP, 0x90);
	//Reset RF state machine
	spi_sw(RFCTL, 0x04);
	spi_sw(RFCTL, 0x00);
	spi_sw(SOFTRST,0x02);
#if PA2206
	spi_lw(RFCTRL3, 0x40);
#endif
#endif /* UZ2400D */
}

void UzEnablePA(void){
        _NOP();
#ifdef UZ2400D
    	/* 1->NG */
#if 0
        spi_lw(TESTMODE,  spi_lr(TESTMODE) |0x0f);
        spi_lw(RFCTRL3, 0x40); //Set Power
#endif
#if 1
    	/* 2 TX ok Rx-NG*/
        spi_lw(TESTMODE, 0x29);

#endif
        /*  3 */
        //spi_lw(0x22f, (spi_lr(0x22f)&~0x7)|0x1); //enable external PA automatically control


#else
	LongSet(TESTMODE, 0x0f);

#endif
        _NOP();
} 

/******************************************************************************8



******************************************************************************/

unsigned char CheckRFACK(void)
{
  unsigned char ReadBuff;
  ReadBuff=spi_sr(TXSR);
  
  if(ReadBuff&0x01)
    return FAILURE;
      else
        return SUCCESS;
  
}

void UzSetChannel(unsigned char NewChannel){ 

	if (NewChannel > 26 || NewChannel < 11) //Check Channel Range
		return; 
#ifdef UZ2400D
	    spi_lw(RFCTRL0, ((NewChannel - 11) << 4) + 0x03); //// Shift logic channel, Program Channel
#else
	    LongSet(RFCTRL0, ((NewChannel - 11) << 4) | 0x02); //// Shift logic channel, Program Channel
#endif
	    _NOP();
	//Reset RF state machine
	ShortSet(RFCTL, 0x04);
	ShortSet(RFCTL, 0x00);

}


/**************************************************************
	Function Name: UzSetPanId()
	Description: Set Pan Id
	Parameters: PanId -> 16bits PAN identifier
	Return Value: None
	Note: Pan Id is used as the hardware filter of RX Normal Mode 
**************************************************************/

void UzSetPanId(unsigned int PanId){
	unsigned char i;


	for(i=0; i<2; i++)
		ShortSet(PANIDL+i, *(((unsigned char *)&PanId)+i)); //Set Value
}


/**************************************************************
	Function Name: UzSetMacAddress()
	Description: Set Mac Address
	Parameters: MacAddress -> 64bits Mac Address Pointer
	Return Value: None
	Note: Mac Address is used as the hardware filter of RX Normal Mode 
**************************************************************/

void UzSetMacAddress(unsigned char *MacAddress){
	unsigned char i;

	for (i=0; i<8; i++)
       	ShortSet(EADR0+i, *(MacAddress+i)); //Set Value 
	
}


/**************************************************************
	Function Name: UzSetNwkAddr()
	Description: Set Network(16bits) Address
	Parameters: NwkAddr -> 16bits short Address
	Return Value: None
**************************************************************/

void UzSetNwkAddr(unsigned int NwkAddr){
	unsigned char i;



       for(i=0; i<2; i++){
		ShortSet(SADRL+i, *(((unsigned char *)&NwkAddr)+i)); //Set Value
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************
	Function Name: UzTxMAC()
	Description: Send a standard Data Frame of IEEE 802.15.4
	Parameters: *OutPtr -> Data Pointer (Refer to data.h : struct DATA_FRM)
	Return Value: SUCCESS -> Send Data Successful
	                     FAILURE -> Send Data Failure
**************************************************************/

UINT8 SeqNum = 0;

UINT8 UzTxMAC(DATA_FRM *FRMCTRL){
#ifdef UZ2400_BUFF_RESIZE
	UINT8 i, Pkt[64], Length = 0;
#else
	UINT8 i, Pkt[127], Length = 0;
#endif /* UZ2400_BUFF_RESIZE */
	UINT16 *FrmCtrl;
	
        Pkt[ContlFarmLow]=0X21;
        Pkt[ContlFarmHigh]=0X88;
   //      Pkt[ContlFarmLow]=0X88;
   //    Pkt[ContlFarmHigh]=0X21;
        Length +=2;  //Length of Frame Control 
        
        Pkt[Length] = SeqNum;//Frame Sequence Number
        
        SeqNum++; //Increase Sequence Number
	Length +=1; //Length of Sequence ddddddddddddddddddddddddddddddddddddddddddd
        
       
        Pkt[Length]=(UINT8 )FRMCTRL->DstPANId; //Fill Destination Pan Id
        Length +=1;
        Pkt[Length]=(UINT8 )(FRMCTRL->DstPANId>>8);
        Length +=1;
        
        
       
        
       
        Pkt[Length] = (UINT8 )FRMCTRL->DstAddr.Short;
        Length +=1;
         Pkt[Length] = (UINT8 )(FRMCTRL->DstAddr.Short>>8);
         Length +=1;
         
         
        Pkt[Length]=(UINT8 )FRMCTRL->SrcPANId; //Fill Destination Pan Id
        Length +=1;
        Pkt[Length]=(UINT8 )(FRMCTRL->SrcPANId>>8);
        Length +=1;
         
        
         Pkt[Length] = (UINT8 )FRMCTRL->SrcAddr.Short;
        Length +=1;
         Pkt[Length] = (UINT8 )(FRMCTRL->SrcAddr.Short>>8);
         Length +=1;
         
         
         
         for(i=0; i< FRMCTRL->msduLength; i++){ //Fill Data Into Frame Payload
		Pkt[Length+i] = *(FRMCTRL->msdu + i);
	}
	//memcpy((void *) (Pkt+Length),(void *)FRMCTRL->msdu,FRMCTRL->msduLength);
	
	Length += FRMCTRL->msduLength; // Length of Data

	
		
		return UzTx(Pkt, Length);
	
}


        ///////////////////////////////////////////////////////////////
	/*FrmCtrl = (UINT16 *) &Pkt[Length];
	*FrmCtrl = 0; //Reset Value
	*FrmCtrl |= 0x0001; //Frame Type : Data
	Length +=2;  //Length of Frame Control 

	if(FRMCTRL->ACKREQ)
		*FrmCtrl |= 0x0020;
		
	Pkt[Length] = SeqNum; //Frame Sequence Number
	SeqNum++; //Increase Sequence Number
	Length +=1; //Length of Sequence 

	
	*(UINT16 *) &Pkt[Length] = FRMCTRL->DstPANId; //Fill Destination Pan Id
	
	Length +=2; //Length of Destination Pan Id 

	if(FRMCTRL->DstAddrMode == ADDR_16){ //Fill Destination Address
		*FrmCtrl |= 0x0800; //16bits Short Address
		
		*(UINT16 *) &Pkt[Length] = FRMCTRL->DstAddr.Short;
		
		Length +=2; //Length of Destination Address 
	}else if(FRMCTRL->DstAddrMode == ADDR_64){ 
		*FrmCtrl |= 0x0c00; //64bits IEEE Address
		for(i=0; i<8; i++){
			Pkt[Length+i] = FRMCTRL->DstAddr.Long[i];
		}
		Length +=8;
	}

	
	*(UINT16 *) &Pkt[Length] = FRMCTRL->SrcPANId; //Fill Source Pan Id
	
	Length +=2; //Length of Source Pan Id 

	if(FRMCTRL->SrcAddrMode == ADDR_16){ //Fill Source Address
		*FrmCtrl |= 0x8000; //16bits Short Address
		
		*(UINT16 *) &Pkt[Length] = FRMCTRL->SrcAddr.Short;
		
		Length +=2; //Length of Source Address 
	}else if(FRMCTRL->SrcAddrMode == ADDR_64){
		*FrmCtrl |= 0xc000; //64bits IEEE Address
		for(i=0; i<8; i++){
			Pkt[Length+i] = FRMCTRL->SrcAddr.Long[7-i];
		}
		Length +=8;
	}

	for(i=0; i< FRMCTRL->msduLength; i++){ //Fill Data Into Frame Payload
		Pkt[Length+i] = *(FRMCTRL->msdu + i);
	}
	//memcpy((void *) (Pkt+Length),(void *)FRMCTRL->msdu,FRMCTRL->msduLength);
	
	Length += FRMCTRL->msduLength; // Length of Data

	if(FRMCTRL->TxOptions == 0x08){ //Security Enable
		*FrmCtrl |= 0x0008; // Set Secure Flag in Frame Control Filed
		
		return UzSecureTx(FRMCTRL->SecureMode, SecKeyPool, Pkt, Length, Length-FRMCTRL->msduLength);
	}else{
		
		return UzTx(Pkt, Length);
	}
}*/

/**************************************************************
	Function Name:  UzSecureTx()
	Description: Transmit Secure Data
	Parameters: SecMode -> Encryption Mode
	                   *SecKey -> Security Key, defined by user
	                   *DataPtr -> Data Pointer
	                   Length -> Data Length
	                   HeaderLength -> Length of Data Header	                
	Return Value: SUCCESS -> Send Data Successful
	                     FAILURE -> Send Data Failure 
**************************************************************/

//Security Mode
#define AES_CTR         	0x01
#define ASE_CCM_128     	0x02
#define AES_CCM_64      	0x03
#define AES_CCM_32      	0x04
#define AES_CBC_MAC_128 0x05
#define AES_CBC_MAC_64  0x06
#define AES_CBC_MAC_32  0x07

unsigned char UzSecureTx(unsigned char SecMode, unsigned char *SecKey, unsigned char *DataPtr, unsigned char Length, unsigned char HeaderLength){
	unsigned char Value;
	unsigned char ISRSTS_vlaue;
	//Fill TXFIFO
	
	LongSet(TX_N_LEN, Length); //Data Length
	spi_fill_fifo(TX_N_BASE, DataPtr, Length); //Data

	//Fill Security key
	spi_fill_fifo(KEY_TX_N, SecKey, 16);

	//Fill in cipher mode
	ShortSet(SECCR0, (spi_sr(SECCR0) & ~0x07) | SecMode);

	Value = spi_sr(TXNMTRIG);

	if(*(DataPtr) & 0x20){
		Value |= 0x05; //Set Ackreq(SREG0x1B[2]) if re-transmission is required
	}else{
		Value &= ~0x07; //Clear
		Value |= 0x01;
	}

	ShortSet(TXNMTRIG, Value); //Trigger Tx start with security

	while(1){
		ISRSTS_vlaue=spi_sr(ISRSTS);
		if((ISRSTS_vlaue&0x01)==0x01){
			ISRSTS_vlaue=ISRSTS_vlaue&0xfe; //Reset Status

			if(!(spi_sr(TXSR) & 0x01)) //Check TXFIFO release state
				return SUCCESS;
	 		else
				break;
		}	
	}

	return FAILURE;	
}


///////////////////////////////////////////////////////////////////////////
/**************************************************************
	Function Name: UzTx()
	Description: Send Raw Data
	Parameters: DataPtr -> Out Data Pointer
	                   Length -> Out data Length
	                   
	Return Value: SUCCESS -> Send Data Successful
	                     FAILURE -> Send Data Failure 
**************************************************************/

INTERRUPT_STS IntFlag = {0, 0, 0, 0, 0, 0, 0, 0}; // Interrupt Flag

unsigned char UzTx(unsigned char *DataPtr, unsigned char Length){
	unsigned char Value;
	unsigned char ISRSTS_vlaue;
        
        unsigned int i;
#ifdef UZ2400_BUFF_RESIZE
        unsigned char TxFIFO[64];
#else
        static unsigned char TxFIFO[128];
#endif
     //     LongSet(0x00,0x88); // Fill Data Length Into TXFIFO
        LongSet(0x00,0); //Header Length
	LongSet(TX_N_LEN, Length); // Fill Data Length Into TXFIFO

	spi_fill_fifo(TX_N_BASE, DataPtr, Length); //Fill Data Into TXFIFO

	Value = spi_sr(TXNMTRIG);

	if(*(DataPtr) & 0x20){
		Value |= 0x05; //Set Ackreq(SREG0x1B[2]) if re-transmission is required
	}else{
		Value &= ~0x07; //Clear
		Value |= 0x01;
	}
      
  /*      i=0;
        do
        {
          TxFIFO[i]=spi_lr(i);
          i++;
        }while(i<127);
    */   
        
       ShortSet(TXNMTRIG, Value);  //Set trigger bit(SREG0x1B[0]) to send packet This bit will be automatically cleared.
       // ShortSet(0X1b, 0X01);
	//Wait Interrupt
	while(1){
                ISRSTS_vlaue=spi_sr(ISRSTS);
                _NOP();
		if((ISRSTS_vlaue&0x01)==0x01){
		ISRSTS_vlaue=ISRSTS_vlaue&0xfe; //Reset Status

			if(!(spi_sr(TXSR) & 0x01)) //Check TXFIFO release state
				return SUCCESS;
	 		else
				break;
		}	
	}

	return FAILURE;	
}



/**************************************************************
	Function Name: 
	Description: Only effect Tx normal FIFO
	Parameters: None
	Return Value: None
**************************************************************/

INTERRUPT_STS UzGetIntState(){
	INTERRUPT_STS Int;

	*((UINT8 *)&Int) = spi_sr(ISRSTS); 
	return Int;
}



/////////////////////////////////////////////////////////////////////
UINT8 UzRxMAC(DATA_FRM *InPtr){
#ifdef UZ2400_BUFF_RESIZE
	UINT8 Buff[64], MacLength, Value, i, Length = 0;
#else
	UINT8 Buff[137], MacLength, Value, i, Length = 0;
#endif
	if(InPtr->TxOptions == 0x08){  // If Security Enable
		MacLength = UzSecureRx(InPtr->SecureMode, SecKeyPool, Buff);
	}else{
		MacLength = UzRx(Buff);
                _NOP();
	}

        
        
	if(MacLength > 0){  // Received Data
                MacLength=MacLength+3;  // 2byte control fram + 1byte paket length + fcs
		Length +=4; // Frame Control & Sequence Fileds

		
		InPtr->DstPANId = *(UINT16 *) &Buff[Length]; //Fill Destination Pan Id
		
		Length +=2; //Length of Destination Pan Id 

		Value = Buff[2] & 0x0f; //Check Destination Address Mode
		if(Value == 0x08){ //16bits Short Address
			InPtr->DstAddrMode = ADDR_16;
			
			InPtr->DstAddr.Short = *(UINT16 *) &Buff[Length]; //Fill Destination Address
			
			Length +=2; //Length of Destination Address
		}else if (Value == 0x0c){ //64bits IEEE Address
			InPtr->DstAddrMode = ADDR_64;
			for(i=0; i<8; i++){
				InPtr->DstAddr.Long[i] = Buff[Length+i];
			}
			Length +=8;
		}

		
		InPtr->SrcPANId = *(UINT16 *) &Buff[Length]; //Fill Source Pan Id
		
		Length +=2; //Length of Source Pan Id 

		Value = Buff[2] & 0xf0; //Check Source Address Mode
		if(Value == 0x80){ //16bits Short Address
			InPtr->SrcAddrMode = ADDR_16;
			InPtr->SrcAddr.Short = *(UINT16 *) &Buff[Length]; //Fill Source Address
			Length +=2; //Length of Source Address
		}else if (Value == 0xc0){ //64bits IEEE Address
			InPtr->SrcAddrMode = ADDR_64;
			for(i=0; i<8; i++){
				InPtr->SrcAddr.Long[i] = Buff[Length+i];
			}
			Length +=8;
		}

	//	InPtr->msduLength = MacLength - Length -2; //Payload Length , 2 is FCS bytes in Promiscuous
                InPtr->msduLength = MacLength - Length ; //Payload Length , 2 is FCS bytes in Promiscuous
		for(i=0; i<InPtr->msduLength; i++){ //Fill Payload into Buffer
			InPtr->msdu[i] = Buff[Length+i];
		}
		//memcpy((void *)InPtr->msdu, (void *)(Buff+Length), InPtr->msduLength);

		return SUCCESS;
	}

	return FAILURE;

}



/////////////
unsigned char UzSecureRx(unsigned char SecMode, unsigned char *SecKey, unsigned char *InBuff){
	unsigned char Length;

	if(IntFlag.Sec == 1){
		IntFlag.Sec = 0;
		
		spi_fill_fifo(KEY_RX_N, SecKey, 16);//Fill Secure key into FIFO

		ShortSet(SECCR0, (spi_sr(SECCR0) & ~0x38) | SecMode); //Fill cipher mode

		//Security start
		ShortSet(SECCR0, spi_sr(SECCR0) | 0x40); // Trigger Security Process 

		//Wait Interrupt
		while(1){
			if(IntFlag.Rx == 1){
				spi_rd_rx_fifo(InBuff, &Length); //Fetch Data from RXFIFO
				IntFlag.Rx =0; //Reset Interrupt Flag
				return Length;
			}
		}		
	}
		
	return 0;
}

////////////////////////////////////////////////////////////////////////////
/**************************************************************
	Function Name:  UzRx()
	Description: Receive Data fron RXFIFO
	Parameters: RecvBuff -> Data buff that put received data
	Return Value: reveived data length
**************************************************************/

unsigned char UzRx(unsigned char *RecvBuff){ 
	unsigned char Length;
        _NOP(); 
	if((Uz24SpIN&SpiInt)==0){ //Check Interrupt Status
		spi_rd_rx_fifo(RecvBuff, &Length); //Receive Data from RxFIFO
		IntFlag.Rx =0; //Reset Status
                _NOP();
		return Length;
	}

	return 0;
}
void ResetU24(void)
{
  unsigned char temp;
  temp=spi_sr(0x0d);
  _NOP();
  temp=temp|0X01;
  ShortSet(0x0d,temp);
  _NOP();
}
///////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////

void UzSleep(){

	/*
  unsigned char temp;
 temp=spi_sr(0x10);
  temp|=0xf0;
  ShortSet(0x10,temp);
  
  temp=spi_sr(0x11);
  temp&=0x20;
  ShortSet(0x11,temp);
  
  
  LongSet(0x229,0x80);*/
  
 spi_sw(SLPACK ,spi_sr(SLPACK) | 0x80);
}

/////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////
void UzDisablePA(){

	spi_lw(TESTMODE,  0x08);
}

/***********************************************************************
Wake up uz2400


************************************************************************/
void WakeUpUz2400(void)
{
  unsigned char temp;
  temp=spi_sr(0x22);
  temp|=0x40;
  ShortSet(0x22,temp);
  
  temp=spi_sr(0x22);
  temp&=(~0x40);
  ShortSet(0x22,temp);
  
  
}
