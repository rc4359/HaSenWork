
//void UzShortCtrlWrt (unsigned char Address ,unsigned char WtrData);
unsigned char spi_sr (unsigned char Address);
void ShortSet (unsigned char Address ,unsigned char WtrData);
void LongSet(unsigned int Address, unsigned char Value);
unsigned char spi_lr(unsigned int Address);
void spi_rd_rx_fifo(unsigned char *DataPtr, unsigned char *RxLength);
void spi_fill_fifo(unsigned int address, unsigned char *DataPtr, unsigned char Length);

// ---------- Uz2400 --------------------------
#define WriteReadCmd    0x01

#define LongShorAddress 0x80