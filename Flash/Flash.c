#include "msp430x22x4.h"
#include "../Header/Io.h"
#include "../Header/Uer.h"


#define FlashSegeC 0x1040
#define FlashSegeD 0x1000
//#define FlashSegeSize 64


void ReadFlash(unsigned int FSegeStrAdd,FlashInfo *SavedData);
void InitSaveData(unsigned int *PANID,unsigned int *Add,unsigned int *DstAddrIndex,unsigned char DesSize);
void WriteSaveData(unsigned int *PANID,unsigned int *Add,unsigned int *DstAddrIndex,unsigned char DesSize);
void write_Seg(unsigned int Sege,FlashInfo WriteData,unsigned char DataSize);

/*******************************************************************************



******************************************************************************/

void InitFlash(void)
{
   FCTL2 = FWKEY + FSSEL0 + FN3;             // MCLK/3 for Flash Timing Generator
  
}


/*********************************************************************8


8********************************************************************/
void InitSaveData(unsigned int *PANID,unsigned int *Add,unsigned int *DstAddrIndex,unsigned char DesSize)
{
  FlashInfo RestoreData;
  unsigned char Cnt;
  unsigned char *Flash_ptr;  
 
  
  
   ReadFlash(FlashSegeD,&RestoreData);
  _NOP();
  
                       ////// PANID ////
  
  *PANID=(unsigned int)RestoreData.PANIDHigh;
  *PANID<<=8;
  *PANID+=RestoreData.PANIDLow;
  
                      ///// SCRADD ////////
   *Add=(unsigned int)RestoreData.AddressHigh;
   *Add<<=8;
   *Add+=RestoreData.AddressLow;
  // DesSize=64;
   Cnt=0;
   do
   {
     *DstAddrIndex=RestoreData.DstAdd[(Cnt)];
      
     *DstAddrIndex<<=8;
  
      *DstAddrIndex+=RestoreData.DstAdd[(Cnt+1)];
      Cnt+=2;
      DstAddrIndex++;
   }while(Cnt<DesSize);
   
    _NOP();
}


/*************************************************************************8


****************************************************************************/
void ReadFlash(unsigned int FSegeStrAdd,FlashInfo *SavedData)
{
   
  unsigned char *Flash_ptr;                         // Segment D pointer
 // FlashInfo SavedData;
  unsigned char *InfoIndex;
  unsigned char Cnt;

  InfoIndex=(unsigned char *)&(*SavedData);
  //InfoIndex=&SavedData;
  
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
 // FCTL3 = FWKEY;                            // Clear Lock bit
  
 // Flash_ptr = (char *)FSegeStrAdd;              // Initialize Flash segment D ptr
  Flash_ptr = (unsigned char *)FSegeStrAdd;              // Initialize Flash segment D ptr
  Cnt=sizeof(*SavedData);
 // Cnt=64;
  do
  {
     *InfoIndex++=*Flash_ptr++;
    Cnt--;
  }while(Cnt>0);
//  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  _NOP();
  
  
  _NOP();
}
/***************************************************************************



***************************************************************************/
void write_Seg(unsigned int Sege,FlashInfo WriteData,unsigned char DataSize)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i;
  unsigned char *WriteIndex;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  
  WriteIndex=(unsigned char *)&WriteData;
  Flash_ptr = (char *)Sege;               // Initialize Flash pointer
  while (FCTL3&BUSY); 
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg
  while (FCTL3&BUSY); 
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i = 0; i < DataSize; i++)
  
  {
    *Flash_ptr++ =*WriteIndex;                   // Write value to flash
   //  *Flash_ptr++ =0xcc;                   // Write value to flash
     while (FCTL3&BUSY); 
    WriteIndex++;
  }
  
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}




/*****************************************************************************



8*****************************************************************************/
void WriteSaveData(unsigned int *PANID,unsigned int *Add,unsigned int *DstAddrIndex,unsigned char DesSize)
{
 static FlashInfo WriteData;
  unsigned char Cnt;
  
  WriteData.PANIDLow=(unsigned char)(*PANID&0xff);
  WriteData.PANIDHigh=(unsigned char)(*PANID>>8);
  
  
  WriteData.AddressLow=(unsigned char)(*Add&0xff);
  WriteData.AddressHigh=(unsigned char)(*Add>>8);
 
  Cnt=0;
  do
  {
    WriteData.DstAdd[Cnt+1]=(unsigned char)(*DstAddrIndex&0xff);
    WriteData.DstAdd[Cnt]=(unsigned char)(*DstAddrIndex>>8);
    Cnt+=2;
    *DstAddrIndex++;
  }while(Cnt<DesSize);
  
  write_Seg(FlashSegeD,WriteData,DesSize);
  
  
}