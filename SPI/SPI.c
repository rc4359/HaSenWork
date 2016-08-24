#include  <msp430x22x2.h>
#include "../Header/Uer.h"
#include "SPI.h "

#define  TXBUF0    UCA0TXBUF
#define  RXBUF0    UCA0RXBUF
static unsigned int i;

void InitSpi(void)
{
 
 
//  P3OUT |= SpiCs;                             // Set slave reset
 // P3DIR |= SpiCs;                            //
  P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
  UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
  
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x02;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
 // IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

  //P3OUT &= ~0x40;                           // Now with SPI signals initialized,
 // P3OUT |= SpiCs;                              // reset slave

  for (i = 100; i > 0; i--);                 // Wait for slave to initialize

  
}
/*

void ShortSet (unsigned char Address ,unsigned char WtrData)
{
  Uz24SpOUT=Uz24SpOUT&(~SpiCs);
     Address=(Address<<1)|0x01;
  volatile unsigned int i;
   for (i = 0x00a0; i > 0; i--);           // Delay
    
    
//  Address=Address<<1;
//  Address=Address|WriteReadCmd;      // setting as Write command
 // Address=Address&(~LongShorAddress);  // short address
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  
  UCA0TXBUF = Address;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
 
  UCA0TXBUF = WtrData;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  
   for (i = 0x00a0; i > 0; i--);           // Delay
  _NOP();
   _NOP();
    _NOP();
     _NOP();
  Uz24SpOUT=Uz24SpOUT|SpiCs;
}

*/
void ShortSet (unsigned char Address ,unsigned char WtrData)
{
    
   Uz24SpOUT=Uz24SpOUT&(~SpiCs);
 // volatile unsigned int i;
 // for (i = 0x00a0; i > 0; i--);           // Delay
     Address = (Address << 1) | 0x0001; //Shfit the Address
     /*
  Address=Address<<1;
  Address=Address|WriteReadCmd;      // setting as Write command
  Address=Address&(~LongShorAddress);  // short address
   */
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
 
  UCA0TXBUF = Address;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
   
  UCA0TXBUF=WtrData;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
 // UCA0TXBUF =  0X00;
  
  _NOP();
  _NOP();
  _NOP();
 
 // for (i = 0x00a0; i > 0; i--);           // Delay

  Uz24SpOUT=Uz24SpOUT|SpiCs;
  
  
}
void spi_sw(unsigned char Address, unsigned char Value)
{
  
   Uz24SpOUT=Uz24SpOUT&(~SpiCs);
 // volatile unsigned int i;
 // for (i = 0x00a0; i > 0; i--);           // Delay
     Address = (Address << 1) | 0x0001; //Shfit the Address
     /*
  Address=Address<<1;
  Address=Address|WriteReadCmd;      // setting as Write command
  Address=Address&(~LongShorAddress);  // short address
   */
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
 
  UCA0TXBUF = Address;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
   
  UCA0TXBUF =  Value;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  _NOP();
  _NOP();
  _NOP();
//UCA0TXBUF =  0X00;
 // for (i = 0x00a0; i > 0; i--);           // Delay

  Uz24SpOUT=Uz24SpOUT|SpiCs;
  
  
  
}


unsigned char spi_sr (unsigned char Address)
{
 Uz24SpOUT=Uz24SpOUT&(~SpiCs);
 // volatile unsigned int i;
 // for (i = 0x00a0; i > 0; i--);           // Delay
  
  Address=Address<<1;
  Address=Address&(~WriteReadCmd);      // setting as Write command
  Address=Address&(~LongShorAddress);  // short address
   
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF = Address;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF = 0x00;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

 // for (i = 0x00a0; i > 0; i--);           // Delay
  _NOP();
   _NOP();
    _NOP();
     _NOP();
  Uz24SpOUT=Uz24SpOUT|SpiCs;
  
  return UCA0RXBUF;
  
  
  
}
void LongSet(unsigned int Address, unsigned char Value)
{
  unsigned char AddHight;
  unsigned char AddLow;
  
  Uz24SpOUT=Uz24SpOUT&(~SpiCs);
 
  Address = (Address << 5) | 0x8010;
  
  AddLow= Address&0xff;
  AddHight=(Address>>8)&0xff;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=AddHight;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=AddLow;
  
   while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=Value;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  _NOP();
    _NOP();
     _NOP();
  
  Uz24SpOUT=Uz24SpOUT|SpiCs;
  
}
//void LoadTxFifo(void)

void spi_lw (unsigned int Address, unsigned char Value)
{
  unsigned char AddHight;
  unsigned char AddLow;
  
  Uz24SpOUT=Uz24SpOUT&(~SpiCs);
 
  Address = (Address << 5) | 0x8010;
  
  AddLow= Address&0xff;
  AddHight=(Address>>8)&0xff;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=AddHight;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=AddLow;
  
   while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF=Value;
  
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  _NOP();
    _NOP();
     _NOP();
  
  Uz24SpOUT=Uz24SpOUT|SpiCs;
  
}
/*
unsigned char spi_lr(unsigned int Address)
{
  unsigned char AddHight;
  unsigned char AddLow;
  Address = (Address << 5) | 0x8000;
  
  
  
  
  
}*/


void spi_fill_fifo(unsigned int address, unsigned char *DataPtr, unsigned char Length){

	 unsigned char AddHigh;
         unsigned char AddLow;
       
          Uz24SpOUT=Uz24SpOUT&(~SpiCs);
        volatile unsigned int i;
        for (i = 0x00a0; i > 0; i--);           // Delay
        
        
         address = (address << 5) | 0x8010;
   //address = (address << 5) | 0x8001;
        AddLow= address&0xff;
        AddHigh=(address>>8)&0xff;
          
  	
  	
    
        
        
         //////////////////////  address high   ////////////////////////
         while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
         UCA0TXBUF=AddHigh;                        // RXBUF0 to TXBUF0
         
         
         //////////////////////////////////////////////////////////////////
         
         
          
         //////////////////////  address low   ////////////////////////
         while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
         UCA0TXBUF=AddLow;                        // RXBUF0 to TXBUF0
         
         

	while (Length--){
      	      while (!(IFG2 & UCA0TXIFG));              // USART0 TX buffer ready?
              
          
             UCA0TXBUF = *DataPtr++; //Write the data
      		
  	}
 
  	

        _NOP();
  	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

        _NOP();
    _NOP();
     _NOP();         // Delay
        Uz24SpOUT=Uz24SpOUT|SpiCs;
	
}

void spi_rd_rx_fifo(unsigned char *DataPtr, unsigned char *RxLength){
	/*unsigned char Length;
        
	//DisInt();
	

	//Set up the SFRPAGE
  	Uz24SpOUT=Uz24SpOUT&(~SpiCs); //Put the CS to Low
        
        
          volatile unsigned int i;
         for (i = 0x00a0; i > 0; i--);           // Delay
        
          while (!(IFG2 & UCA0TXIFG));              // USART0 TX buffer ready?
         //SpiOut &= ~0x01;                         // Latch data into 'HC165
         //SpiOut |= 0x01;
 
    
         TXBUF0 = 0xE0;                        // RXBUF0 to TXBUF0
         
         //////////////////////////////////////////////////
         
          while (!(IFG2 & UCA0TXIFG));              // USART0 TX buffer ready?
         //SpiOut &= ~0x01;                         // Latch data into 'HC165
        //SpiOut |= 0x01;
 
    
         TXBUF0 = 0x00;                        // RXBUF0 to TXBUF0
         
         ///////////////////////////////////////////////////

  	  while (!(IFG2 & UCA0TXIFG));             // USART0 TX buffer ready?
         //SpiOut &= ~0x01;                         // Latch data into 'HC165
        /// SpiOut |= 0x01;
 
    
         TXBUF0 =0x00;                        // RXBUF0 to TXBUF0
         
         for (i = 0x00a0; i > 0; i--);           // Delay
    //     Temp = RXBUF0;

    //     Temp = RXBUF0;   
         *RxLength = RXBUF0;
          Length = RXBUF0;
      
          _NOP();
         
         ///////////////////////////////////////////////////

  	

  	

  	while(Length--){
      		 
      		while (!(IFG2 & UCA0TXIFG)); 
                TXBUF0 = 0x00; //Write the NULL data
      			*DataPtr++ = RXBUF0;
                    //     for (i = 0x00a0; i > 0; i--);           // Delay
      			
  	}
         for (i = 0x00aA; i > 0; i--);           // Delay
        
  	Uz24SpOUT=Uz24SpOUT|SpiCs;


  */
	unsigned char Length;
      
  	
        Uz24SpOUT=Uz24SpOUT&(~SpiCs);
        volatile unsigned int i;
        for (i = 0x00a0; i > 0; i--);           // Delay
        
        ////////////////////  write Address hight //////////////////////////
        
         while (!(IFG2 & UCA0TXIFG));              // USART0 TX buffer ready?
         
         UCA0TXBUF = 0xE0;                        // RXBUF0 to TXBUF0
         
         //////////////////////////////////////////////////////////////////
         
         
         
         
         
         
         ////////////////////  write Address low   //////////////////////////
         while (!(IFG2 & UCA0TXIFG));              // USART0 TX buffer ready?
         
 
    
         UCA0TXBUF = 0x00;                        // RXBUF0 to TXBUF0
         
         ///////////////////////////////////////////////////
         
         
         
         
         

  	 while (!(IFG2 & UCA0TXIFG));               // USART0 TX buffer ready?
         UCA0TXBUF =0x00;                        // RXBUF0 to TXBUF0
         
           for (i = 0x00a0; i > 0; i--);           // Delay
        
   
         *RxLength = UCA0RXBUF;
          Length = UCA0RXBUF;
      
          _NOP();
         
         ///////////////////////////////////////////////////

  	

  	//volatile unsigned int i;
        for (i = 0x00a0; i > 0; i--);           // Delay

  	while(Length--){
      		 
      		while (!(IFG2 & UCA0TXIFG));  
                UCA0TXBUF = 0x00; //Write the NULL data
      			*DataPtr++ = UCA0RXBUF;
                    //     for (i = 0x00a0; i > 0; i--);           // Delay
      			
  	}
      
        
  	


        while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
       
        _NOP();
    _NOP();
     _NOP();
        Uz24SpOUT=Uz24SpOUT|SpiCs;
  

}

unsigned char spi_lr(unsigned int address){
	unsigned char Value;

        unsigned char AddHigh;
         unsigned char AddLow;
         
	//Set up the SFRPAGE
  	 Uz24SpOUT=Uz24SpOUT&(~SpiCs);
         
         
  	address = (address << 5) | 0x8000;

        
           AddLow= address&0xff;
        AddHigh=(address>>8)&0xff;
        
  	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
        UCA0TXBUF=AddHigh;
        
        while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
        UCA0TXBUF=AddLow;
  

  	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
         UCA0TXBUF = 0x00;
  
        while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

           for (i = 0x00a0; i > 0; i--);           // Delay
          _NOP();
         _NOP();
         _NOP();
            _NOP();

  	Uz24SpOUT=Uz24SpOUT|SpiCs;

       


  	return UCA0RXBUF;
}
