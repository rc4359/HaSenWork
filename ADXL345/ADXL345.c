#include "msp430x22x4.h"
#include "ADXL345.h"
#include "../Header/Uer.h"
#include "../i2c/i2c.h"
#include <math.h>


#define	M_SlaveAddress    0x3C
#define	A_SlaveAddress    0x30

int A_X,A_Y,A_Z;


unsigned char ADXLBUF[8];



/**********************************************************************



**********************************************************************/
void  Init_ADXL345(void)
{
	Single_Write(ADXL345_Addr,0x31,0x00);
	Single_Write(ADXL345_Addr,0x2C,0x08);
	Single_Write(ADXL345_Addr,0x2D,0x08);
	Single_Write(ADXL345_Addr,0x2E,0x80);
	Single_Write(ADXL345_Addr,0x1E,0x00);
	Single_Write(ADXL345_Addr,0x1F,0x00);
	Single_Write(ADXL345_Addr,0x20,0x05);
}


/*************************************************************8888


*******************************************************************/
void read_ADXL345(int *A_XReslut,int *A_YReslut,int *A_ZReslut)
{
       
       unsigned char i=0;
       for(i=0;i<8;i++)
       {
         ADXLBUF[i]=0; 
       }
  
  
       ADXLBUF[0]=Single_Read(ADXL345_Addr,0x32);//OUT_X_L_A
       ADXLBUF[1]=Single_Read(ADXL345_Addr,0x33);//OUT_X_H_A

	   ADXLBUF[2]=Single_Read(ADXL345_Addr,0x34);//OUT_Y_L_A
       ADXLBUF[3]=Single_Read(ADXL345_Addr,0x35);//OUT_Y_H_A

	   ADXLBUF[4]=Single_Read(ADXL345_Addr,0x36);//OUT_Z_L_A
       ADXLBUF[5]=Single_Read(ADXL345_Addr,0x37);//OUT_Z_H_A

	   A_X=(ADXLBUF[1]<<8)+ADXLBUF[0];
	   A_Y=(ADXLBUF[3]<<8)+ADXLBUF[2];
	   A_Z=(ADXLBUF[5]<<8)+ADXLBUF[4];
           
	  
		   *A_XReslut = A_X;
	  
		   *A_YReslut = A_Y;
	  
		   *A_ZReslut = A_Z;
	


}
unsigned char Single_WriteLSM303D(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //發送設備地址+寫信號//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//設置高起始地址+器件地址
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //設置低起始地址
    I2C_WaitAck();
    I2C_SendByte(REG_data);
    I2C_WaitAck();
    I2C_Stop();
    return TRUE;

}
