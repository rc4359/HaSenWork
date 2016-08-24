
#define	ADXL345_Addr    0xA6	

void  Init_ADXL345(void);
void read_ADXL345(int *A_XReslut,int *A_YReslut,int *A_ZReslut);
unsigned char Single_WriteLSM303D(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);