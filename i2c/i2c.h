#include "msp430x22x4.h"


//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改


#define	BMP085_Addr     0xee	//气压传感器器件地址


/* sensor int  */
#define tInt1_input    P1DIR&=(~(1<<4))
#define aInt1_input    P1DIR&=(~(1<<6))
#define IntDrdy_input  P1DIR&=(~(1<<7))

#define SDA_input  P3DIR&=(~(1<<1))
#define SDA_output P3DIR|=((1<<1))

#define SCL_output P3DIR|=((1<<2))

#define SDA_H   P3OUT|=(1<<1)
#define SDA_L   P3OUT&=~(1<<1)
#define SCL_H   P3OUT|=(1<<2)
#define SCL_L   P3OUT&=~(1<<2)
#define SDA_read P3IN&(1<<1)

/********************/
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
bool I2C_Start(void);
void I2C_delay(void);
void I2C_Stop(void);
void I2C_SendByte(u8 SendByte) ;
bool I2C_WaitAck(void);
void delay5ms(void);
unsigned char I2C_RadeByte(void);
void I2C_NoAck(void);
void Delayms(unsigned long m);
void I2C_Ack(void);
