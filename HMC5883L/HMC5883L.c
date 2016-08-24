
#include "msp430x22x4.h"
#include "HMC5883L.h"
#include "../Header/Uer.h"
#include "../i2c/i2c.h"
#include <math.h>

unsigned char BUF[8];

int   x,y,z;
float angle;
uchar ge,shi,bai,qian,wan,shiwan;


extern SENSORDATA_T zSensorInfo;

short data_xyz[3];
float Roll,Pitch,Q,T,K;

/*****************************************************************8


******************************************************************/
 void  Init_HMC5883L(void)
{
   Single_Write(HMC5883L_Addr,0x00,0x14);   //
   Single_Write(HMC5883L_Addr,0x02,0x00);   //
}
  unsigned int TextAng;
 
        

////////////////////////////////////////////////////////////////////
void read_hmc5883l(int *Hx,int *Hy,int *Hz,int *fangleResult)
{
       unsigned char i=0;
     
       unsigned int TempAng;

       for(i=0;i<8;i++)
       {
         BUF[i]=0; 
       }
  
       Single_Write(HMC5883L_Addr,0x00,0x14);   //
       Single_Write(HMC5883L_Addr,0x02,0x00);   //
  	   Delayms(10);

       BUF[1]=Single_Read(HMC5883L_Addr,0x03);//OUT_X_L_A
       BUF[2]=Single_Read(HMC5883L_Addr,0x04);//OUT_X_H_A

	   BUF[3]=Single_Read(HMC5883L_Addr,0x07);//OUT_Y_L_A
       BUF[4]=Single_Read(HMC5883L_Addr,0x08);//OUT_Y_H_A

	   BUF[5]=Single_Read(HMC5883L_Addr,0x05);//OUT_Y_L_A
       BUF[6]=Single_Read(HMC5883L_Addr,0x06);//OUT_Y_H_A

       z=(BUF[1] << 8) | BUF[2]; //Combine MSB and LSB of X Data output register
       y=(BUF[3] << 8) | BUF[4]; //Combine MSB and LSB of Z Data output register
       x=(BUF[5] << 8) | BUF[6];

      // if(x>0x7fff)x-=0xffff;
      // if(y>0x7fff)y-=0xffff;
      // if(z>0x7fff)z-=0xffff;
       
       *Hx = x;
       *Hy = y;
       *Hz = z;

#if 1
       TextAng= atan2( x,y)* (180 / 3.14159265) +180;

       {
         angle= TextAng;
       }
      
       *fangleResult=(int)angle;
#endif /* caculate angle of digit compree by c# */
}
