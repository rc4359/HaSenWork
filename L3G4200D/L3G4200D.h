
#define	L3G4200_Addr    0xD2	
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

void Init_L3G4200D(void);
void read_L3G4200D(int *T_XReslut,int *T_YReslut,int *T_ZReslut);
