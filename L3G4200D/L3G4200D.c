

#include "msp430x22x4.h"
#include "L3G4200D.h"
#include "../Header/Uer.h"
#include "../i2c/i2c.h"
#include <math.h>

int T_X,T_Y,T_Z;

unsigned char L3G42BUF[8]; 
/*************************************************************88



****************************************************************/
void Init_L3G4200D(void)
{
	Single_Write(L3G4200_Addr,CTRL_REG1, 0x0f);
	Single_Write(L3G4200_Addr,CTRL_REG2, 0x00);
	Single_Write(L3G4200_Addr,CTRL_REG3, 0x08);
	Single_Write(L3G4200_Addr,CTRL_REG4, 0x30);	//+-2000dps
	Single_Write(L3G4200_Addr,CTRL_REG5, 0x00);
}
/******************************************************************



******************************************************************/
int Last_TX;
int Last_TY;
int Last_TZ;

void read_L3G4200D(int *T_XReslut,int *T_YReslut,int *T_ZReslut)
{
   unsigned char i=0;
   for(i=0;i<8;i++)
   {
     L3G42BUF[i]=0; 
   }
  
   L3G42BUF[0]=Single_Read(L3G4200_Addr,OUT_X_L);
   L3G42BUF[1]=Single_Read(L3G4200_Addr,OUT_X_H);
   T_X=	(L3G42BUF[1]<<8)|L3G42BUF[0];
 

      *T_XReslut=T_X;
      Last_TX=T_X;
  

   L3G42BUF[2]=Single_Read(L3G4200_Addr,OUT_Y_L);
   L3G42BUF[3]=Single_Read(L3G4200_Addr,OUT_Y_H);
   T_Y=	(L3G42BUF[3]<<8)|L3G42BUF[2];
  

      *T_YReslut=T_Y;
      Last_TY=T_Y;

   
   
   L3G42BUF[4]=Single_Read(L3G4200_Addr,OUT_Z_L);
   L3G42BUF[5]=Single_Read(L3G4200_Addr,OUT_Z_H);
   T_Z=	(L3G42BUF[5]<<8)|L3G42BUF[4];
   

      *T_ZReslut=T_Z;
      Last_TZ=T_Z;
   
  
}
