/****************************************************************************88



******************************************************************************/
unsigned char UzSecureTx(unsigned char SecMode, unsigned char *SecKey, unsigned char *DataPtr, unsigned char Length, unsigned char HeaderLength);
void InitSpi(void);
unsigned char UzTx(unsigned char *DataPtr, unsigned char Length);
unsigned char UzRx(unsigned char *RecvBuff);
unsigned char UzSecureRx(unsigned char SecMode, unsigned char *SecKey, unsigned char *InBuff);
void UzSleep(void);
void UzDisablePA(void);
void WakeUpUz2400(void);
void TestTx(SENSORDATA_T *zSensorInfoData);
UINT8 UzTxMAC(DATA_FRM *FRMCTRL);
void InitUz2400(void);
void UzEnablePA(void);
void UzSetMacAddress(unsigned char *MacAddress);
void UzSetNwkAddr(unsigned int NwkAddr);
void UzSetChannel(unsigned char NewChannel);
void UzSetPanId(unsigned int PanId);
void InitZ2400(void);
unsigned char spi_sr (unsigned char Address);
void spi_lw (unsigned int Address, unsigned char Value);
void spi_sw(unsigned char Address, unsigned char Value);
UINT8 UzRxMAC(DATA_FRM *InPtr);
void TestRx(void);
unsigned char AllRx(DATA_FRM *RxstruBuf);
unsigned char spi_lr(unsigned int address);

void UzWakUpDelay (unsigned DealyCnt);
unsigned char CheckRFACK(void);