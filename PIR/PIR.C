#include "PIR.H"


unsigned int ADCToVoltage(unsigned int ADCValue);
unsigned char PIRLogic(unsigned char PIRVol);
unsigned char OFFDelay(unsigned char PIRStatus);
extern unsigned char PIRISR(void);
////////////////////////////////////////////////////////





//unsigned char PIRLogic;  // 1: Active   0: nonactive
//unsigned char OccupancyStatus;   
long OFFDelayCnt=OFFDelaySec;


/**********************************************************************8


********************************************************************/
unsigned char PIRISR(void)
{
  return OFFDelay(PIRLogic(ADCToVoltage(ADC10MEM)));
  //OccupancyStatus=OFFDelay(PIRLogic(ADCToVoltage(ADC10MEM))); 
 // return OccupancyStatus;
}





/*************************************************************************


*************************************************************************/

unsigned int ADCToVoltage(unsigned int ADCValue)
{
    
  return (LDOOut*ADCValue)/1024;
  
  
  
  
}

/********************************************************************8


**********************************************************************/
unsigned char PIRLogic(unsigned char PIRVol)
{
   if((PIRVol<PIRLowWinVol)||(PIRVol>PIRHighWinVol))
      return 1;
   else
      return 0;
  
  
}



/***************************************************************8


*************************************************************************/
unsigned char OFFDelay(unsigned char PIRStatus)
{
     if(PIRStatus==1)
     {
       OFFDelayCnt=OFFDelaySec;
         return 1;
       
     }
     else
     {
       if(OFFDelayCnt>0)
       {
         OFFDelayCnt--;
         return 1;     
       }
       else
       {
         return 0; 
       }
     }
 
}


/*****************************************************************



******************************************************************/
unsigned char GetPIRIO(void)
{
  if((PIRIOIN&PIRIOPIN)== PIRIOPIN)
    return 1;
    else
      return 0;
 
  
  
}