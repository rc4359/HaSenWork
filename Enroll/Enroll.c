  #include "msp430x22x4.h"
#include "../Header/Uer.h"
#include "../uz2400/uz2400Header.h"
//#include "../Header/Io.h"
typedef struct{
  unsigned int DestAddress;
  unsigned int NewPanID;
  unsigned int AssignedAdd;
}EnrollNewInfo;

extern UINT16 PanId;

#ifdef UZ2400_RX_ENABLE
extern DATA_FRM RxDATAFRM;
#endif

unsigned char EnrollTimer;
 /*************************************************************/

             void SettingEnrollTimer(void);
             void ReNewEnrollInfo(EnrollNewInfo *NewInfo);
             void Delay(unsigned int Cnt);
  
    //////////////////////////////////////////////////////////////////////



extern unsigned char EnrollTx(void);


  //////////  Enrol flag //////////
unsigned char EnrollFlag;






#define EnrlTmUp   (1<<0)   // 1 : time up 
#define OK 1
#define NG 0
#define EnrollCmd 0xFF
#define BoardCastPanID 0xFFFF
#define ReAckEnrollCmd 0xFE
#define EnrollWaitTim  20*5    // 1 seconed  use timer a

   
   


   /****************************************************************/


/********************************************************************************



*******************************************************************************/
unsigned char CheckEnrollHost(unsigned char *ResponPagPrt, EnrollNewInfo *NewInfo)

{
  unsigned char *NewInfoPrt; 
  unsigned int i,Temp;
  NewInfoPrt=(unsigned char *)&(*NewInfo);
  
  
   ResponPagPrt++;
   if(*ResponPagPrt!=ReAckEnrollCmd) 
     return NG;
   
   ResponPagPrt++;
   
   for(i=0;i<sizeof(*NewInfo);i++)
   {
     *NewInfoPrt=*ResponPagPrt;
     NewInfoPrt++;
     ResponPagPrt++;
   }
   
   Temp=NewInfo->DestAddress;
   NewInfo->DestAddress=((NewInfo->DestAddress&0x00ff)<<8)+(unsigned char)((Temp&0xff00)>>8);
   
   Temp=NewInfo->NewPanID;
   NewInfo->NewPanID=((NewInfo->NewPanID&0x00ff)<<8)+(unsigned char)((Temp&0xff00)>>8);
   
    _NOP();
   
   
   
   return OK;
  
  
  
}

/***************************************************************************


***************************************************************************/
void ReNewEnrollInfo(EnrollNewInfo *NewInfo)
{
  PanId=NewInfo->NewPanID;
  
  UzSetPanId(PanId);
}

#ifdef UZ2400_RX_ENABLE
/****************************************************************************

****************************************************************************/
unsigned char EnrollProcedrue(void)
{
  EnrollNewInfo NewInfoBuff;
//  DATA_FRM RxDATAFRMS;
  static unsigned char Test1;
  
  WakeUpUz2400();                 
  //UzEnablePA();
   Delay(65533);
   
  EnrollTx();                              // P1.0 = toggle
  SettingEnrollTimer();
  
  do{
       if((Uz24SpIN&SpiInt)==0)
       {
           if(AllRx(&RxDATAFRM))
           {
            
              if(CheckEnrollHost((unsigned char *)&RxDATAFRM.msdu,&NewInfoBuff))
              {
                
                 ReNewEnrollInfo(&NewInfoBuff);
                 UzSleep();
                 TACCTL0 &= ~CCIE;                       // TACCR0 interrupt enabled
                 return OK;
              }
           }
       }
  }while(!EnrollFlag&EnrlTmUp);
 //  Delay(65533);
   UzSleep();
  // ReNewEnrollInfo(&NewInfoBuff);
    TACCTL0 &= ~CCIE;                       // TACCR0 interrupt enabled
    return NG;
     
  
}
#endif /* #ifdef UZ2400_RX_ENABLE */
/**************************************************************************

***************************************************************************/
void Delay(unsigned int Cnt)
{
 do
 {
   
   Cnt--;
 }while(Cnt>0);
  
  
}
/**************************************************************************


***************************************************************************/
void SettingEnrollTimer(void)
{
    // P1IE&=~EnrollKey;                      // Enable  Enroll key IO change interrupt key
     TACCR0 = 50000;
     TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
     EnrollTimer=EnrollWaitTim;                // reset enroll time counter
     EnrollFlag&= ~EnrlTmUp;                   // clear Enroll time up flag // 0 : no time up 
     
     TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
     __bis_SR_register(GIE);                    // Enter LPM0 w/ interrupt
  
}

/***************************************************************************


***************************************************************************/
// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
     
    if((EnrollTimer--)==0)                     // if enroll is wait time up ser the time up flag to indicate
        EnrollFlag|= EnrlTmUp;                 // set flag                                            
     
    
    
}



