

#define	HMC5883L_Addr   0x3C	

void  Init_HMC5883L(void);
void read_hmc5883l(int *Hx,int *Hy,int *Hz,int *fangleResult);
