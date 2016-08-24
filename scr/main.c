#include "msp430x22x4.h"
#include "../Header/Uer.h"
#include "../Header/RFID.h"
#include "../i2c/i2c.h"
#include "../uz2400/uz2400Header.h"
#include "../HMC5883L/HMC5883L.h"
#include "../L3G4200D/L3G4200D.h"
#include "../ADXL345/ADXL345.h"
#include <math.h>

static unsigned long CaAALCnt;
#define	M_SlaveAddress    0x3C
#define	LSM303M_SlaveAddress   0x3C   //定義器件在IIC總線中的從地址,(磁場)根據ALT    ADDRESS地址引腳不同修改
#define	A_SlaveAddress    0x30

#ifdef PERIOD_16MS
#define CaTime         40*15  //10se
#else

#define CaTime         40  //10se
#endif 
//extern void ReadFlash(unsigned int FSegeStrAdd,unsigned int *PANID,unsigned int *NetAdd,unsigned int *AddrIndex);
extern unsigned char PIRISR(void);
extern void ReadFlash(unsigned int FSegeStrAdd,FlashInfo *SavedData);

extern void WriteSaveData(unsigned int *PANID,unsigned int *Add,unsigned int *DstAddrIndex,unsigned char DesSize);
extern unsigned char EnrollProcedrue(void);
extern void Delay(unsigned int Cnt);
extern unsigned char GetPIRIO(void);

void PIRInitADC(void);
unsigned int  Multiple_read(unsigned char SlaveAddress,unsigned char REG_Address,int *x,int *y,int *z);
void FillSensorData(SENSORDATA_T *zFillSensorInfoData);
unsigned char PIRMask(unsigned char *Counter,unsigned char Constant);
void MotionHandller(unsigned char CurrentOcp,unsigned char *PIRBit);
void InitLSM303D(void);
void InitailIo(void);
void L3AceMtr(int *A_x,int *A_y,int *A_z);
 
int GetBatAdcValue(void);
void ADC_Init(void);

unsigned char OccupancyStatus;   // 1: occpancy   0:Non-Occupancy
unsigned char EnrollTx(void);
unsigned char lastOccupancyStatus;
unsigned char PIRMaskBit=0;
unsigned char PIRMaskCnt;
unsigned int BatValue;
unsigned char WdtCouner=0;

// (r8 / (r4 + r8))  3.3k/ 13.3k = 0.248
// vADC = (ADC Raw data / 1024 (10bit ) * 1.5V )
// vADC = vBAT * 0.248
#define LOW_BAT_THRD  460 //2.7v
#define PIRMaskTime	  2
//#define TX_TEST 


//#define MotionSensorID     0x03
//#define MainController     0x01
//#define MainController     0x2222




/* Sensor result variable*/
//int   ADX_X,ADX_Y,ADX_Z;
//short L3G_X,L3G_Y,L3G_Z;
//float hmcangle;
SENSORDATA_T zSensorInfo;

#define SENSOR_RF_LEN  sizeof(zSensorInfo)


////////////////////////////////////////////////////////////////////////////
UINT8 SecKeyPool[16] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf}; //Secure Key
 
UINT16 PanId = 0x0001;
UINT16 BoardcastPanId = 0xffff;
UINT8 SrcMacAddr[8] = {0x11, 0x11, 0x11, 0x11, 0xaa, 0xbb, 0xcc, 0xdd};
UINT8 DstMacAddr[8] = {0x22, 0x22, 0x22, 0x22, 0xaa, 0xaa, 0xaa, 0xaa};
UINT16 dstAddr = Baseunit;
UINT16 SrcAddr = MY_ID;
UINT16 NonUse;
unsigned int DstAddr[10]={0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x8888, 0x9999, 0xaaaa};
BOOL RF_RX_FLAG = 0;
DATA_FRM TxDATAFRM;

DATA_FRM RxDATAFRM;


//#define SourDevice   MotionSensorID
//#define TouchSwLCD   MainController 
#define DataLen      sizeof(zSensorInfo)+10 // scr addr  + dist addr+ start+ Zsensorinfo+ stop + CR + (FCL..rssi)
#define SENSOR_DATA_START 0x02
#define SENSOR_DATA_STOP  0x03
#define   I_AM_A   0

unsigned char readtest;
/*******************************************************************************



*******************************************************************************/

void main(void)
{
   
 WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
 BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8MHz
  DCOCTL = CALDCO_8MHZ;
 Delay(2000);
 InitailIo();
  InitSpi();
  ADC_Init();
  if(PanId==0xffff){PanId=0xa5a3;}
  
  InitUz2400();
  
//   WriteSaveData(&PanId,&SrcAddr,DstAddr,(sizeof(DstAddr)+4));

  IE1 |= WDTIE;                             // Enable WDT interrupt
  
  P2DIR &= (~(1<<1)); 
  //TestTx(1);
#ifdef SensorLS303
  InitLSM303D();
  Init_L3G4200D();
#else
 Init_HMC5883L();
 Init_L3G4200D();
 Init_ADXL345();
#endif
ShortSet(0X00,0X03);
                _NOP();
		readtest=spi_sr(0X00);
                _NOP();
                readtest=spi_sr(0X03);
                _NOP();
                readtest=spi_sr(0X04);
                _NOP();
                readtest=spi_sr(0X00);
                _NOP();
TestTx(&zSensorInfo);
 //UzSleep();  
 // BCSCTL3|=((1<<2)+(1<<3));
   WDTCTL = WDT_ADLY_250 ;
  //WDTCTL = WDT_ADLY_16          ;                   // WDT 1s/4 interval timer
  __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts

}



void InitLSM303D(void)
{
	Single_WriteLSM303D(M_SlaveAddress,0x00,0x14);   //
    Single_WriteLSM303D(M_SlaveAddress,0x02,0x00);   //
	Single_WriteLSM303D(A_SlaveAddress,0x20,0x27);   //測量範圍,正負2g，16位模式
}


/****************************************************************************/

void InitailIo(void)
{
  
    
    //---------- uz2400 spi ------------------------
    Uz24SpDIR=Uz24SpDIR|SpiCs|SpiNC;
   Uz24SpOUT=Uz24SpOUT|SpiCs;
   Uz24SpDIR=Uz24SpDIR&(~SpiInt);
   
     // -------------- enroll key ------------------
   EnrollPort&=~EnrollKey;

#ifdef P1_2_SENSOR_CONF

#else
   P1IE|=EnrollKey;
   P1IES |= EnrollKey;                            // P1.2 Hi/lo edge
   P1IFG &= ~EnrollKey;;                           // P1.2 IFG cleared
#endif /* P1_2_SENSOR_CONF */
  
   PIRIODIR&=(~PIRIOPIN);
   
   tInt1_input;
   aInt1_input;
   IntDrdy_input;
   
   SCL_output;
   SDA_output;
   
   SDA_H;
   SCL_H;

#ifdef LED_ENABLE
   LED_1_SET_OUT;
   LED_2_SET_OUT;
   LED_1_OFF;
   LED_2_OFF;
#endif /* LED_ENABLE*/
}

/***************************************************************************



****************************************************************************/
void PIRInitADC(void)
{
   ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
   ADC10AE0 |= PIRAnlog;                         // P2.1 ADC option select
   ADC10CTL1|=A1_PIR;
   
   
}



/**********************************************************************8



****************************************************************************/
void PIRADCOFF(void)
{
  
  ADC10CTL0 = ADC10CTL0&(~ADC10ON)&(~ADC10IE);   // ADC10ON OFF
  
  
}





/***************************************************************************




***************************************************************************/


#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  unsigned char Test1; 
  
  float value;
  
   WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
                       // ACLK/4

#ifdef 0//PERIOD_16MS
  if(++WdtCouner>WATCH_DOG_DIV)
  {
  WdtCouner=0;
#endif /* PERIOD_16MS */
	   RF_GOT_ACK_LED_OFF;
	   BatValue=GetBatAdcValue();
#ifdef LED_ENABLE

	   if(BatValue<LOW_BAT_THRD)
	   {
		   LOW_POER_LED_ON;
	   }
	   else
	   {
		   LOW_POER_LED_OFF;
	   }
#endif
	   if(CaAALCnt<=CaTime)
	   {
	     CaAALCnt+=1;
	   }

	   //InitailIo();
	  // PIRInitADC();
	  // ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	  // __bis_SR_register(CPUOFF+GIE);        // LPM0, ADC10_ISR will force exit

	   _NOP();
	   //if(PIRMaskBit)
	   // PIRMaskBit=PIRMask(&PIRMaskCnt,PIRMaskTime);
	  //  PIRADCOFF();
	   // if(!PIRMask(&PIRMaskCnt,PIRMaskTime))
	   //  MotionHandller(GetPIRIO(),&PIRMaskBit);


#if 0
            if ((zSensorInfo.ADX_X>=0)&& (zSensorInfo.ADX_Y>=0))
            {
                 value = (float)zSensorInfo.ADX_X;
                 zSensorInfo.AcAngValue = (int)((value / 1024) * 90);        
		 zSensorInfo.AcAngSig = 0; /* 無負號 */
            }
            else if ((zSensorInfo.ADX_X <= 0) && (zSensorInfo.ADX_Y>=0))
            {
                value = (float)(0xffff-zSensorInfo.ADX_X);
                 zSensorInfo.AcAngValue = (int)((value / 1024) * 90);        
		 zSensorInfo.AcAngSig = 1; /* 有負號 */
            }		
	   else if ((zSensorInfo.ADX_X>= 0) && (zSensorInfo.ADX_Y< 0))
            {
                value = (1024 - (float)zSensorInfo.ADX_Y) + 1204;
                 zSensorInfo.AcAngValue = (int)((value / 1024) * 90);        
		zSensorInfo.AcAngSig = 0; /* 無負號 */
         
            }
	  else if ((zSensorInfo.ADX_X<= 0) && (zSensorInfo.ADX_Y<= 0))
            {
                value =1024- (1024-(float)zSensorInfo.ADX_Y) + 1204;
                 zSensorInfo.AcAngValue = (int)((value / 1024) * 90);        
		 zSensorInfo.AcAngSig = 1; /* 有負號 */
            }
	  #endif


          
            
#if 0

#ifndef SensorLS303
	    read_hmc5883l(&zSensorInfo.HMC_INT_X,&zSensorInfo.HMC_INT_Y,&zSensorInfo.HMC_INT_Z,&zSensorInfo.HmAng);
            read_L3G4200D(&zSensorInfo.L3G_X,&zSensorInfo.L3G_Y,&zSensorInfo.L3G_Z);
	    read_ADXL345(&zSensorInfo.ADX_X,&zSensorInfo.ADX_Y,&zSensorInfo.ADX_Z);
#else
	    while(Uz24SpIN&SpiInt){};

	    AllRx(&RxDATAFRM);

        Test1=spi_sr(0x31);
		Test1=spi_sr(0x0d);
		spi_sw(0x0d,(Test1|0x01));


	     if((RxDATAFRM.msdu[0]==MY_ID)&&(RxDATAFRM.msdu[2]==MY_ID))
         {

				L3AceMtr(&zSensorInfo.ADX_X,&zSensorInfo.ADX_Y,&zSensorInfo.ADX_Z);
				Multiple_read(LSM303M_SlaveAddress,0X03,&zSensorInfo.HMC_INT_X,&zSensorInfo.HMC_INT_Y,&zSensorInfo.HMC_INT_Z); //連續讀出數據，存儲在BUF中
			   // read_L3G4200D(&zSensorInfo.L3G_X,&zSensorInfo.L3G_Y,&zSensorInfo.L3G_Z);

				zSensorInfo.HmAng= atan2(zSensorInfo.HMC_INT_X,zSensorInfo.HMC_INT_Y)* (180 / 3.14159265) +180;
         }
#endif
 TestTx(&zSensorInfo);
#else

#ifndef SensorLS303
	    read_hmc5883l(&zSensorInfo.HMC_INT_X,&zSensorInfo.HMC_INT_Y,&zSensorInfo.HMC_INT_Z,&zSensorInfo.HmAng);
            read_L3G4200D(&zSensorInfo.L3G_X,&zSensorInfo.L3G_Y,&zSensorInfo.L3G_Z);
	    read_ADXL345(&zSensorInfo.ADX_X,&zSensorInfo.ADX_Y,&zSensorInfo.ADX_Z);
#else

        L3AceMtr(&zSensorInfo.ADX_X,&zSensorInfo.ADX_Y,&zSensorInfo.ADX_Z);
	    Multiple_read(LSM303M_SlaveAddress,0X03,&zSensorInfo.HMC_INT_X,&zSensorInfo.HMC_INT_Y,&zSensorInfo.HMC_INT_Z); //連續讀出數據，存儲在BUF中
	   // read_L3G4200D(&zSensorInfo.L3G_X,&zSensorInfo.L3G_Y,&zSensorInfo.L3G_Z);

        zSensorInfo.HmAng= atan2(zSensorInfo.HMC_INT_X,zSensorInfo.HMC_INT_Y)* (180 / 3.14159265) +180;
#endif
  SIP:         
	    while(Uz24SpIN&SpiInt){};
           
	    AllRx(&RxDATAFRM);
	    
        	   Test1=spi_sr(0x31);
		Test1=spi_sr(0x0d);
		spi_sw(0x0d,(Test1|0x01));
        
	
	     if((RxDATAFRM.msdu[0]==MY_ID)&&(RxDATAFRM.msdu[2]==MY_ID))
             {
               TestTx(&zSensorInfo);
             }
             else
             {
               _NOP();
               goto SIP; 
             }
            
	    
#endif




#ifdef 0//PERIOD_16MS 
   }
#endif 





#ifdef PERIOD_16MS
    WDTCTL = WDT_ADLY_16;                   // WDT 1s/4 interval timer
#else
    WDTCTL = WDT_ADLY_250 ;
#endif 
    // WDTCTL = WDT_ADLY_1000 ;
}
			

/***************************************************************************



***************************************************************************/
void MotionHandller(unsigned char CurrentOcp,unsigned char *PIRBit)
{
 // if((CurrentOcp!=lastOccupancyStatus)&&(!*PIRBit))
#ifdef TX_TEST

#else  
  if(CurrentOcp!=lastOccupancyStatus)
#endif /* TX_TEST */
  {
     
      // WakeUpUz2400();                 
       //UzEnablePA();
       Delay(65533);  
      // TestTx(CurrentOcp);
       //UzDisablePA();
      // UzSleep();  
           *PIRBit=1;
           PIRMaskCnt=0;
     
   
  }
  
  
     lastOccupancyStatus=CurrentOcp;


}

/**************************************************************************



****************************************************************************/
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
 
  
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}






/**************************************************************************


**************************************************************************/
void InitUz2400(void)
{
 
   Uz24SpDIR=Uz24SpDIR|SpiCs;
   Uz24SpOUT=Uz24SpOUT|SpiCs;
   InitZ2400();
   UzEnablePA();
   UzSetChannel(13);
   UzSetPanId(FIXED_PANID );

    _NOP();

          UzSetMacAddress(SrcMacAddr);
       //   UzSetMacAddress(DstMacAddr);
            UzSetNwkAddr(MY_ID);


  
  
}

/***************************************************************************



*******************************************************************************/
void TestTx(SENSORDATA_T *zSensorInfoData)
{
   unsigned char Test1,Test2;

                TxDATAFRM.SrcAddr.Short = SrcAddr;
                TxDATAFRM.DstAddr.Short = dstAddr;
        
                TxDATAFRM.SrcAddrMode = ADDR_16;
                TxDATAFRM.DstAddrMode = ADDR_16;
                TxDATAFRM.SrcPANId = FIXED_PANID;
                TxDATAFRM.DstPANId = FIXED_PANID;
                TxDATAFRM.msduLength = DataLen ;
   

        FillSensorData(zSensorInfoData) ;
        
	TxDATAFRM.ACKREQ = 1;
	//TxDATAFRM.TxOptions = 0x00;//No Security
        TxDATAFRM.TxOptions = 0x08;//No Security
        
        
          if(UzTxMAC(&TxDATAFRM) == SUCCESS){
              
              Test1=spi_sr(0X24);
              _NOP();
              Test2=spi_sr(0X1B);
              _NOP();
              Test1=spi_sr(0X11);
              _NOP();
#ifdef LED_ENABLE
              RF_GOT_ACK_LED_ON;
#endif
          }else{
          _NOP();
          RF_GOT_ACK_LED_OFF;
          }
          
    return;
}

#ifdef UZ2400_RX_ENABLE
/********************************************************************8


***********************************************************************/
void TestRx(void){
  unsigned char Test1;
       // InitSpi();
	if(UzRxMAC(&RxDATAFRM) == SUCCESS){
                  _NOP();
                   
                     
                  Test1=spi_sr(0x0d );           // 
                   spi_sw(0x0d ,(Test1|=0x01));  // Clear rx fifo
                   Test1=spi_sr(0x31);          // clear interrupt
		//if(RxDATAFRM.msdu[0] == 0x01 && RxDATAFRM.msdu[1] == 0x02 && RxDATAFRM.msdu[2] == 0x03){
			//LED_SHOW();
		//}
                   Test1=spi_sr(0x00);
                     Test1&=(~0x03);
                     spi_sw(0x00,Test1);
                     Test1=spi_sr(0x00);
	}
        else
        {
          
           _NOP();
                 
        }
} 

#endif /* UZ2400_RX_ENABLE */
/**************************************************************

***************************************************************/
unsigned char AllRx(DATA_FRM *RxstruBuf){
  unsigned char Test1;

	if(UzRxMAC(&(*RxstruBuf)) == SUCCESS){
                  _NOP();
                 
                   
                    
                     
                  Test1=spi_sr(0x31);
                    _NOP();
                   
                   
                  //   Test1=spi_sr(0x00);
                  //   Test1&=(~0x03);
                  //   spi_sw(0x00,Test1);
                   //  Test1=spi_sr(0x00);
                   //  
                   return 1;
		
	}
        else
        {
          return 0;
      
                 
        }
} 


/**************************************************************************


**************************************************************************/
#define EnrollCmd 0xFF
#define BoardCastPanID 0xFFFF

unsigned char EnrollTx(void)
{
        unsigned char Test2,Test1;
                TxDATAFRM.SrcAddr.Short =MY_ID;
               TxDATAFRM.DstAddr.Short = Baseunit  ;
              //  TxDATAFRM.DstAddr.Short = 0xFFFF;
               //  TxDATAFRM.SrcAddr.Short = 0x1234;
                 
                 
                TxDATAFRM.SrcAddrMode = ADDR_16;
                TxDATAFRM.DstAddrMode = ADDR_16;
               TxDATAFRM.SrcPANId = PanId ;
              TxDATAFRM.DstPANId = BoardCastPanID ;	
            //  TxDATAFRM.DstPANId = 0x0001;
             //   TxDATAFRM.SrcPANId = 0x3456;	
                TxDATAFRM.msduLength = DataLen+1 ;
   

                TxDATAFRM.msdu[0]=0x00;
                TxDATAFRM.msdu[1]=EnrollCmd;
                TxDATAFRM.msdu[2]=MY_ID;
                
                
        TxDATAFRM.ACKREQ = 1;
	//TxDATAFRM.TxOptions = 0x00;//No Security
TxDATAFRM.TxOptions = 0x08;//No Security
    if(UzTxMAC(&TxDATAFRM) == SUCCESS){
	
        Test1=spi_sr(0X24);
        _NOP();
        Test2=spi_sr(0X1B);
        _NOP();
        Test1=spi_sr(0X11);
        _NOP();
    }else{
    _NOP();
    }

    return 0;
    
	/*TxDATAFRM.ACKREQ = 1;
	
        TxDATAFRM.TxOptions = 0x08;//No Security
        
        
          if(UzTxMAC(&TxDATAFRM) == SUCCESS){
              
            
          }else{
          _NOP();
          }
  
  
  
  return 0;*/
}

/***************************************************************************8



***************************************************************************/
void FillSensorData(SENSORDATA_T *zFillSensorInfoData)
{
      unsigned char i=0;
      unsigned char Index;
      unsigned char *bpSensorDataPrt;

      bpSensorDataPrt =(unsigned char *) zFillSensorInfoData;

      Index=0;
  
      
      
      TxDATAFRM.msdu[Index]=SENSOR_DATA_START ;
      Index++;
#ifdef P1_2_SENSOR_CONF

#ifdef CK_ENABLE
      if((isKeyRess)&&(CaAALCnt<CaTime))
     //   if(isKeyRess)
      {
           Delay(65); 
           if(isKeyRess)
           {
             Delay(650); 
             if(isKeyRess)
             {
            
                 TxDATAFRM.msdu[Index]=MY_ID | 0xf0;
                 Index++;
      
                 TxDATAFRM.msdu[Index]=Baseunit | 0xf0 ;
                 Index++;
             }
            
           }
      }
      else{
#endif /* CK_ENABLE */
    	  TxDATAFRM.msdu[Index]=MY_ID ;
    	            Index++;

    	            TxDATAFRM.msdu[Index]=Baseunit;
    	            Index++;

#ifdef CK_ENABLE
      }
#endif /* CK_ENABLE */
#else
      TxDATAFRM.msdu[Index]=MY_ID ;
      Index++;
      
      TxDATAFRM.msdu[Index]=Baseunit;
      Index++;
#endif /* P1_2_SENSOR_CONF */

      
      
      for(i=0;i<SENSOR_RF_LEN;i++)
      {
    	  TxDATAFRM.msdu[Index+i]= *bpSensorDataPrt;
    	  bpSensorDataPrt++;
      }

      TxDATAFRM.msdu[Index+i]= SENSOR_DATA_STOP;
      TxDATAFRM.msdu[Index+i+1]= 0X0D;
      //TxDATAFRM.msdu[Index+i+2]= 0x0a;
}


/***********************************************************************


***********************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
//  __bic_SR_register(GIE);  
  P1IFG &= ~EnrollKey;                              // P1.2 IFG cleared
  //EnrollTx(SourDevice);                           // P1.0 = toggle

#ifdef P1_2_SENSOR_CONF

#else
  EnrollProcedrue();                                // excute enroll procedure
  WriteSaveData(&PanId,&SrcAddr,DstAddr,(sizeof(DstAddr)+4));  
#endif /* P1_2_SENSOR_CONF */
  P1IFG &= ~EnrollKey;                              // P1.2 IFG cleared// P1.0 = toggle
  P1IE|=EnrollKey;                      // Enable  Enroll key IO change interrupt key
  
 //  __bis_SR_register(GIE); 
  
}



/**********************************************************************



***********************************************************************/
unsigned char PIRMask(unsigned char *Counter,unsigned char Constant)
{
  
  *Counter=*Counter+1;
  if(*Counter>Constant)
  {

   *Counter=0;
   return 0;
  }
   return 1;



}

void Delay(unsigned int Cnt)
{
   do
   {
     _NOP();
     Cnt--;
   }while(Cnt>0);
}
unsigned char L303BUF[6];

//*********************************************************
//連續讀出LSM303內部數據
//*********************************************************
void L3AceMtr(int *A_x,int *A_y,int *A_z)
{
double x,y;
int ax,ay;
  
	       L303BUF[0]=Single_Read(A_SlaveAddress,0x28);//OUT_X_A
	       L303BUF[1]=Single_Read(A_SlaveAddress,0x29);//OUT_X_A

	       L303BUF[2]=Single_Read(A_SlaveAddress,0x2A);//OUT_Y_A
	       L303BUF[3]=Single_Read(A_SlaveAddress,0x2B);//OUT_Y_A

	       L303BUF[4]=Single_Read(A_SlaveAddress,0x2C);//OUT_Z_A
	       L303BUF[5]=Single_Read(A_SlaveAddress,0x2D);//OUT_Z_A

	       *A_x=(L303BUF[1] << 8) | L303BUF[0]; //合成16位數據
	       *A_y=(L303BUF[3] << 8) | L303BUF[2]; //合成16位數據
	       *A_z=(L303BUF[5] << 8) | L303BUF[4]; //合成16位數據

	       x=(float)(*A_x/16.383);
	       y=(float)(*A_y/16.383);

	       *A_x/=16.383;  //mg
	       *A_y/=16.383;  //mg
	       *A_z/=16.383;  //mg


}


unsigned int  Multiple_read(unsigned char SlaveAddress,unsigned char REG_Address,int *x,int *y,int *z)
{   
  
       unsigned char i,test;
 float TempA;
        int M_x,M_y,M_z;

    	I2C_Start();
        I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//設置高起始地址+器件地址
        I2C_WaitAck();
        I2C_SendByte((u8) REG_Address);   //設置起始地址
        I2C_WaitAck();
        I2C_Start();
        I2C_SendByte(SlaveAddress+1);
        I2C_WaitAck();

    	for (i=0; i<6; i++)                //連續讀取6個地址數據，存儲中BUF
        {
            L303BUF[i] = I2C_RadeByte();       //BUF[]存儲
            if (i == 5)I2C_NoAck();        //最後一個數據需要回NOACK
              else I2C_Ack();              //回應ACK
        }
        I2C_Stop();

        M_x=(L303BUF[0] << 8) | L303BUF[1]; //合成16位數據
        M_z=(L303BUF[2] << 8) | L303BUF[3]; //合成16位數據
        M_y=(L303BUF[4] << 8) | L303BUF[5]; //合成16位數據
        TempA= atan2(M_y,M_x) * (180 / 3.14159265) + 180; // angle in degrees

        *x=M_x;
        *y=M_y;
        *z=M_z;
        return (int)TempA;


}

void ADC_Init(void)
{
	  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON ;


	  ADC10AE0 |= 0x01;                         // P2.0 ADC option select
}

int GetBatAdcValue(void)
{
	int ADC_temp;
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

	while(!(ADC10CTL0&ADC10IFG)){}

		ADC_temp = ADC10MEM;


	return ADC_temp;
}
